#ifndef __GALAXY_FORT_SHADER_H
#define __GALAXY_FORT_SHADER_H

#include <vector>
#include <functional>
#include <SDL_opengl.h>

class Sprite;
class SceneObject;

class Shader {
    private:
        char* vertSource;
        char* fragSource;

        unsigned int numTextures;

        GLuint programID;
        GLuint vertPosBuffer;

        // A list might be faster
        std::vector<std::reference_wrapper<SceneObject>> SceneObjects;

    public:
        // This will compile a default shader
        Shader();
        Shader(const char* vertSource, const char* fragSource, unsigned int numTextures);
        ~Shader();

        void draw(Sprite spr, int x, int y);
        void use(SceneObject& so);

        GLuint getProgramID();
};

#endif
