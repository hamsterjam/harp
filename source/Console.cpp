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

    open = false;

    // ECS stuff
    id = harp->createEntity();
    Vector<2, double> pos = zeroVector<2, double>();
    pos[1] = SCREEN_HEIGHT;
    harp->setComponent(id, comp_position, &pos);

    Vector<2, double> vel = zeroVector<2, double>();
    harp->setComponent(id, comp_velocity, &vel);

    Color hist  = rgbaToColor(0.06, 0.06, 0.05, 0.9);
    Color input = rgbaToColor(0.10, 0.10, 0.15, 0.9);

    spec.addRectangle(prim, 0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 50, 0, hist);
    spec.addRectangle(prim, 0, 0, SCREEN_WIDTH, 16,  0, input);
    spec.addText(font, inputBuffer, 3, 3-2);
    harp->setComponent(id, comp_visual, &spec);

    harp->setFlag(id, flag_hidden, true);
}

Console::~Console() {
    harp->deleteEntity(id);
}

void Console::toggle() {
    open = !open;
    if (open) harp->setFlag(id, flag_hidden, false);
}

void Console::update() {
    auto& pos = * (Vector<2, double>*) harp->getComponent(id, comp_position);
    auto& vel = * (Vector<2, double>*) harp->getComponent(id, comp_velocity);

    auto tarPos = zeroVector<2, double>();
    tarPos[1] = (open) ? 50 : SCREEN_HEIGHT;

    if (tarPos == pos) return;

    vel = (tarPos - pos)*15.0;

    if (dot(vel, vel) < 0.1) {
        pos = tarPos;
        vel = zeroVector<2, double>();
        if (!open) harp->setFlag(id, flag_hidden, true);
    }
}

void Console::append(std::string text) {
    inputBuffer += text;
}

void Console::backspace() {
    if (inputBuffer.size() == 0) return;
    inputBuffer.pop_back();
}

void Console::clear() {
    inputBuffer = "";
}

void Console::process() {
    // Just delete the string for now
    clear();
}
