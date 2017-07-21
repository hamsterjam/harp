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

#include <graphics/Shader.h>
#include <graphics/Texture.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Sprite.h>
#include <graphics/VisualSpec.h>

//
// Harp Lua Functions
//

static int l_createEntity(lua_State* L) {
    Entity& ent = * (Entity*) lua_newuserdata(L, sizeof(Entity));
    luaL_getmetatable(L, "harp.entity");
    lua_setmetatable(L, -2);

    ent = harp.createEntity();

    return 1;
}

static int l_deleteEntity(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    harp.deleteEntity(ent);

    return 0;
}

static int l_setComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.component");
    Component& comp = * (Component*) lua_touserdata(L, 2);

    luaL_checkudata(L, 3, "harp.blob");
    void* data = lua_touserdata(L, 3);

    harp.setComponent(ent, comp, data);

    return 0;
}

static int l_setFlag(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.flag");
    Component& comp = * (Component*) lua_touserdata(L, 2);

    luaL_checktype(L, 3, LUA_TBOOLEAN);
    bool val = lua_toboolean(L, 3);

    harp.setFlag(ent, comp, val);

    return 0;
}

static int l_setParent(lua_State* L) {
    Entity ent = luaL_checkinteger(L, 1);
    Entity par = luaL_checkinteger(L, 2);

    harp.setParent(ent, par);

    return 0;
}

static const char* checkFieldString(const char* name, int index);
static int checkFieldInteger(const char* name, int index);

static int l_Sprite(lua_State* L) {

    Sprite* spr = new Sprite();

    luaL_checktype(L, 1, LUA_TTABLE);
    int n = luaL_len(L, 1);

    // Populate the Sprite
    for (int i=1; i <= n; ++i) {
        const char* filename;
        const char* texUniform = 0;
        const char* UVAttrib = 0;

        lua_geti(L, 1, i);
        luaL_checktype(L, -1, LUA_TTABLE);

        // Get the strings that specify our image
        filename   = checkFieldString("filename", i);
        texUniform = checkFieldString("uniform",  i);
        UVAttrib   = checkFieldString("UVAttrib", i);

        if (!filename) luaL_error(L, "No filename in member #%d", i);

        // Test if we are using a texture atlas or not
        lua_pushstring(L, "tileW");
        lua_rawget(L, -2);
        bool usesAtlas = !lua_isnoneornil(L, -1);
        lua_pop(L, 1);
        if (usesAtlas) {
            // Use a texture atlas!
            int tileW = checkFieldInteger("tileW", i);
            int tileH = checkFieldInteger("tileH", i);
            int tileX = checkFieldInteger("tileX", i);
            int tileY = checkFieldInteger("tileY", i);

            TextureAtlas atlas(filename, tileW, tileH, 0, 0);

            if (texUniform && UVAttrib) {
                atlas.addTextureToSprite(*spr, texUniform, UVAttrib, tileX, tileY);
            }
            else {
                atlas.addTextureToSprite(*spr, tileX, tileY);
            }

            // Make sure you pop the table
            lua_pop(L, 1);
            continue;
        }

        // Check if we have custom UV coordinates
        lua_pushstring(L, "x");
        lua_rawget(L, -2);
        bool usesUV = !lua_isnoneornil(L, -1);
        lua_pop(L, 1);

        if (usesUV) {
            // Grab the custom UV coordinates
            unsigned int x = checkFieldInteger("x", i);
            unsigned int y = checkFieldInteger("y", i);
            int w = checkFieldInteger("w", i);
            int h = checkFieldInteger("h", i);

            if (texUniform && UVAttrib) {
                spr->addSubImage(filename, texUniform, UVAttrib, x, y, w, h);
            }
            else {
                spr->addSubImage(filename, x, y, w, h);
            }

            // Again, remember to pop the table from the stack
            lua_pop(L, 1);
            continue;
        }

        // Otherwise we just want to put the whole image on
        if (texUniform && UVAttrib) {
            spr->addImage(filename, texUniform, UVAttrib);
        }
        else {
            spr->addImage(filename);
        }

        lua_pop(L, -1);
    }

    // The userdata stores a pointer to the sprite so we can
    // properly call destructors when we need to (so we dont leak memory)
    auto ret = (Sprite**) lua_newuserdata(L, sizeof(Sprite*));
    luaL_getmetatable(L, "harp.sprite");
    lua_setmetatable(L, -2);
    *ret = spr;

    return 1;
}

static int l_SpriteVisualSpec(lua_State* L) {
    Sprite* spr;
    Shader* shd;
    int dx, dy;

    int numArgs = lua_gettop(L);
    if (numArgs == 3) {
        luaL_checkudata(L, 1, "harp.sprite");
        spr = * (Sprite**) lua_touserdata(L, 1);
        dx = luaL_checkinteger(L, 2);
        dy = luaL_checkinteger(L, 3);

        shd = defaultShader;
    }
    else if (numArgs == 4) {
        luaL_checkudata(L, 1, "harp.shader");
        shd = * (Shader**) lua_touserdata(L, 1);
        luaL_checkudata(L, 2, "harp.sprite");
        spr = * (Sprite**) lua_touserdata(L, 2);
        dx = luaL_checkinteger(L, 3);
        dy = luaL_checkinteger(L, 4);
    }
    else {
        luaL_error(L, "SpriteVisualSpec takes either 3 or 4 arguments, found %d", numArgs);
    }

    VisualSpec* ret = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *ret = getSpriteSpec(*shd, *spr, dx, dy);

    return 1;
}

static int l_Vec2Double(lua_State* L) {
    double val1 = luaL_checknumber(L, 1);
    double val2 = luaL_checknumber(L, 2);
    auto ret = (Vec<2, double>*) lua_newuserdata(L, sizeof(Vec<2, double>));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->data[0] = val1;
    ret->data[1] = val2;
    return 1;
}

//
// Auxiliary
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
// Destructors
//

static int l_callSpriteDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.sprite");
    auto spr = (Sprite**) lua_touserdata(L, 1);
    delete *spr;

    return 0;
}

static int l_callShaderDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.shader");
    auto shd = (Shader**) lua_touserdata(L, 1);
    delete *shd;

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

static const char* checkFieldString(const char* name, int index) {
    lua_getfield(L, -1, name);
    if (lua_isnil(L, -1)) {
        lua_pop(L, 1);
        return 0;
    }
    if (!lua_isstring(L, -1)) {
        luaL_error(L, "In member #%d, %s is a %s, expected a string", index, name, lua_typename(L, -1));
    }
    const char* ret  = lua_tostring(L, -1);
    lua_pop(L, 1);
    return ret;
}

static int checkFieldInteger(const char* name, int index) {
    lua_getfield(L, -1, name);
    if (!lua_isinteger(L, -1)) {
        luaL_error(L, "In member #%d, %s is a %s, expected a integer", index, name, lua_typename(L, -1));
    }
    int ret  = lua_tointeger(L, -1);
    lua_pop(L, 1);
    return ret;
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
    luaL_newmetatable(L, "harp.shader");

    luaL_getmetatable(L, "harp.entity");
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_deleteEntity);
    lua_settable(L, -3);

    luaL_getmetatable(L, "harp.sprite");
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_callSpriteDestructor);
    lua_settable(L, -3);

    luaL_getmetatable(L, "harp.shader");
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_callShaderDestructor);
    lua_settable(L, -3);

    lua_pop(L, 3);

    // Functions

    lua_pushcfunction(L, l_createEntity); lua_setglobal(L, "createEntity");
    lua_pushcfunction(L, l_deleteEntity); lua_setglobal(L, "deleteEntity");
    lua_pushcfunction(L, l_setComponent); lua_setglobal(L, "setComponent");
    lua_pushcfunction(L, l_setFlag);      lua_setglobal(L, "setFlag");
    lua_pushcfunction(L, l_setParent);    lua_setglobal(L, "setParent");

    lua_pushcfunction(L, l_Sprite);     lua_setglobal(L, "Sprite");
    lua_pushcfunction(L, l_Vec2Double); lua_setglobal(L, "Vec2Double");

    lua_pushcfunction(L, l_SpriteVisualSpec); lua_setglobal(L, "SpriteVisualSpec");

    lua_pushcfunction(L, l_print);  lua_setglobal(L, "print");
    lua_pushcfunction(L, l_exit);   lua_setglobal(L, "exit");

    // Components

    lua_newtable(L);
    lua_setglobal(L, "comp");

    setComponentGlobal(L, "position",     comp_position);
    setComponentGlobal(L, "velocity",     comp_velocity);
    setComponentGlobal(L, "acceleration", comp_acceleration);
    setComponentGlobal(L, "visual",       comp_visual);

    lua_newtable(L);
    lua_setglobal(L, "flag");

    setFlagGlobal(L, "hidden", flag_hidden);
    setFlagGlobal(L, "frozen", flag_frozen);
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
