#include <globals.h>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <iostream>

#include <ECS.h>
#include <Console.h>
#include <harpMath.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/TextureAtlas.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/VisualSpec.h>

// Lua functions

static int l_print(lua_State* L) {
    std::cout << "Hello!" << std::endl;
    const char* message = luaL_checkstring(L, 1);
    Console::getInstance().log(std::string(message));
    return 0;
}

static int l_exit(lua_State* L) {
    shouldExit = true;
    return 0;
}

void initGlobals() {

    // Lua

    L = luaL_newstate();

    // modules
    luaopen_base(L);
    luaopen_coroutine(L);
    luaopen_table(L);
    luaopen_string(L);
    luaopen_math(L);

    // functions
    lua_pushcfunction(L, l_print);  lua_setglobal(L, "print");
    lua_pushcfunction(L, l_exit);   lua_setglobal(L, "exit");

    // ECS

    harp = new ECS(32, 8, 128);

    comp_position     = harp->createComponentType(sizeof(Vec<2, double>));
    comp_velocity     = harp->createComponentType(sizeof(Vec<2, double>));
    comp_acceleration = harp->createComponentType(sizeof(Vec<2, double>));
    flag_hidden       = harp->createFlagType();
    comp_visual       = harp->createComponentType(sizeof(VisualSpec));

    // Shaders

    defaultShader = new Shader();
    defaultPrimitiveShader = new Shader(defaultPrimitiveVertSource, defaultPrimitiveFragSource);

    // Console

    defaultPrim = new PrimitiveRenderer();
    TextureAtlas consoleFontAtlas("res/testfont.png", 8, 12, 0, 0);
    consoleFont = new FontRenderer(consoleFontAtlas, ' ', '~');

    Console::init(*defaultPrim, *consoleFont);
    console = &Console::getInstance();
}

void cleanupGlobals() {
    Console::cleanup();

    lua_close(L);

    delete harp;

    delete defaultPrim;
    delete consoleFont;

    delete defaultShader;
    delete defaultPrimitiveShader;
}

lua_State* L = 0;

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

int SCREEN_WIDTH  = 640;
int SCREEN_HEIGHT = 480;

bool shouldExit = false;
