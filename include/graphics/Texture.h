#ifndef __GALAXY_FORT_TEXTURE_H
#define __GALAXY_FORT_TEXTURE_H

#include <SDL_opengl.h>

struct Texture {
    int w, h;
    int channels;
    GLuint textureID;
};

Texture* createTexture(const char* filename);
// Because this acts like a static manager class, this unloads ALL textures
void destroyTextures();

#endif
