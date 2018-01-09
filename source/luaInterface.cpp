#include <luaInterface.h>

#include <cstdlib>
#include <iostream>

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <globals.h>
#include <Action.h>
#include <luaFunctions.h>

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

static void setComponentGlobal(lua_State* L, const char* name, Component comp) {
    lua_getglobal(L, "comp");
    lua_pushstring(L, name);

    Component& ret = * (Component*) lua_newuserdata(L, sizeof(Component));
    luaL_getmetatable(L, "harp.component");
    lua_setmetatable(L, -2);

    ret = comp;

    lua_settable(L, -3);
    lua_pop(L, 1);
}

static void setFlagGlobal(lua_State* L, const char* name, Component flag) {
    lua_getglobal(L, "flag");
    lua_pushstring(L, name);

    Component& ret = * (Component*) lua_newuserdata(L, sizeof(Component));
    luaL_getmetatable(L, "harp.flag");
    lua_setmetatable(L, -2);

    ret = flag;

    lua_settable(L, -3);
    lua_pop(L, 1);
}

static void setActionGlobal(lua_State* L, const char* name, Action action) {
    lua_getglobal(L, "action");
    lua_pushstring(L, name);

    Action& ret = * (Action *) lua_newuserdata(L, sizeof(Action));
    luaL_getmetatable(L, "harp.action");
    lua_setmetatable(L, -2);

    ret = action;

    lua_settable(L, -3);
    lua_pop(L, 1);
}

//
// Exported Functions
//

void luaopen_harp(lua_State* L) {
    // Types

    luaL_newmetatable(L, "harp.blob");
    luaL_newmetatable(L, "harp.entity");
    luaL_newmetatable(L, "harp.component");
    luaL_newmetatable(L, "harp.flag");
    luaL_newmetatable(L, "harp.sprite");
    luaL_newmetatable(L, "harp.animationmanager");
    luaL_newmetatable(L, "harp.shader");
    luaL_newmetatable(L, "harp.primitiverenderer");
    luaL_newmetatable(L, "harp.fontrenderer");
    luaL_newmetatable(L, "harp.action");

    luaL_getmetatable(L, "harp.entity");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index"); // What does this even accomplish?
    lua_pushcfunction(L, l_deleteEntity);    lua_setfield(L, -2, "__gc");
    lua_pushcfunction(L, l_setComponent);    lua_setfield(L, -2, "set");
    lua_pushcfunction(L, l_getComponent);    lua_setfield(L, -2, "get");
    lua_pushcfunction(L, l_hasComponent);    lua_setfield(L, -2, "has");
    lua_pushcfunction(L, l_removeComponent); lua_setfield(L, -2, "remove");
    lua_pushcfunction(L, l_setFlag);         lua_setfield(L, -2, "setFlag");
    lua_pushcfunction(L, l_getFlag);         lua_setfield(L, -2, "getFlag");
    lua_pushcfunction(L, l_setParent);       lua_setfield(L, -2, "setParent");
    lua_pushcfunction(L, l_removeParent);    lua_setfield(L, -2, "removeParent");

    luaL_getmetatable(L, "harp.sprite");
    lua_pushcfunction(L, l_callSpriteDestructor); lua_setfield(L, -2, "__gc");

    luaL_getmetatable(L, "harp.animationmanager");
    lua_pushcfunction(L, l_callAnimationManagerDestructor); lua_setfield(L, -2, "__gc");

    luaL_getmetatable(L, "harp.shader");
    lua_pushcfunction(L, l_callShaderDestructor); lua_setfield(L, -2, "__gc");

    luaL_getmetatable(L, "harp.primitiverenderer");
    lua_pushcfunction(L, l_callPrimitiveRendererDestructor); lua_setfield(L, -2, "__gc");

    luaL_getmetatable(L, "harp.fontrenderer");
    lua_pushcfunction(L, l_callFontRendererDestructor); lua_setfield(L, -2, "__gc");

    lua_pop(L, 6);

    // Functions

    lua_pushcfunction(L, l_createEntity);       lua_setglobal(L, "createEntity");
    lua_pushcfunction(L, l_deleteEntity);       lua_setglobal(L, "deleteEntity");
    lua_pushcfunction(L, l_setParent);          lua_setglobal(L, "setParent");
    lua_pushcfunction(L, l_removeParent);       lua_setglobal(L, "removeParent");

    lua_pushcfunction(L, l_setComponent);       lua_setglobal(L, "setComponent");
    lua_pushcfunction(L, l_getComponent);       lua_setglobal(L, "getComponent");
    lua_pushcfunction(L, l_hasComponent);       lua_setglobal(L, "hasComponent");
    lua_pushcfunction(L, l_removeComponent);    lua_setglobal(L, "removeComponent");
    lua_pushcfunction(L, l_setFlag);            lua_setglobal(L, "setFlag");
    lua_pushcfunction(L, l_getFlag);            lua_setglobal(L, "getFlag");

    lua_pushcfunction(L, l_Vec2);               lua_setglobal(L, "Vec2");
    lua_pushcfunction(L, l_Mat3);               lua_setglobal(L, "Mat3");
    lua_pushcfunction(L, l_LineCollider);       lua_setglobal(L, "LineCollider");
    lua_pushcfunction(L, l_BoxCollider);        lua_setglobal(L, "BoxCollider");
    lua_pushcfunction(L, l_Sprite);             lua_setglobal(L, "Sprite");
    lua_pushcfunction(L, l_AnimationManager);   lua_setglobal(L, "AnimationManager");
    lua_pushcfunction(L, l_Shader);             lua_setglobal(L, "Shader");
    lua_pushcfunction(L, l_FontRenderer);       lua_setglobal(L, "FontRenderer");

    lua_pushcfunction(L, l_asNumber);           lua_setglobal(L, "asNumber");
    lua_pushcfunction(L, l_asInteger);          lua_setglobal(L, "asInteger");
    lua_pushcfunction(L, l_asFunction);         lua_setglobal(L, "asFunction");
    lua_pushcfunction(L, l_asVec2);             lua_setglobal(L, "asVec2");
    lua_pushcfunction(L, l_asMat3);             lua_setglobal(L, "asMat3");

    lua_pushcfunction(L, l_SpriteSpec);         lua_setglobal(L, "SpriteSpec");
    lua_pushcfunction(L, l_AnimationSpec);      lua_setglobal(L, "AnimationSpec");
    lua_pushcfunction(L, l_RectSpec);           lua_setglobal(L, "RectSpec");
    lua_pushcfunction(L, l_RoundedRectSpec);    lua_setglobal(L, "RoundedRectSpec");
    lua_pushcfunction(L, l_ElipseArcSpec);      lua_setglobal(L, "ElipseArcSpec");
    lua_pushcfunction(L, l_TriangleSpec);       lua_setglobal(L, "TriangleSpec");
    lua_pushcfunction(L, l_LineSpec);           lua_setglobal(L, "LineSpec");
    lua_pushcfunction(L, l_GlyphSpec);          lua_setglobal(L, "GlyphSpec");
    lua_pushcfunction(L, l_TextSpec);           lua_setglobal(L, "TextSpec");

    lua_pushcfunction(L, l_printRaw);           lua_setglobal(L, "printRaw");
    lua_pushcfunction(L, l_exit);               lua_setglobal(L, "exit");

    // Components

    lua_newtable(L);
    lua_setglobal(L, "comp");

    setComponentGlobal(L, "position",      comp_position);
    setComponentGlobal(L, "velocity",      comp_velocity);
    setComponentGlobal(L, "acceleration",  comp_acceleration);
    setComponentGlobal(L, "collider",      comp_collider);
    setComponentGlobal(L, "visual",        comp_visual);
    setComponentGlobal(L, "layer",         comp_layer);
    setComponentGlobal(L, "inputFunction", comp_inputFunction);

    setComponentGlobal(L, "nextPosition",  comp_nextPosition);
    setComponentGlobal(L, "onSurface",     comp_onSurface);
    setComponentGlobal(L, "partialStep",   comp_partialStep);

    lua_newtable(L);
    lua_setglobal(L, "flag");

    setFlagGlobal(L, "hidden", flag_hidden);
    setFlagGlobal(L, "static", flag_static);

    // Actions

    lua_newtable(L);
    lua_setglobal(L, "action");

    setActionGlobal(L, "noaction",  Action::NO_ACTION);
    setActionGlobal(L, "jump",      Action::JUMP);
    setActionGlobal(L, "walkleft",  Action::WALK_LEFT);
    setActionGlobal(L, "walkright", Action::WALK_RIGHT);
    setActionGlobal(L, "fastdrop",  Action::FAST_DROP);
}

static int weakRefTable;
static bool weakRefTableDefined = false;

int weakLuaRef(lua_State* L) {
    if (!weakRefTableDefined) {
        lua_newtable(L);
        lua_newtable(L); // the metatable
        lua_pushstring(L, "v");
        lua_setfield(L, -2, "__mode");
        lua_setmetatable(L, -2);
        weakRefTable = luaL_ref(L, LUA_REGISTRYINDEX);

        weakRefTableDefined = true;
    }
    lua_pushvalue(L, -1); // luaL_ref will eat the thing if we dont do this
    lua_rawgeti(L, LUA_REGISTRYINDEX, weakRefTable);
    lua_insert(L, -2);

    int ret = luaL_ref(L, -2);
    lua_pop(L, 1);
    return ret;
}

void getWeakLuaRef(lua_State* L, int key) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, weakRefTable);
    lua_rawgeti(L, -1, key);
    lua_remove(L, -2);
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

    lua_getfield(L, -1, key);
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

    lua_getfield(L, -1, key);
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

    lua_getfield(L, -1, key);
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

    lua_getfield(L, -1, key);
    if (!lua_isstring(L, -1)) {
        std::cerr << "\"" << table << "." << key << "\" is not an string." << std::endl;
        std::exit(1);
    }
    const char* ret = lua_tostring(L, -1);
    lua_pop(L, 2);
    return ret;
}
