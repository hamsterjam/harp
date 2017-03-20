#ifndef __GALAXY_FORT_SHADER_H
#define __GALAXY_FORT_SHADER_H

#include <SDL_opengl.h>

class Shader {
    private:
        char* vertSource;
        char* fragSource;

        GLuint programID;

    public:
        // This will compile a default shader
        Shader();
        ~Shader();

        void draw();
};

#endif
