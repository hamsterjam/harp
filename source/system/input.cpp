#include <systems.h>

#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
#include <SDL.h>

#include <ECS.h>
#include <FunctionWrapper.h>
#include <luaInterface.h>
#include <Console.h>
#include <harpMath.h>
#include <globals.h>
#include <Action.h>

const SDL_Scancode UP_KEY    = SDL_SCANCODE_W;
const SDL_Scancode DOWN_KEY  = SDL_SCANCODE_S;
const SDL_Scancode LEFT_KEY  = SDL_SCANCODE_A;
const SDL_Scancode RIGHT_KEY = SDL_SCANCODE_D;

const double WALK_MAGNITUDE = 300;
const double JUMP_MAGNITUDE = 10000;

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

    for (auto it = ecs.begin({comp_inputFunction}); it != ecs.end(); ++it) {
        Entity e = *it;

        // Check if this ent is done processing
        if (ecs.getComponent(e, comp_partialStep)) {
            auto partialStep = * (double *) ecs.getComponent(e, comp_partialStep);
            if (partialStep == 0) continue;
        }

        auto inputFunction = * (FunctionWrapper *) ecs.getComponent(e, comp_inputFunction);

        // This will be returned by inputFunction
        Action action;

        if (inputFunction.isLua) {
            getWeakLuaRef(L, inputFunction.luaFunc);
            auto luaref = * (int*) ecs.getComponent(e, meta_luaRef);
            getWeakLuaRef(L, luaref);

            lua_pushvalue(L, -3);

            if (lua_pcall(L, 2, 1, 0)) {
                // Something bad happened
                const char* errorMessage = lua_tostring(L, -1);
                Console::getInstance().log("EE " + std::string(errorMessage));
                lua_pop(L, 1);
            }

            action = * (Action *) lua_touserdata(L, -1);
            lua_pop(L, 1);
        }
        else {
            auto func = (Action(*)(Entity)) inputFunction.cFunc;
            action = func(e);
        }

        //
        // Now we have the inputed action, time to do something with it
        //

        Vec<2, double> acc;
        if (ecs.getComponent(e, comp_acceleration)) {
            acc = * (Vec<2, double> *) ecs.getComponent(e, comp_acceleration);
        }
        else {
            acc = zeroVec<2, double>();
        }

        switch (action) {
            case (Action::NO_ACTION):
                // Do nothing
                break;
            case (Action::JUMP):
                acc[1] += JUMP_MAGNITUDE;
                break;
            case (Action::WALK_LEFT):
                acc[0] -= WALK_MAGNITUDE;
                break;
            case (Action::WALK_RIGHT):
                acc[0] += WALK_MAGNITUDE;
                break;
            case (Action::FAST_DROP):
                acc[1] -= WALK_MAGNITUDE;
                break;
            default:
                assert(false);
        }

        ecs.setComponent(e, comp_acceleration, &acc);
    }

    lua_pop(L, 1);
}
