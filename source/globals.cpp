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

static const char* defVertSrc = "";
static const char* defFragSrc = "";

static const char* primVertSrc = "";
static const char* primFragSrc = "";

// Lua functions

static int l_print(lua_State* L) {
    const char* message = luaL_checkstring(L, 1);
    Console::getInstance().log("<- " + std::string(message));
    return 0;
}

static int l_exit(lua_State* L) {
    shouldExit = true;
    return 0;
}

// Lua helper functions

static void dumpStack() {
    int i;
    int top = lua_gettop(L);
    for (i = 0; i <= top; ++i) {
        int t = lua_type(L, i);
        switch(t) {
            case LUA_TSTRING:
                std::cout << "\"" << lua_tostring(L, i) << "\"";
                break;

            case LUA_TBOOLEAN:
                std::cout << ((lua_toboolean(L, i)) ? "true" : "false");
                break;

            case LUA_TNUMBER:
                std::cout << lua_tonumber(L, i);
                break;

            default:
                std::cout << lua_typename(L, i);
                break;
        }
        std::cout << " : ";
    }
    std::cout << std::endl;
}

static int getGlobalInt(const char* global) {
    lua_getglobal(L, global);
    if (!lua_isinteger(L, -1)) {
        std::cerr << "\"" << global << "\" is not an integer." << std::endl;
        shouldExit = true;
        return 0;
    }
    int ret = lua_tointeger(L, -1);
    lua_pop(L, -1);
    return ret;
}

static const char * getTableString(const char* table, const char* key) {
    lua_getglobal(L, table);
    if (!lua_istable(L, -1)) {
        std::cerr << "\"" << table << "\" is not a table." << std::endl;
        shouldExit = true;
        return 0;
    }
    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isstring(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not a string." << std::endl;
        shouldExit = true;
        return 0;
    }
    const char* ret = lua_tostring(L, -1);
    lua_pop(L, 2);
    return ret;
}

// This is seperate because we need it BEFORE all SDL stuff is up
void initLua() {
    L = luaL_newstate();

    // modules
    luaL_openlibs(L);

    // functions
    lua_pushcfunction(L, l_print);  lua_setglobal(L, "print");
    lua_pushcfunction(L, l_exit);   lua_setglobal(L, "exit");

    // read config.lua
    if (luaL_loadfile(L, "config.lua") || lua_pcall(L, 0, 0, 0) ) {
        std::cerr << "Failed to load config.lua" << std::endl;
        shouldExit = true;
        return;
    }

    SCREEN_WIDTH  = getGlobalInt("screenWidth");
    SCREEN_HEIGHT = getGlobalInt("screenHeight");

    defVertSrc = getTableString("defaultShader", "vertSrc");
    defFragSrc = getTableString("defaultShader", "fragSrc");

    primVertSrc = getTableString("primShader", "vertSrc");
    primFragSrc = getTableString("primShader", "fragSrc");
}

void initGlobals() {
    defaultShader = new Shader(defVertSrc, defFragSrc);
    defaultPrimitiveShader = new Shader(primVertSrc, primFragSrc);

    // Console
    defaultPrim = new PrimitiveRenderer();
    TextureAtlas consoleFontAtlas("res/testfont.png", 8, 12, 0, 0);
    consoleFont = new FontRenderer(consoleFontAtlas, ' ', '~');

    Console::init(*defaultPrim, *consoleFont);
    console = &Console::getInstance();
}

void initECS() {
    harp = new ECS(32, 8, 128);

    comp_position     = harp->createComponentType(sizeof(Vec<2, double>));
    comp_velocity     = harp->createComponentType(sizeof(Vec<2, double>));
    comp_acceleration = harp->createComponentType(sizeof(Vec<2, double>));
    flag_hidden       = harp->createFlagType();
    comp_visual       = harp->createComponentType(sizeof(VisualSpec));

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

//
// Default Values
//

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

int SCREEN_WIDTH  = 0;
int SCREEN_HEIGHT = 0;

bool shouldExit = false;
