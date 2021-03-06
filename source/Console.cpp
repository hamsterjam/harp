#include <Console.h>

#include <cassert>
#include <string>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <ECS.h>
#include <harpMath.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/Color.h>
#include <graphics/VisualSpec.h>
#include <luaInterface.h>
#include <globals.h>

static int consoleLayer = 200;
static int textLayer = 201;

Console* Console::instance = 0;

Console::Console(PrimitiveRenderer& prim, FontRenderer& font) {
    this->open = false;
    this->font = &font;

    inputBuffer = "";

    for (int i = 0; i < logLines; ++i) {
        logBuffer[i] = "";
    }

    // ECS stuff
    id = harp.createEntity();
    Vec<2, double> pos = zeroVec<2, double>();
    pos[1] = SCREEN_HEIGHT;
    harp.setComponent(id, comp_position, &pos);

    Vec<2, double> vel = zeroVec<2, double>();
    harp.setComponent(id, comp_velocity, &vel);

    auto histColor = rgbaToColor(0.06, 0.06, 0.04, 0.9);
    auto histSpec  = getRectangleFillSpec(0, 16, SCREEN_WIDTH, logLines*12 + 2, histColor, prim);
    harp.setComponent(id, comp_visual, &histSpec);
    harp.setComponent(id, comp_layer,  &consoleLayer);

    inputBoxID = harp.createEntity();
    harp.setParent(inputBoxID, id);
    auto inputColor = rgbaToColor(0.10, 0.10, 0.15, 0.9);
    auto inputSpec  = getRectangleFillSpec(0, 0, SCREEN_WIDTH, 16, inputColor, prim);
    harp.setComponent(inputBoxID, comp_visual, &inputSpec);
    harp.setComponent(inputBoxID, comp_layer,  &consoleLayer);

    inputID = harp.createEntity();
    harp.setParent(inputID, id);
    auto inputTextSpec = getTextSpec(inputBuffer.c_str(), 3, 3-2, font);
    harp.setComponent(inputID, comp_visual, &inputTextSpec);
    harp.setComponent(inputID, comp_layer,  &textLayer);

    for (int i = 0; i < logLines; ++i) {
        logLineID[i] = harp.createEntity();
        harp.setParent(logLineID[i], id);
        auto spec = getTextSpec(logBuffer[i].c_str(), 3, 16 + 12*i, font);
        harp.setComponent(logLineID[i], comp_visual, &spec);
        harp.setComponent(logLineID[i], comp_layer,  &textLayer);
    }

    harp.setFlag(id, flag_hidden, true);
}

Console::~Console() {
    harp.deleteEntity(id);
    harp.deleteEntity(inputBoxID);
    harp.deleteEntity(inputID);
    for (int i = 0; i < logLines; ++i) harp.deleteEntity(logLineID[i]);
}

void Console::init(PrimitiveRenderer& prim, FontRenderer& font) {
    assert(!instance);
    instance = new Console(prim, font);
}

void Console::cleanup() {
    if (instance) {
        delete instance;
    }
}

Console& Console::getInstance() {
    return *instance;
}

void Console::toggle() {
    open = !open;
    if (open) harp.setFlag(id, flag_hidden, false);
}

void Console::update() {
    auto& pos = * (Vec<2, double>*) harp.getComponent(id, comp_position);
    auto& vel = * (Vec<2, double>*) harp.getComponent(id, comp_velocity);

    auto tarPos = zeroVec<2, double>();
    tarPos[1] = (open) ? SCREEN_HEIGHT - (logLines*12 + 2) - 16 : SCREEN_HEIGHT;

    if (tarPos == pos) return;

    vel = (tarPos - pos)*15.0;

    if (dot(vel, vel) < 0.1) {
        pos = tarPos;
        vel = zeroVec<2, double>();
        if (!open) harp.setFlag(id, flag_hidden, true);
    }
}

void Console::log(std::string message) {
    for (int i = logLines-2; i >= 0; --i) {
        logBuffer[i+1] = logBuffer[i];

        VisualSpec spec = getTextSpec(logBuffer[i+1].c_str(), 3, 16 + 12*(i+1), *font);
        harp.setComponent(logLineID[i+1], comp_visual, &spec);
    }

    logBuffer[0] = message;

    VisualSpec spec = getTextSpec(logBuffer[0].c_str(), 3, 16, *font);
    harp.setComponent(logLineID[0], comp_visual, &spec);
}

void Console::appendToInput(std::string text) {
    inputBuffer += text;

    // This could (read: will) invalidate the pointer in ECS
    VisualSpec spec = getTextSpec(inputBuffer.c_str(), 3, 3-2, *font);
    harp.setComponent(inputID, comp_visual, &spec);
}

void Console::backspace() {
    if (inputBuffer.size() == 0) return;
    inputBuffer.pop_back();
}

void Console::process() {
    log("-> " + inputBuffer);
    int error;
    error = luaL_loadstring(L, inputBuffer.c_str()) || lua_pcall(L, 0, 0, 0);
    inputBuffer = "";

    VisualSpec spec = getTextSpec(inputBuffer.c_str(), 3, 3-2, *font);
    harp.setComponent(inputID, comp_visual, &spec);

    if (error) {
        const char* errorMessage = lua_tostring(L, -1);
        log(std::string(errorMessage));
        lua_pop(L, 1);
    }
}
