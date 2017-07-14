/*
 * HARP
 * graphics/SceneObject.h
 *
 * This is a wrapper on a Uniform Buffer Object. It is supposed to be a nicer
 * way to set the uniforms in your shader program without having to interface
 * with OpenGL directly every time
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_SCENE_OBJECT_H
#define HARP_SCENE_OBJECT_H

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
        void updateBuffer(Shader& shd, GLuint bindingPoint);
};

#endif
