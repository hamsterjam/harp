#include <globals.h>
#include <ECS.h>
#include <Console.h>
#include <harpMath.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/TextureAtlas.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/VisualSpec.h>

void initGlobals() {
    harp = new ECS(32, 8, 128);

    comp_position     = harp->createComponentType(sizeof(Vector<2, double>));
    comp_velocity     = harp->createComponentType(sizeof(Vector<2, double>));
    comp_acceleration = harp->createComponentType(sizeof(Vector<2, double>));
    flag_hidden       = harp->createFlagType();
    comp_visual       = harp->createComponentType(sizeof(VisualSpec));

    defaultShader = new Shader();
    defaultPrimitiveShader = new Shader(defaultPrimitiveVertSource, defaultPrimitiveFragSource);

    defaultPrim = new PrimitiveRenderer();
    TextureAtlas consoleFontAtlas("res/testfont.png", 8, 12, 0, 0);
    consoleFont = new FontRenderer(consoleFontAtlas, ' ', '~');

    Console::init(*defaultPrim, *consoleFont);
    console = &Console::getInstance();
}

void cleanupGlobals() {
    Console::cleanup();

    delete harp;

    delete defaultPrim;
    delete consoleFont;

    delete defaultShader;
    delete defaultPrimitiveShader;
}

ECS* harp = 0;

Component comp_position = 0;
Component comp_velocity = 0;
Component comp_acceleration = 0;
Component flag_hidden = 0;
Component comp_visual = 0;

Console* console = 0;

Shader* defaultShader = 0;
Shader* defaultPrimitiveShader = 0;

PrimitiveRenderer* defaultPrim = 0;
FontRenderer*      consoleFont = 0;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

bool shouldExit = false;
