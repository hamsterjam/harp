#ifndef __GALAXY_FORT_SCENE_OBJECT_H
#define __GALAXY_FORT_SCENE_OBJECT_H

#include <cstdlib>
#include <SDL_opengl.h>

class Shader;

class SceneObject {
    private:
        friend Shader;

        GLubyte* data;

        GLuint blockID;
        GLint bufferSize;
        GLuint bufferID;

        unsigned int numUniforms;

        const char** names;
        GLuint* indices;
        GLint* offsets;

    public:
        SceneObject(const char* blockName, unsigned int numUniforms, const char** uniformNames, Shader shd);
        ~SceneObject();

        void setUniform(const char* uniformName, std::size_t size, void* value);
        void updateBuffer();
};

#endif
