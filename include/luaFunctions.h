#ifndef HARP_LUA_FUNCTIONS_H
#define HARP_LUA_FUNCTIONS_H

extern "C" {
#include <lua.h>
}

int l_createEntity(lua_State* L);
int l_deleteEntity(lua_State* L);
int l_setComponent(lua_State* L);
int l_setFlag(lua_State* L);
int l_setParent(lua_State* L);

int l_Vec2(lua_State* L);
int l_Sprite(lua_State* L);
int l_Shader(lua_State* L);
int l_FontRenderer(lua_State* L);

int l_SpriteSpec(lua_State* L);
int l_RectSpec(lua_State* L);
int l_RoundedRectSpec(lua_State* L);
int l_ElipseArcSpec(lua_State* L);
int l_TriangleSpec(lua_State* L);
int l_LineSpec(lua_State* L);
int l_GlyphSpec(lua_State* L);
int l_TextSpec(lua_State* L);

int l_print(lua_State* L);
int l_exit(lua_State* L);

int l_callSpriteDestructor(lua_State* L);
int l_callShaderDestructor(lua_State* L);
int l_callPrimitiveRendererDestructor(lua_State* L);
int l_callFontRendererDestructor(lua_State* L);

#endif
