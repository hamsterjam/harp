/*
 * HARP
 * luaInterface.h
 *
 * This is 2 things, some boilerplate to more easily access global Lua
 * variables, and the place where the Lua functions that interface with the C++
 * code are defined. See luaFunctions.md to see the functions that are defined
 * (or rather, will be defined)
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
#ifndef HARP_LUA_INTERFACE_H
#define HARP_LUA_INTERFACE_H

#include <ECS.h>

extern "C" {
#include <lua.h>
}

struct LuaEntityWrapper {
    Entity e;
    bool shouldGC;
};

void luaopen_harp(lua_State* L);

int  weakLuaRef(lua_State* L);
void getWeakLuaRef(lua_State* L, int key);

void lua_pushEntity(lua_State* L, Entity ent);

int         getGlobalInt   (lua_State* L, const char* global);
double      getGlobalFloat (lua_State* L, const char* global);
bool        getGlobalBool  (lua_State* L, const char* global);
const char* getGlobalString(lua_State* L, const char* global);

int         getTableInt   (lua_State* L, const char* table, const char* key);
double      getTableFloat (lua_State* L, const char* table, const char* key);
bool        getTableBool  (lua_State* L, const char* table, const char* key);
const char* getTableString(lua_State* L, const char* table, const char* key);

#endif
