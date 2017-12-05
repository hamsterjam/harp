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
#include <FunctionWrapper.h>
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
    luaopen_harp(L);
}

void initGlobals() {
    defaultShader = new Shader(defVertSrc, defFragSrc);
    defaultPrimitiveShader = new Shader(primVertSrc, primFragSrc);

    // Console
    defaultPrim = new PrimitiveRenderer();
    TextureAtlas consoleFontAtlas("res/testfont.png", 8, 12, 0, 0);
    consoleFont = new FontRenderer(consoleFontAtlas, ' ', '~');

    Console::init(*defaultPrim, *consoleFont);

    // Call the Lua init function
    lua_getglobal(L, "init");
    if (lua_pcall(L, 0, 0, 0)) {
        const char* errorMessage = lua_tostring(L, -1);
        std::cerr << errorMessage << std::endl;
        lua_pop(L, 1);
    }
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

Component comp_position       = harp.createComponentType(sizeof(Vec<2, double>));
Component comp_velocity       = harp.createComponentType(sizeof(Vec<2, double>));
Component comp_acceleration   = harp.createComponentType(sizeof(Vec<2, double>));
Component comp_visual         = harp.createComponentType(sizeof(VisualSpec));
Component comp_layer          = harp.createComponentType(sizeof(int));
Component comp_inputFunction  = harp.createComponentType(sizeof(FunctionWrapper));

Component flag_hidden = harp.createFlagType();
Component flag_static = harp.createFlagType();

Component meta_luaRef = harp.createComponentType(sizeof(int));

Shader* defaultShader = 0;
Shader* defaultPrimitiveShader = 0;

PrimitiveRenderer* defaultPrim = 0;
FontRenderer*      consoleFont = 0;

int SCREEN_WIDTH  = 0;
int SCREEN_HEIGHT = 0;

bool shouldExit = false;
