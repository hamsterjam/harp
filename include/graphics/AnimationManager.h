#ifndef HARP_ANIMATION_MANAGER_H
#define HARP_ANIMATION_MANAGER_H

#include <FunctionWrapper.h>
#include <ECS.h> // Entity typedef

class Sprite;

class AnimationManager {
    private:
        Sprite** frames;
        FunctionWrapper updateFunc;
        unsigned int currentFrame;

    public:
        AnimationManager() {}
        AnimationManager(Sprite** frames, unsigned int numFrames, int (*updateFunc)(Entity, double));
        AnimationManager(Sprite** frames, unsigned int numFrames, int luaFuncReference);
        ~AnimationManager();

        void    update(Entity e, double deltaT);
        Sprite& getCurrentFrame();
};

#endif
