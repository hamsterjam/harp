#include <luaInterface.h>

#include <cstdlib>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <globals.h>
#include <Console.h>

//
// Harp Lua Functions
//

static int l_print(lua_State* L) {
    const char* message = luaL_checkstring(L, 1);
    Console::getInstance().log("<- " + std::string(message));
    return 0;
}

static int l_exit(lua_State* L) {
    shouldExit = true;
    return 0;
}

//
// Helper Functions
//

static void readyTable(lua_State* L, const char* table) {
    lua_getglobal(L, table);
    if (!lua_istable(L, -1)) {
        std::cerr << "\"" << table << "\" is not a table." << std::endl;
        std::exit(1);
    }
}

//
// Exported Functions
//

void openHarp(lua_State* L) {
    lua_pushcfunction(L, l_print);  lua_setglobal(L, "print");
    lua_pushcfunction(L, l_exit);   lua_setglobal(L, "exit");
}

int getGlobalInt(lua_State* L, const char* global) {
    lua_getglobal(L, global);
    if (!lua_isinteger(L, -1)) {
        std::cerr << "\"" << global << "\" is not an integer." << std::endl;
        std::exit(1);
    }
    int ret = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return ret;
}

double getGlobalFloat(lua_State* L, const char* global) {
    lua_getglobal(L, global);
    if (!lua_isnumber(L, -1)) {
        std::cerr << "\"" << global << "\" is not a number." << std::endl;
        std::exit(1);
    }
    double ret = lua_tonumber(L, -1);
    lua_pop(L, 1);
    return ret;
}

bool getGlobalBool(lua_State* L, const char* global) {
    lua_getglobal(L, global);
    if (!lua_isboolean(L, -1)) {
        std::cerr << "\"" << global << "\" is not a boolean." << std::endl;
        std::exit(1);
    }
    bool ret = lua_toboolean(L, -1);
    lua_pop(L, 1);
    return ret;
}

const char* getGlobalString(lua_State* L, const char* global) {
    lua_getglobal(L, global);
    if (!lua_isstring(L, -1)) {
        std::cerr << "\"" << global << "\" is not a string." << std::endl;
        std::exit(1);
    }
    const char* ret = lua_tostring(L, -1);
    lua_pop(L, 1);
    return ret;
}


int getTableInt(lua_State* L, const char* table, const char* key) {
    readyTable(L, table);

    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isinteger(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not an integer." << std::endl;
        std::exit(1);
    }
    int ret = lua_tointeger(L, -1);
    lua_pop(L, 2);
    return ret;
}

double getTableFloat(lua_State* L, const char* table, const char* key) {
    readyTable(L, table);

    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isnumber(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not a number." << std::endl;
        std::exit(1);
    }
    float ret = lua_tonumber(L, -1);
    lua_pop(L, 2);
    return ret;
}

bool getTableBool(lua_State* L, const char* table, const char* key) {
    readyTable(L, table);

    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isboolean(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not an boolean." << std::endl;
        std::exit(1);
    }
    bool ret = lua_toboolean(L, -1);
    lua_pop(L, 2);
    return ret;
}

const char* getTableString(lua_State* L, const char* table, const char* key) {
    readyTable(L, table);

    lua_pushstring(L, key);
    lua_gettable(L, -2);
    if (!lua_isstring(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not an string." << std::endl;
        std::exit(1);
    }
    const char* ret = lua_tostring(L, -1);
    lua_pop(L, 2);
    return ret;
}
