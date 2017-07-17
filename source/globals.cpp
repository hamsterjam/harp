#include <globals.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <cstdlib>
#include <iostream>

#include <ECS.h>
#include <Console.h>
#include <harpMath.h>
#include <luaInterface.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/TextureAtlas.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/VisualSpec.h>

static const char* defVertSrc = "";
static const char* defFragSrc = "";

static const char* primVertSrc = "";
static const char* primFragSrc = "";

void readConfig() {
    luaL_openlibs(L);

    if (luaL_loadfile(L, "config.lua") || lua_pcall(L, 0, 0, 0)) {
        std::cerr << "Failed to load config.lua" << std::endl;
        exit(1);
    }

    SCREEN_WIDTH  = getGlobalInt(L, "screenWidth");
    SCREEN_HEIGHT = getGlobalInt(L, "screenHeight");

    defVertSrc = getTableString(L, "defaultShader", "vertSrc");
    defFragSrc = getTableString(L, "defaultShader", "fragSrc");

    primVertSrc = getTableString(L, "primShader", "vertSrc");
    primFragSrc = getTableString(L, "primShader", "fragSrc");

    // Adding in the harp functions after loading the config ensures that the
    // config file can't do anything to the state of the actual program
    openHarp(L);
}

void initGlobals() {
    defaultShader = new Shader(defVertSrc, defFragSrc);
    defaultPrimitiveShader = new Shader(primVertSrc, primFragSrc);

    // Console
    defaultPrim = new PrimitiveRenderer();
    TextureAtlas consoleFontAtlas("res/testfont.png", 8, 12, 0, 0);
    consoleFont = new FontRenderer(consoleFontAtlas, ' ', '~');

    Console::init(*defaultPrim, *consoleFont);
}

void cleanupGlobals() {
    Console::cleanup();

    lua_close(L);

    delete defaultPrim;
    delete consoleFont;

    delete defaultShader;
    delete defaultPrimitiveShader;
}

//
// Default Values
//

lua_State* L = luaL_newstate();

ECS harp(32, 8, 128);

Component comp_position     = harp.createComponentType(sizeof(Vec<2, double>));
Component comp_velocity     = harp.createComponentType(sizeof(Vec<2, double>));
Component comp_acceleration = harp.createComponentType(sizeof(Vec<2, double>));
Component flag_hidden       = harp.createFlagType();
Component comp_visual       = harp.createComponentType(sizeof(VisualSpec));

Shader* defaultShader = 0;
Shader* defaultPrimitiveShader = 0;

PrimitiveRenderer* defaultPrim = 0;
FontRenderer*      consoleFont = 0;

int SCREEN_WIDTH  = 0;
int SCREEN_HEIGHT = 0;

bool shouldExit = false;
