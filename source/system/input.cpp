#include <systems.h>

#include <cassert>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}
#include <SDL.h>

#include <ECS.h>
#include <DynamicPoolAllocator.h>
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

const std::size_t POOL_SIZE = 128;
const double WALK_MAGNITUDE = 300;
const double JUMP_MAGNITUDE = 10000;

// This linked list is not a good solution for this

struct SurfaceList {
    Vec<2, double> surf;
    SurfaceList* next;
};

struct SurfaceListList {
    Entity ent;
    SurfaceList* surfs;
    SurfaceListList* next;
};

static DynamicPoolAllocator pool(POOL_SIZE);

static SurfaceListList* surfsThisFrame = NULL;

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

        // Check if we are on a surface
        if (ecs.getComponent(e, comp_onSurface)) {
            auto surfNorm = * (Vec<2, double> *) ecs.getComponent(e, comp_onSurface);

            // Add the current surface to the surface list for this entity if
            // it exists, otherwise make a list for it
            SurfaceListList** currList = &surfsThisFrame;
            while (*currList) {
                if ((*currList)->ent == e) break;
                currList = &(*currList)->next;
            }
            if (!(*currList)) {
                *currList = (SurfaceListList*) pool.alloc(sizeof(SurfaceListList));
                (*currList)->ent = e;
                (*currList)->surfs = NULL;
                (*currList)->next = NULL;
            }

            // Traverse this list till we get to the end and add our new list
            SurfaceList** currSurf = &(*currList)->surfs;
            while (*currSurf) currSurf = &(*currSurf)->next;
            *currSurf = (SurfaceList*) pool.alloc(sizeof(SurfaceList));
            (*currSurf)->surf = surfNorm;
            (*currSurf)->next = NULL;
        }
        // If we aren't on a surface, remove the list associated with that entity
        else {
            SurfaceListList** currList = &surfsThisFrame;
            while (*currList) {
                if ((*currList)->ent == e) break;
                currList = &(*currList)->next;
            }
            if (*currList) *currList = (*currList)->next;

            // P.S. we don't have to worry about deallocating stuff (for now)
            // this is a pool allocator
        }

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

        // Remove acceleration that would take us back through any of the
        // surfaces we are already on.
        {
            SurfaceListList* currList = surfsThisFrame;
            while (currList) {
                if (currList->ent == e) break;
                currList = currList->next;
            }

            while (currList) {
                SurfaceList* currSurf = currList->surfs;

                Vec<2, double> surfNorm = currSurf->surf;

                auto accPerpToSurf = proj(acc, surfNorm);
                if (dot(accPerpToSurf, surfNorm) < 0) {
                    // If this is negative, we are accelerationg toward the surface
                    acc -= accPerpToSurf;
                }

                currList = currList->next;
            }
        }

        ecs.setComponent(e, comp_acceleration, &acc);
    }
    // If the list of surface lists is empty, we can free the pool, this will
    // always happen at least once per frame
    if (!surfsThisFrame) pool.freeAll();

    lua_pop(L, 1);
}
