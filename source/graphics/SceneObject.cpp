#include <cstdlib>
#include <cstring>

#include <iostream>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <graphics/Shader.h>
#include <graphics/SceneObject.h>

SceneObject::SceneObject(const char* blockName, unsigned int numUniforms, const char** uniformNames, Shader shd) {
    this->numUniforms = numUniforms;
    names = uniformNames;
    programID = shd.getProgramID();

    blockID = glGetUniformBlockIndex(programID, blockName);
    glGetActiveUniformBlockiv(programID, blockID, GL_UNIFORM_BLOCK_DATA_SIZE, &bufferSize);

    glGenBuffers(1, &bufferID);

    // Uniform info
    indices = (GLuint*)  malloc(sizeof(GLuint) * numUniforms);
    offsets  = (GLint*)   malloc(sizeof(GLint)  * numUniforms);

    glGetUniformIndices(programID, numUniforms, names, indices);
    glGetActiveUniformsiv(programID, numUniforms, indices, GL_UNIFORM_OFFSET, offsets);

    data = (GLubyte*) malloc(bufferSize);
}

SceneObject::~SceneObject() {
    glDeleteBuffers(1, &bufferID);
    free(indices);
    free(offsets);
    free(data);
}

void SceneObject::setUniform(const char* uniformName, std::size_t size, void* value) {
    // Get the index of the uniform
    // This is a bad way to do this but fuck it
    int index = -1;
    for (int i = 0; i < numUniforms; ++i) {
        if (strcmp(uniformName, names[i]) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        std::cerr << "Couldn't find uniform '" << uniformName << "' in this block." << std::endl;
        exit(1);
    }

    memcpy(data + offsets[index], value, size);
}

void SceneObject::updateBuffer() {
    glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
    glBufferData(GL_UNIFORM_BUFFER, bufferSize, data, GL_STATIC_DRAW);

    // Please cleanup your shit
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
