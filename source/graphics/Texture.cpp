#include <cstdlib>
#include <iostream>

#include <GL/glew.h>

#include <SDL_opengl.h>
#include <stb_image.h>

#include <graphics/Texture.h>

// Just wrap the stb_image interface for now, in the future this
// should make sure it only loads each filename once.

Texture* createTexture(const char* filename) {
    Texture* ret = (Texture*) malloc(sizeof(Texture));

    ret->data = stbi_load(filename, &ret->w, &ret->h, &ret->channels, 0);

    if (!ret->data) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        exit(1);
    }

    // OpenGL texture stuff

    glGenTextures(1, &ret->textureID);

    glBindTexture(GL_TEXTURE_2D, ret->textureID);

    GLint internalFormat;
    switch (ret->channels) {
        case 1:
            internalFormat = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA;
            break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, ret->w, ret->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ret->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return ret;
}

void destroyTexture(Texture* tex) {
    stbi_image_free(tex->data);
    glDeleteTextures(1, &tex->textureID);
    free(tex);
}
