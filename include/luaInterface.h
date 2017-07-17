/*
 * HARP
 * LuaInterface.h
 *
 * This is 2 things, some boilerplate to more easily access global Lua
 * variables, and the place where the Lua functions that interface with the C++
 * code are defined, defined functions are:
 *
 * print(message)
 *      logs the string message to the Console
 *
 * exit()
 *      exits the program (I might make this only accesible from the console)
 *      this does a "soft" exit by setting shouldExit to true.
 *
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
#ifndef HARP_LUA_INTERFACE_H
#define HARP_LUA_INTERFACE_H

extern "C" {
#include <lua.h>
}

void openHarp(lua_State* L);

int         getGlobalInt   (lua_State* L, const char* global);
double      getGlobalFloat (lua_State* L, const char* global);
bool        getGlobalBool  (lua_State* L, const char* global);
const char* getGlobalString(lua_State* L, const char* global);

int         getTableInt   (lua_State* L, const char* table, const char* key);
double      getTableFloat (lua_State* L, const char* table, const char* key);
bool        getTableBool  (lua_State* L, const char* table, const char* key);
const char* getTableString(lua_State* L, const char* table, const char* key);

#endif
