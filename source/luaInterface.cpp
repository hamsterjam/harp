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
        lua_getfield(L, -1, "filename");
        filename = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "uniform");
        if (!lua_isnil(L, -1)) texUniform = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "UVAttrib");
        if (!lua_isnil(L, -1)) UVAttrib = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        // Test if we are using a texture atlas or not
        lua_pushstring(L, "tileW");
        lua_rawget(L, -2);
        bool usesAtlas = !lua_isnoneornil(L, -1);
        lua_pop(L, 1);
        if (usesAtlas) {
            // Use a texture atlas!
            int tileW, tileH;
            int tileX, tileY;

            lua_getfield(L, -1, "tileW");
            tileW = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "tileH");
            tileH = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "tileX");
            tileX = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "tileY");
            tileY = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

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
            unsigned int x, y;
            int w, h;

            lua_getfield(L, -1, "x");
            x = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "y");
            y = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "w");
            w = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_getfield(L, -1, "h");
            h = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

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

static int l_getSpriteSpecDef(lua_State* L) {
    luaL_checkudata(L, 1, "harp.sprite");
    Sprite& spr = ** (Sprite**) lua_touserdata(L, 1);
    int dx = luaL_checkinteger(L, 2);
    int dy = luaL_checkinteger(L, 3);

    VisualSpec* ret = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *ret = getSpriteSpec(*defaultShader, spr, dx, dy);

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

//
// Exported Functions
//

void luaopen_harp(lua_State* L) {
    luaL_newmetatable(L, "harp.blob");
    luaL_newmetatable(L, "harp.entity");
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

    lua_pushcfunction(L, l_createEntity); lua_setglobal(L, "createEntity");
    lua_pushcfunction(L, l_deleteEntity); lua_setglobal(L, "deleteEntity");
    lua_pushcfunction(L, l_setComponent); lua_setglobal(L, "setComponent");
    lua_pushcfunction(L, l_setParent);    lua_setglobal(L, "setParent");

    lua_pushcfunction(L, l_Sprite);     lua_setglobal(L, "Sprite");
    lua_pushcfunction(L, l_Vec2Double); lua_setglobal(L, "Vec2Double");

    lua_pushcfunction(L, l_getSpriteSpecDef); lua_setglobal(L, "getSpriteSpecDef");

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
