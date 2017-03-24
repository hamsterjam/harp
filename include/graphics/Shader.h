#ifndef __GALAXY_FORT_SHADER_H
#define __GALAXY_FORT_SHADER_H

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

    public:
        // This will compile a default shader
        Shader();
        Shader(const char* vertSource, const char* fragSource, unsigned int numTextures);
        ~Shader();

        void draw(Sprite spr, int x, int y);

        void use(SceneObject so);
        GLuint getProgramID();
};

#endif
