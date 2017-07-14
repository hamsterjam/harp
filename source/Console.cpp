#include <Console.h>

#include <cstring>

#include <ECS.h>
#include <harpMath.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/Color.h>
#include <graphics/VisualSpec.h>
#include <globals.h>

Console::Console(PrimitiveRenderer& prim, FontRenderer& font) {
    inputBuffer = "";

    for (int i = 0; i < logLines; ++i) {
        logBuffer[i] = "";
    }

    open = false;

    // ECS stuff
    id = harp->createEntity();
    Vector<2, double> pos = zeroVector<2, double>();
    pos[1] = SCREEN_HEIGHT;
    harp->setComponent(id, comp_position, &pos);

    Vector<2, double> vel = zeroVector<2, double>();
    harp->setComponent(id, comp_velocity, &vel);

    auto histColor = rgbaToColor(0.06, 0.06, 0.04, 0.9);
    auto histSpec  = getRectangleFillSpec(prim, 0, 16, SCREEN_WIDTH, logLines*12 + 2, histColor);
    harp->setComponent(id, comp_visual, &histSpec);

    inputBoxID = harp->createEntity();
    harp->setComponent(inputBoxID, comp_parent, &id);
    harp->setComponent(inputBoxID, comp_position, &pos);
    auto inputColor = rgbaToColor(0.10, 0.10, 0.15, 0.9);
    auto inputSpec  = getRectangleFillSpec(prim, 0, 0, SCREEN_WIDTH, 16, inputColor);
    harp->setComponent(inputBoxID, comp_visual, &inputSpec);

    inputID = harp->createEntity();
    harp->setComponent(inputID, comp_parent, &inputBoxID);
    harp->setComponent(inputID, comp_position, &pos);
    auto inputTextSpec = getTextSpec(font, inputBuffer, 3, 3-2);
    harp->setComponent(inputID, comp_visual, &inputTextSpec);

    for (int i = 0; i < logLines; ++i) {
        logLineID[i] = harp->createEntity();
        harp->setComponent(logLineID[i], comp_parent,   &id);
        harp->setComponent(logLineID[i], comp_position, &pos);
        auto spec = getTextSpec(font, logBuffer[i], 3, 16 + 12*i);
        harp->setComponent(logLineID[i], comp_visual, &spec);
    }

    harp->setFlag(id, flag_hidden, true);
}

Console::~Console() {
    harp->deleteEntity(id);
    harp->deleteEntity(inputBoxID);
    harp->deleteEntity(inputID);
    for (int i = 0; i < logLines: ++i) harp->deleteEntity(logLineID[i]);
}

void Console::toggle() {
    open = !open;
    if (open) harp->setFlag(id, flag_hidden, false);
}

void Console::update() {
    auto& pos = * (Vector<2, double>*) harp->getComponent(id, comp_position);
    auto& vel = * (Vector<2, double>*) harp->getComponent(id, comp_velocity);

    auto tarPos = zeroVector<2, double>();
    tarPos[1] = (open) ? SCREEN_HEIGHT - (logLines*12 + 2) - 16 : SCREEN_HEIGHT;

    if (tarPos == pos) return;

    vel = (tarPos - pos)*15.0;

    if (dot(vel, vel) < 0.1) {
        pos = tarPos;
        vel = zeroVector<2, double>();
        if (!open) harp->setFlag(id, flag_hidden, true);
    }
}

void Console::log(std::string message) {
    for (int i = logLines-2; i>= 0; --i) {
        logBuffer[i+1] = logBuffer[i];
    }

    logBuffer[0] = message;
}

void Console::appendToInput(std::string text) {
    inputBuffer += text;
}

void Console::backspace() {
    if (inputBuffer.size() == 0) return;
    inputBuffer.pop_back();
}

void Console::process() {
    if (inputBuffer == "exit") {
        shouldExit = true;
    }
    log("--> " + inputBuffer);
    inputBuffer = "";
}
