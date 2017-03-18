#ifndef __GALAXY_FORT_TEXTURE_H
#define __GALAXY_FORT_TEXTURE_H

#include <SDL2/SDL_opengl.h>

struct Texture {
    int w, h;
    int channels;
    GLuint textureID;
    unsigned char* data;
};

Texture* createTexture(const char* filename);
void destroyTexture(Texture* tex);

#endif
