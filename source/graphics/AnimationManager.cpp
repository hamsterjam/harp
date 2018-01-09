#include <graphics/AnimationManager.h>

#include <cstring> // memcpy
#include <cstdlib>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
}

#include <FunctionWrapper.h>
#include <graphics/Sprite.h>
#include <ECS.h>
#include <globals.h>
#include <Console.h>
#include <luaInterface.h>

AnimationManager::AnimationManager(Sprite** frames, unsigned int numFrames, int (*updateFunc)(Entity, double)) {
    // Copy the frames into an array owned by this object
    this->frames = (Sprite**) malloc(sizeof(Sprite*) * numFrames);
    memcpy(this->frames, frames, sizeof(Sprite*) * numFrames);

    // Set up the update function
    this->updateFunc.isLua = false;
    this->updateFunc.cFunc = (void(*)()) updateFunc;

    this->currentFrame = 0;
}

AnimationManager::AnimationManager(Sprite** frames, unsigned int numFrames, int luaFuncReference) {
    // Copy the frames into an array owned by this object
    this->frames = (Sprite**) malloc(sizeof(Sprite*) * numFrames);
    mempcpy(this->frames, frames, sizeof(Sprite*) * numFrames);

    // Set up the update function
    this->updateFunc.isLua = true;
    this->updateFunc.luaFunc = luaFuncReference;

    this->currentFrame = 0;
}

AnimationManager::~AnimationManager() {
    // We own the frames array, so we need to free it
    if (frames) free(frames);
}

void AnimationManager::update(Entity e, double deltaT) {
    // All we want to do is call the update function
    int newState = -1;

    if (updateFunc.isLua) {
        // Push the function
        getWeakLuaRef(L, updateFunc.luaFunc);
        // Push the lua reference to the Entity
        auto entRef = * (int*) harp.getComponent(e, meta_luaRef);
        getWeakLuaRef(L, entRef);
        // Push deltaT
        lua_pushnumber(L, deltaT);

        // Call the function
        if (lua_pcall(L, 2, 1, 0)) {
            // Something bad happened
            const char* errorMessage = lua_tostring(L, -1);
            Console::getInstance().log("EE " + std::string(errorMessage));
            lua_pop(L, 1);
        }
        else {
            // No error
            newState = lua_tointeger(L, -1);
            lua_pop(L, 1);
        }
    }
    else {
        // Call the C function
        auto cFunc = (int (*)(Entity, double)) updateFunc.cFunc;
        newState = cFunc(e, deltaT);
    }

    // Do something with the newState variable
    if (newState >= 0) {
        currentFrame = (unsigned int) newState;
    }
}

Sprite& AnimationManager::getCurrentFrame() {
    return *frames[currentFrame];
}
