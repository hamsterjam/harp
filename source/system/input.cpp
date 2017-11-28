#include <systems.h>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
#include <SDL.h>

#include <ECS.h>
#include <FunctionWrapper.h>
#include <luaInterface.h>
#include <Console.h>
#include <globals.h>

const SDL_Scancode UP_KEY    = SDL_SCANCODE_W;
const SDL_Scancode DOWN_KEY  = SDL_SCANCODE_S;
const SDL_Scancode LEFT_KEY  = SDL_SCANCODE_A;
const SDL_Scancode RIGHT_KEY = SDL_SCANCODE_D;

void system_input(ECS& ecs, bool acceptingInput) {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    lua_newtable(L);

    if (acceptingInput) {
        lua_pushboolean(L, keyState[UP_KEY]);    lua_setfield(L, -2, "up");
        lua_pushboolean(L, keyState[DOWN_KEY]);  lua_setfield(L, -2, "down");
        lua_pushboolean(L, keyState[LEFT_KEY]);  lua_setfield(L, -2, "left");
        lua_pushboolean(L, keyState[RIGHT_KEY]); lua_setfield(L, -2, "right");
    }
    // Else, all this will have a value of nil which tests as if it were false

    for (auto it = ecs.begin({comp_inputFunc}); it != ecs.end(); ++it) {
        Entity e = *it;
        auto inputFunc = * (FunctionWrapper *) ecs.getComponent(e, comp_inputFunc);


        if (inputFunc.isLua) {
            getWeakLuaRef(L, inputFunc.luaFunc);
            auto luaref = * (int*) ecs.getComponent(e, meta_luaRef);
            getWeakLuaRef(L, luaref);

            lua_pushvalue(L, -3);

            if (lua_pcall(L, 2, 0, 0)) {
                // Something bad happened
                const char* errorMessage = lua_tostring(L, -1);
                Console::getInstance().log("EE " + std::string(errorMessage));
                lua_pop(L, 1);
            }
        }
        else {
            auto func = (void(*)(Entity)) inputFunc.cFunc;
            func(e);
        }
    }

    lua_pop(L, 1);
}
