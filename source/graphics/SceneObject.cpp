#include <cstdlib>
#include <cstring>

#include <map>
#include <utility>
#include <iostream>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <graphics/Shader.h>
#include <graphics/SceneObject.h>

using namespace std;

SceneObject::SceneObject(const char* blockName) {
    this->blockName = blockName;

    glGenBuffers(1, &bufferID);
    bufferSize = 0;

    needsShaderInit = true;
    needsBufferUpdate = true;
}

void SceneObject::setUniform(const char* name, size_t size, void* value) {
    if (uniforms.count(name) == 0) {
        // New uniform
        UniformSpec spec;
        spec.name = name;
        spec.size = size;
        spec.data = malloc(size);
        memcpy(spec.data, value, size);

        uniforms[name] = spec;

        needsShaderInit = true;
    }
    else {
        // Modify existing uniform
        UniformSpec spec = uniforms[name];

        // Might be a good idea to error if the sizes don't match? idk.
        if (spec.size != size) {
            spec.data = realloc(spec.data, size);
            spec.size = size;
        }
        memcpy(spec.data, value, size);

        uniforms[name] = spec;
    }

    needsBufferUpdate = true;
}

void SceneObject::shaderInit(Shader shd) {
    if (!needsShaderInit) {
        return;
    }
    GLuint programID = shd.getProgramID();

    blockID = glGetUniformBlockIndex(programID, blockName);
    glGetActiveUniformBlockiv(programID, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);

    // Now get the uniform indices and offsets
    for (auto& item : uniforms) {
        UniformSpec spec = item.second;
        GLuint index;
        glGetUniformIndices(programID, 1, &spec.name, &index);
        glGetActiveUniformsiv(programID, 1, &index, GL_UNIFORM_OFFSET, &spec.offset);
        item.second = spec;
    }

    needsShaderInit = false;
}

void SceneObject::updateBuffer() {
    if (needsShaderInit) {
        // You need to do that shader init stuff first, this really should error.
        return;
    }
    if (!needsBufferUpdate) {
        return;
    }

    GLubyte* bufferData = (GLubyte*) malloc(bufferSize);

    // Zero it out so any uniforms we havn't set are defaulted to 0
    memset(bufferData, 0, bufferSize);

    for (auto it = uniforms.begin(); it != uniforms.end(); ++it) {
        UniformSpec spec = it->second;
        memcpy(bufferData + spec.offset, spec.data, spec.size);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);

    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    free(bufferData);

    needsBufferUpdate = false;
}

GLuint SceneObject::getBlockID() {
    return blockID;
}

GLuint SceneObject::getBufferID() {
    return bufferID;
}
