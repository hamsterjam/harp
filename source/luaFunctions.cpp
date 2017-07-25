#include <luaFunctions.h>

#include <iostream>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <luaInterface.h>

#include <globals.h>
#include <Console.h>
#include <harpMath.h>
#include <FunctionWrapper.h>

#include <graphics/Shader.h>
#include <graphics/Texture.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Sprite.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/VisualSpec.h>
#include <graphics/Color.h>

//
// Helper Functions
//

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

static Color getColor(lua_State* L, int index) {
    lua_getfield(L, index, "r");
    lua_getfield(L, index, "g");
    lua_getfield(L, index, "b");
    lua_getfield(L, index, "a");

    float r = (float) lua_tonumber(L, -4);
    float g = (float) lua_tonumber(L, -3);
    float b = (float) lua_tonumber(L, -2);
    float a = (float) lua_tonumber(L, -1);

    lua_pop(L, 4);

    return rgbaToColor(r, g, b, a);
}

//
// Lua Functions
//

int l_createEntity(lua_State* L) {
    Entity& ent = * (Entity*) lua_newuserdata(L, sizeof(Entity));
    luaL_getmetatable(L, "harp.entity");
    lua_setmetatable(L, -2);

    ent = harp.createEntity();

    return 1;
}

int l_deleteEntity(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    harp.deleteEntity(ent);

    return 0;
}

int l_setComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.component");
    Component& comp = * (Component*) lua_touserdata(L, 2);

    if (lua_isuserdata(L, 3)) {
        luaL_checkudata(L, 3, "harp.blob");
        void* data = lua_touserdata(L, 3);
        harp.setComponent(ent, comp, data);
    }
    else if (lua_isinteger(L, 3)) {
        int data = lua_tointeger(L, 3);
        harp.setComponent(ent, comp, &data);
    }
    else if (lua_isnumber(L, 3)) {
        double data = lua_tonumber(L, 3);
        harp.setComponent(ent, comp, &data);
    }
    else if (lua_isfunction(L, 3)) {
        FunctionWrapper func;
        func.isLua = true;
        func.luaFunc = weakLuaRef(L);

        harp.setComponent(ent, comp, &func);
    }

    return 0;
}

int l_setFlag(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    Entity& ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.flag");
    Component& comp = * (Component*) lua_touserdata(L, 2);

    luaL_checktype(L, 3, LUA_TBOOLEAN);
    bool val = lua_toboolean(L, 3);

    harp.setFlag(ent, comp, val);

    return 0;
}

int l_setParent(lua_State* L) {
    Entity ent = luaL_checkinteger(L, 1);
    Entity par = luaL_checkinteger(L, 2);

    harp.setParent(ent, par);

    return 0;
}

int l_Vec2(lua_State* L) {
    double val1 = luaL_checknumber(L, 1);
    double val2 = luaL_checknumber(L, 2);
    auto ret = (Vec<2, double>*) lua_newuserdata(L, sizeof(Vec<2, double>));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->data[0] = val1;
    ret->data[1] = val2;
    return 1;
}

int l_Sprite(lua_State* L) {

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

int l_Shader(lua_State* L) {
    const char* vertSrc = luaL_checkstring(L, 1);
    const char* fragSrc = luaL_checkstring(L, 2);

    auto ret = (Shader**) lua_newuserdata(L, sizeof(Sprite*));
    luaL_getmetatable(L, "harp.shader");
    lua_setmetatable(L, -2);

    *ret = new Shader(vertSrc, fragSrc);

    return 1;
}

int l_FontRenderer(lua_State* L) {
    int numArgs = lua_gettop(L);

    const char* filename = luaL_checkstring(L, 1);
    int tileW = luaL_checkinteger(L, 2);
    int tileH = luaL_checkinteger(L, 3);

    TextureAtlas fontAtlas(filename, tileW, tileH, 0, 0);
    FontRenderer** font = (FontRenderer**) lua_newuserdata(L, sizeof(FontRenderer));
    luaL_getmetatable(L, "harp.fontrenderer");
    lua_setmetatable(L, -2);

    if (numArgs < 4) {
        luaL_error(L, "FontRenderer takes at least 4 arguments, found %d", numArgs);
    }
    if (numArgs >= 5) {
        char firstChar = luaL_checkstring(L, 4)[0];
        char lastChar  = luaL_checkstring(L, 5)[0];

        *font = new FontRenderer(fontAtlas, firstChar, lastChar);
    }
    else {
        const char* charDef = luaL_checkstring(L, 4);
        *font = new FontRenderer(fontAtlas, charDef);
    }

    return 1;
}

int l_SpriteSpec(lua_State* L) {
    Shader* shd = defaultShader;
    Sprite* spr;
    float dx, dy;

    int numArgs = lua_gettop(L);
    if (numArgs < 3) {
        luaL_error(L, "SpriteSpec takes at least 3 arguments, found %d", numArgs);
    }
    if (numArgs >= 4) {
        luaL_checkudata(L, 4, "harp.shader");
        shd = * (Shader**) lua_touserdata(L, 1);
    }

    luaL_checkudata(L, 1, "harp.sprite");
    spr = * (Sprite**) lua_touserdata(L, 1);
    dx = (float) luaL_checknumber(L, 2);
    dy = (float) luaL_checknumber(L, 3);

    VisualSpec* ret = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *ret = getSpriteSpec(*spr, dx, dy, *shd);

    return 1;
}

int l_RectSpec(lua_State* L) {
    PrimitiveRenderer* prim = defaultPrim;
    float x, y, w, h, lineW;
    Color color;

    int numArgs = lua_gettop(L);
    if (numArgs < 6) {
        luaL_error(L, "RectangleSpec takes at least 6 arguments, found %d.", numArgs);
    }
    if (numArgs == 7) {
        luaL_checkudata(L, 7, "harp.primitiverenderer");
        prim = * (PrimitiveRenderer**) lua_touserdata(L, 7);
    }

    x     = (float) luaL_checknumber(L, 1);
    y     = (float) luaL_checknumber(L, 2);
    w     = (float) luaL_checknumber(L, 3);
    h     = (float) luaL_checknumber(L, 4);
    lineW = (float) luaL_checknumber(L, 5);

    luaL_checktype(L, 6, LUA_TTABLE);
    color = getColor(L, 6);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getRectangleSpec(x, y, w, h, lineW, color, *prim);

    return 1;
}

int l_RoundedRectSpec(lua_State* L) {
    PrimitiveRenderer* prim = defaultPrim;
    float x, y, w, h, r, lineW;
    Color color;

    int numArgs = lua_gettop(L);
    if (numArgs < 7) {
        luaL_error(L, "RoundedRectangleSpec takes at least 7 arguments, found %d.", numArgs);
    }
    if (numArgs == 8) {
        luaL_checkudata(L, 8, "harp.primitiverenderer");
        prim = * (PrimitiveRenderer**) lua_touserdata(L, 8);
    }

    x     = (float) luaL_checknumber(L, 1);
    y     = (float) luaL_checknumber(L, 2);
    w     = (float) luaL_checknumber(L, 3);
    h     = (float) luaL_checknumber(L, 4);
    r     = (float) luaL_checknumber(L, 5);
    lineW = (float) luaL_checknumber(L, 6);

    luaL_checktype(L, 7, LUA_TTABLE);
    color = getColor(L, 7);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getRoundedRectangleSpec(x, y, w, h, r, lineW, color, *prim);

    return 1;
}

int l_ElipseArcSpec(lua_State* L) {
    PrimitiveRenderer* prim = defaultPrim;
    float x, y, rx, ry, theta1, theta2, lineW;
    Color color;

    int numArgs = lua_gettop(L);
    if (numArgs < 8) {
        luaL_error(L, "ElipseArcSpec takes at least 8 arguments, found %d.", numArgs);
    }
    if (numArgs == 9) {
        luaL_checkudata(L, 9, "harp.primitiverenderer");
        prim = * (PrimitiveRenderer**) lua_touserdata(L, 9);

        lua_remove(L, 1);
    }

    x      = (float) luaL_checknumber(L, 1);
    y      = (float) luaL_checknumber(L, 2);
    rx     = (float) luaL_checknumber(L, 3);
    ry     = (float) luaL_checknumber(L, 4);
    theta1 = (float) luaL_checknumber(L, 5);
    theta2 = (float) luaL_checknumber(L, 6);
    lineW  = (float) luaL_checknumber(L, 7);

    luaL_checktype(L, 8, LUA_TTABLE);
    color = getColor(L, 8);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getElipseArcSpec(x, y, rx, ry, theta1, theta2, lineW, color, *prim);

    return 1;
}

int l_TriangleSpec(lua_State* L) {
    PrimitiveRenderer* prim = defaultPrim;
    float x1, y1, x2, y2, x3, y3, lineW;
    Color color;

    int numArgs = lua_gettop(L);
    if (numArgs < 8) {
        luaL_error(L, "TriangleSpec takes at least 8 arguments, found %d.", numArgs);
    }
    if (numArgs == 9) {
        luaL_checkudata(L, 9, "harp.primitiverenderer");
        prim = * (PrimitiveRenderer**) lua_touserdata(L, 9);

        lua_remove(L, 1);
    }

    x1    = (float) luaL_checknumber(L, 1);
    y1    = (float) luaL_checknumber(L, 2);
    x2    = (float) luaL_checknumber(L, 3);
    y2    = (float) luaL_checknumber(L, 4);
    x3    = (float) luaL_checknumber(L, 5);
    y3    = (float) luaL_checknumber(L, 6);
    lineW = (float) luaL_checknumber(L, 7);

    luaL_checktype(L, 8, LUA_TTABLE);
    color = getColor(L, 8);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getTriangleSpec(x1, y1, x2, y2, x3, y3, lineW, color, *prim);

    return 1;
}

int l_LineSpec(lua_State* L) {
    PrimitiveRenderer* prim = defaultPrim;
    float x1, y1, x2, y2, lineW;
    Color color;

    int numArgs = lua_gettop(L);
    if (numArgs < 6) {
        luaL_error(L, "LineSpec takes at least 6 arguments, found %d.", numArgs);
    }
    if (numArgs == 7) {
        luaL_checkudata(L, 7, "harp.primitiverenderer");
        prim = * (PrimitiveRenderer**) lua_touserdata(L, 7);

        lua_remove(L, 1);
    }

    x1    = (float) luaL_checknumber(L, 1);
    y1    = (float) luaL_checknumber(L, 2);
    x2    = (float) luaL_checknumber(L, 3);
    y2    = (float) luaL_checknumber(L, 4);
    lineW = (float) luaL_checknumber(L, 5);

    luaL_checktype(L, 6, LUA_TTABLE);
    color = getColor(L, 6);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getLineSpec(x1, y1, x2, y2, lineW, color, *prim);

    return 1;
}

int l_GlyphSpec(lua_State* L) {
    FontRenderer* font;
    float x, y;
    char glyph;

    glyph = luaL_checkstring(L, 1)[0];
    x = (float) luaL_checknumber(L, 2);
    y = (float) luaL_checknumber(L, 3);
    luaL_checkudata(L, 4, "harp.fontrenderer");
    font = * (FontRenderer**) lua_touserdata(L, 4);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getGlyphSpec(glyph, x, y, *font);

    return 1;
}

int l_TextSpec(lua_State* L) {
    FontRenderer* font;
    float x, y;
    const char* text;

    text = luaL_checkstring(L, 1);
    x = (float) luaL_checknumber(L, 2);
    y = (float) luaL_checknumber(L, 3);
    luaL_checkudata(L, 4, "harp.fontrenderer");
    font = * (FontRenderer**) lua_touserdata(L, 4);

    VisualSpec* spec = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *spec = getTextSpec(text, x, y, *font);

    return 1;
}

//
// Auxiliary
//

int l_print(lua_State* L) {
    const char* message = luaL_checkstring(L, 1);
    Console::getInstance().log("<- " + std::string(message));
    return 0;
}

int l_exit(lua_State* L) {
    shouldExit = true;
    return 0;
}

//
// Destructors
//

int l_callSpriteDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.sprite");
    auto spr = * (Sprite**) lua_touserdata(L, 1);
    delete spr;

    return 0;
}

int l_callShaderDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.shader");
    auto shd = * (Shader**) lua_touserdata(L, 1);
    delete shd;

    return 0;
}

int l_callPrimitiveRendererDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.primitiverenderer");
    auto prim = * (PrimitiveRenderer**) lua_touserdata(L, 1);
    delete prim;

    return 0;
}

int l_callFontRendererDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.fontrenderer");
    auto font = * (FontRenderer**) lua_touserdata(L, 1);
    delete font;

    return 0;
}
