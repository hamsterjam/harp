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

#include <graphics/Texture.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Sprite.h>

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

static int l_newSprite(lua_State* L) {
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
        lua_pushstring(L, "filename");
        lua_gettable(L, -2);
        filename = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "uniform");
        lua_gettable(L, -2);
        if (!lua_isnil(L, -1)) texUniform = luaL_checkstring(L, -1);
        lua_pop(L, 1);

        lua_pushstring(L, "UVAttrib");
        lua_gettable(L, -2);
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

            lua_pushstring(L, "tileW");
            lua_gettable(L, -2);
            tileW = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "tileH");
            lua_gettable(L, -2);
            tileH = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "tileX");
            lua_gettable(L, -2);
            tileX = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "tileY");
            lua_gettable(L, -2);
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

            lua_pushstring(L, "x");
            lua_gettable(L, -2);
            x = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "y");
            lua_gettable(L, -2);
            y = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "w");
            lua_gettable(L, -2);
            w = luaL_checkinteger(L, -1);
            lua_pop(L, 1);

            lua_pushstring(L, "h");
            lua_gettable(L, -2);
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
    luaL_getmetatable(L, "harp.blob.sprite");
    lua_setmetatable(L, -2);
    *ret = spr;

    return 1;
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
// Destructors
//

static int l_callSpriteDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob.sprite");
    auto spr = (Sprite**) lua_touserdata(L, 1);
    delete *spr;

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
    luaL_newmetatable(L, "harp.blob.sprite");

    luaL_getmetatable(L, "harp.blob.sprite");
    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, l_callSpriteDestructor);
    lua_settable(L, -3);

    lua_pushcfunction(L, l_createEntity); lua_setglobal(L, "createEntity");
    lua_pushcfunction(L, l_setComponent); lua_setglobal(L, "setComponent");
    lua_pushcfunction(L, l_setParent);    lua_setglobal(L, "setParent");

    lua_pushcfunction(L, l_newSprite);     lua_setglobal(L, "newSprite");
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
