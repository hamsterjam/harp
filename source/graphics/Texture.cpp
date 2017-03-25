#include <cstdlib>

#include <iostream>
#include <map>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <stb_image.h>

#include <graphics/Texture.h>

static std::map<Texture*> loadedTextures;

Texture* createTexture(const char* filename) {
    if (loadedTextures.count(filename) != 0) {
        return loadedTextures[filename];
    }
    Texture* ret = (Texture*) malloc(sizeof(Texture));

    unsigned char* data = stbi_load(filename, &ret->w, &ret->h, &ret->channels, 0);

    if (!data) {
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
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, ret->w, ret->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

    loadedTextures[filename] = ret;

    return ret;
}

// Because this acts
void destroyTextures() {
    for (auto item : loadedTextures) {
        Texture* tex = item.second;
        glDeleteTextures(1, &tex->textureID);
    }
}
