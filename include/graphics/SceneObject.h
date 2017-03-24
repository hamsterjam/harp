#ifndef __GALAXY_FORT_SCENE_OBJECT_H
#define __GALAXY_FORT_SCENE_OBJECT_H

#include <cstdlib>

#include <map>

#include <SDL_opengl.h>

class Shader;

class SceneObject {
    private:
        struct UniformSpec {
            const char* name;
            std::size_t size;
            GLint offset;
            void* data;
        };

        const char* blockName;
        GLuint blockID;

        GLuint bufferID;
        GLint bufferSize;

        std::map<const char*, UniformSpec> uniforms;

        bool needsShaderInit;
        bool needsBufferUpdate;

    public:
        SceneObject(const char* blockName);

        void setUniform(const char* name, std::size_t size, void* value);
        void shaderInit(Shader shd);
        void updateBuffer();

        GLuint getBlockID();
        GLuint getBufferID();
};

#endif
