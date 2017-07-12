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
    inputBuffer = (char*) malloc(sizeof(char) * 80);

    const char* testString = "The quick brown fox jumps over the lazy dog.";
    memcpy(inputBuffer, testString, sizeof(char) * 45);

    // ECS stuff
    id = harp->createEntity();
    Vector<2, double> pos = zeroVector<2, double>();
    pos[1] = 50;

    harp->setComponent(id, comp_position, &pos);

    Color hist  = rgbaToColor(0.05, 0.05, 0.05, 0.9);
    Color input = rgbaToColor(0.10, 0.10, 0.15, 0.9);

    spec.addRectangle(prim, 0, 16, SCREEN_WIDTH, SCREEN_HEIGHT - 50, 0, hist);
    spec.addRectangle(prim, 0, 0, SCREEN_WIDTH, 16,  0, input);
    spec.addText(font, inputBuffer, 3, 3-2);

    harp->setComponent(id, comp_visual, &spec);
}

Console::~Console() {
    harp->deleteEntity(id);
    free(inputBuffer);
}
