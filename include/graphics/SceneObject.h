#ifndef __GALAXY_FORT_SCENE_OBJECT_H
#define __GALAXY_FORT_SCENE_OBJECT_H

#include <cstdlib>

#include <map>

#include <SDL_opengl.h>

class Shader;

class SceneObject {
    private:
        struct UniformSpec {
            std::size_t size;
            GLint offset;
            void* data;
        };

        const char* blockName;
        GLuint blockID;

        GLuint bufferID;
        GLint bufferSize;

        GLuint lastProgramID;

        std::map<const char*, UniformSpec> uniforms;

        bool needsShaderInit;
        bool needsBufferUpdate;

    public:
        SceneObject(const char* blockName);
        SceneObject(SceneObject& clone);
        ~SceneObject();

        void setUniform(const char* name, std::size_t size, void* value);
        void shaderInit(Shader& shd);
        void updateBuffer(Shader& shd);
};

#endif
