#include <luaInterface.h>

#include <cstdlib>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <globals.h>
#include <harpMath.h>
#include <Console.h>

//
// Harp Lua Functions
//

static int l_createEntity(lua_State* L) {
    Entity ent = harp.createEntity();
    lua_pushinteger(L, ent);
    return 1;
}

static int l_setComponent(lua_State* L) {
    Entity ent = luaL_checkinteger(L, 1);
    Component comp = luaL_checkinteger(L, 2); //TODO// Change this to accept a string instead
    luaL_checkudata(L, 3, "harp.blob");
    void* data = lua_touserdata(L, 3);

    harp.setComponent(ent, comp, data);

    return 0;
}

static int l_setParent(lua_State* L) {
    Entity ent = luaL_checkinteger(L, 1);
    Entity par = luaL_checkinteger(L, 2);

    harp.setParent(ent, par);

    return 0;
}

static int l_getVec2Double(lua_State* L) {
    double val1 = luaL_checknumber(L, 1);
    double val2 = luaL_checknumber(L, 2);
    auto ret = (Vec<2, double>*) lua_newuserdata(L, sizeof(Vec<2, double>));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->data[0] = val1;
    ret->data[1] = val2;
    return 1;
}

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

void luaopen_harp(lua_State* L) {
    luaL_newmetatable(L, "harp.blob");

    lua_pushcfunction(L, l_createEntity); lua_setglobal(L, "createEntity");
    lua_pushcfunction(L, l_setComponent); lua_setglobal(L, "setComponent");
    lua_pushcfunction(L, l_setParent);    lua_setglobal(L, "setParent");

    lua_pushcfunction(L, l_getVec2Double); lua_setglobal(L, "getVec2Double");

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
