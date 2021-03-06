#ifndef HARP_LUA_FUNCTIONS_H
#define HARP_LUA_FUNCTIONS_H

extern "C" {
#include <lua.h>
}

// Entity operations
int l_createEntity(lua_State* L);
int l_deleteEntity(lua_State* L);
int l_setParent(lua_State* L);
int l_removeParent(lua_State* L);

// Component operations
int l_setComponent(lua_State* L);
int l_getComponent(lua_State* L);
int l_hasComponent(lua_State* L);
int l_removeComponent(lua_State* L);
int l_setFlag(lua_State* L);
int l_getFlag(lua_State* L);

// Conversion (Lua -> C)
int l_Vec2(lua_State* L);
int l_Mat3(lua_State* L);
int l_LineCollider(lua_State* L);
int l_BoxCollider(lua_State* L);
int l_Sprite(lua_State* L);
int l_AnimationManager(lua_State* L);
int l_Shader(lua_State* L);
int l_FontRenderer(lua_State* L);

// Conversion (C -> Lua)
int l_asNumber(lua_State* L);
int l_asInteger(lua_State* L);
int l_asFunction(lua_State* L);
int l_asVec2(lua_State* L);
int l_asMat3(lua_State* L);

// Visual Specs
int l_SpriteSpec(lua_State* L);
int l_AnimationSpec(lua_State* L);
int l_RectSpec(lua_State* L);
int l_RoundedRectSpec(lua_State* L);
int l_ElipseArcSpec(lua_State* L);
int l_TriangleSpec(lua_State* L);
int l_LineSpec(lua_State* L);
int l_GlyphSpec(lua_State* L);
int l_TextSpec(lua_State* L);

// Auxiliary
int l_printRaw(lua_State* L);
int l_exit(lua_State* L);

// Destructors
int l_callSpriteDestructor(lua_State* L);
int l_callAnimationManagerDestructor(lua_State* L);
int l_callShaderDestructor(lua_State* L);
int l_callPrimitiveRendererDestructor(lua_State* L);
int l_callFontRendererDestructor(lua_State* L);

#endif
