#include <luaFunctions.h>

#include <string>

#include <cstdlib>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <luaInterface.h>

#include <globals.h>
#include <Console.h>
#include <harpMath.h>
#include <Collider.h>
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


//
// Entity operations
//

int l_createEntity(lua_State* L) {
    auto& ent = * (Entity*) lua_newuserdata(L, sizeof(Entity));
    luaL_getmetatable(L, "harp.entity");
    lua_setmetatable(L, -2);

    auto weakRef = weakLuaRef(L);

    ent = harp.createEntity();
    harp.setComponent(ent, meta_luaRef, &weakRef);

    return 1;
}

int l_deleteEntity(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    auto ent = * (Entity*) lua_touserdata(L, 1);

    harp.deleteEntity(ent);

    return 0;
}

int l_setParent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    luaL_checkudata(L, 2, "harp.entity");

    auto ent = * (Entity*) lua_touserdata(L, 1);
    auto par = * (Entity*) lua_touserdata(L, 2);

    harp.setParent(ent, par);

    return 0;
}

int l_removeParent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");

    auto ent = * (Entity*) lua_touserdata(L, 1);
    harp.removeParent(ent);

    return 1;
}

//
// Component operations
//

int l_setComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    auto ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.component");
    auto comp = * (Component*) lua_touserdata(L, 2);

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

int l_getComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    luaL_checkudata(L, 2, "harp.component");

    auto ent  = * (Entity*)    lua_touserdata(L, 1);
    auto comp = * (Component*) lua_touserdata(L, 2);

    void* val        = harp.getComponent(ent, comp);
    std::size_t size = harp.sizeOfComponent(comp);

    void* ret = lua_newuserdata(L, size);
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    memcpy(ret, val, size);

    return 1;
}

int l_hasComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    luaL_checkudata(L, 2, "harp.component");

    auto ent  = * (Entity*)    lua_touserdata(L, 1);
    auto comp = * (Component*) lua_touserdata(L, 2);

    lua_pushboolean(L, (bool) harp.getComponent(ent, comp));

    return 1;
}

int l_removeComponent(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    luaL_checkudata(L, 2, "harp.component");

    auto ent  = * (Entity*)    lua_touserdata(L, 1);
    auto comp = * (Component*) lua_touserdata(L, 2);

    harp.removeComponent(ent, comp);

    return 0;
}

int l_setFlag(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    auto ent = * (Entity*) lua_touserdata(L, 1);

    luaL_checkudata(L, 2, "harp.flag");
    auto comp = * (Component*) lua_touserdata(L, 2);

    luaL_checktype(L, 3, LUA_TBOOLEAN);
    bool val = lua_toboolean(L, 3);

    harp.setFlag(ent, comp, val);

    return 0;
}

int l_getFlag(lua_State* L) {
    luaL_checkudata(L, 1, "harp.entity");
    luaL_checkudata(L, 2, "harp.flag");

    auto ent  = * (Entity*)    lua_touserdata(L, 1);
    auto flag = * (Component*) lua_touserdata(L, 2);

    bool val = harp.getFlag(ent, flag);

    lua_pushboolean(L, val);
    return 1;
}

//
// Conversion (Lua -> C)
//

int l_Vec2(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_geti(L, 1, 1);
    lua_geti(L, 1, 2);

    double val1 = lua_tonumber(L, -2);
    double val2 = lua_tonumber(L, -1);

    lua_pop(L, 2);

    auto ret = (Vec<2, double>*) lua_newuserdata(L, sizeof(Vec<2, double>));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->data[0] = val1;
    ret->data[1] = val2;
    return 1;
}

int l_Mat3(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    auto ret = (Matrix<3, 3, double>*) lua_newuserdata(L, sizeof(Matrix<3, 3, double>));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    for (int i = 0; i < 3; ++i) {
        lua_geti(L, 1, i+1); // Remember, Lua is 1 indexed.
        luaL_checktype(L, -1, LUA_TTABLE);
        for (int j = 0; j < 3; ++j) {
            lua_geti(L, -1, j+1);
            (*ret)[i][j] = lua_tonumber(L, -1);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }

    return 1;
}

int l_LineCollider(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_geti(L, 1, 1);
    lua_geti(L, 1, 2);

    double val1 = lua_tonumber(L, -2);
    double val2 = lua_tonumber(L, -1);

    lua_pop(L, 2);

    auto ret = (Collider*) lua_newuserdata(L, sizeof(Collider));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->type = LINE_COLLIDER;
    ret->delta[0] = val1;
    ret->delta[1] = val2;

    return 1;
}

int l_BoxCollider(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_geti(L, 1, 1);
    lua_geti(L, 1, 2);

    double val1 = lua_tonumber(L, -2);
    double val2 = lua_tonumber(L, -1);

    lua_pop(L, 2);

    auto ret = (Collider*) lua_newuserdata(L, sizeof(Collider));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);
    ret->type = BOX_COLLIDER;
    ret->delta[0] = val1;
    ret->delta[1] = val2;

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

int l_AnimationManager(lua_State* L) {
    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TFUNCTION);

    // I can't staticly allocate this... Which means it gets malloced twice...
    int numFrames = luaL_len(L, 1);
    auto frames = (Sprite**) malloc(sizeof(Sprite*) * numFrames);
    for (int i = 0; i < numFrames; ++i) {
        // Remember Lua is FUCKING ONE INDEXED
        lua_geti(L, 1, i+1);
        luaL_checkudata(L, -1, "harp.sprite");
        frames[i] = * (Sprite**) lua_touserdata(L, -1);
        lua_pop(L, 1);
    }

    int funcRef = weakLuaRef(L);

    auto ret = (AnimationManager**) lua_newuserdata(L, sizeof(AnimationManager*));
    luaL_getmetatable(L, "harp.animationmanager");
    lua_setmetatable(L, -2);
    *ret = new AnimationManager(frames, numFrames, funcRef);

    free(frames);

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

//
// Conversion (C -> Lua)
//

int l_asNumber(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob");

    double val = * (double*) lua_touserdata(L, 1);

    lua_pushnumber(L, val);
    return 1;
}

int l_asInteger(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob");

    int val = * (int*) lua_touserdata(L, 1);

    lua_pushinteger(L, val);
    return 1;
}

int l_asFunction(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob");

    FunctionWrapper val = * (FunctionWrapper*) lua_touserdata(L, 1);

    if (!val.isLua) {
        lua_pushnil(L);
        return 1;
    }

    getWeakLuaRef(L, val.luaFunc);
    return 1;
}

int l_asVec2(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob");

    Vec<2, double> val = * (Vec<2, double>*) lua_touserdata(L, 1);

    // Remember Lua indexes from one!!
    lua_newtable(L);
    lua_pushnumber(L, val[0]); lua_seti(L, -2, 1);
    lua_pushnumber(L, val[1]); lua_seti(L, -2, 2);

    return 1;
}

int l_asMat3(lua_State* L) {
    luaL_checkudata(L, 1, "harp.blob");

    Matrix<3, 3, double> val = * (Matrix<3, 3, double>*) lua_touserdata(L, 1);

    lua_newtable(L);
    for (int i = 0; i < 3; ++i) {
        lua_newtable(L);
        for (int j = 0; j < 3; ++j) {
            lua_pushnumber(L, val[i][j]);
            lua_seti(L, -2, j+1);
        }
        lua_seti(L, -2, i+1);
    }

    return 1;
}

//
// Visual Specs
//

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

int l_AnimationSpec(lua_State* L) {
    Shader* shd = defaultShader;

    int numArgs = lua_gettop(L);
    if (numArgs < 3) {
        luaL_error(L, "AnimationSpec takes at least 3 arguments, found %d", numArgs);
    }
    if (numArgs >= 4) {
        luaL_checkudata(L, 4, "harp.shader");
        shd = * (Shader**) lua_touserdata(L, 1);
    }

    luaL_checkudata(L, 1, "harp.animationmanager");
    auto man = * (AnimationManager**) lua_touserdata(L, 1);
    auto  dx = (float) luaL_checknumber(L, 2);
    auto  dy = (float) luaL_checknumber(L, 3);

    auto ret = (VisualSpec*) lua_newuserdata(L, sizeof(VisualSpec));
    luaL_getmetatable(L, "harp.blob");
    lua_setmetatable(L, -2);

    *ret = getAnimationSpec(*man, dx, dy, *shd);

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

int l_printRaw(lua_State* L) {
    std::string message;
    if (lua_isstring(L, 1)) {
        message = std::string(lua_tostring(L, 1));
    }
    else if (lua_isboolean(L, 1)) {
        bool value = lua_toboolean(L, 1);
        message = (value) ? "true" : "false";
    }
    else if (lua_isnil(L, 1)) {
        message = "nil";
    }
    else if (lua_isuserdata(L, 1)) {
        if (luaL_testudata(L, 1, "harp.entity")) {
            auto id = * (Entity*) lua_touserdata(L, 1);
            message = "Entity: " + std::to_string(id);
        }
        else if (luaL_testudata(L, 1, "harp.component")) {
            auto id = * (Component*) lua_touserdata(L, 1);
            message = "Component: " + std::to_string(id);
        }
        else if (luaL_testudata(L, 1, "harp.flag")) {
            auto id = * (Component*) lua_touserdata(L, 1);
            message = "Flag: " + std::to_string(id);
        }
        else {
            luaL_error(L, "Attempted to print non-printable type");
        }
    }
    else {
        luaL_error(L, "Attempted to print non-printable type");
    }
    Console::getInstance().log("<- " + message);
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

int l_callAnimationManagerDestructor(lua_State* L) {
    luaL_checkudata(L, 1, "harp.animationmanager");
    auto anim = * (AnimationManager**) lua_touserdata(L, 1);
    delete anim;

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
