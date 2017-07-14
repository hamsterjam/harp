/*
 * HARP
 * graphics/Texture.h
 *
 * This is the first abstraction of an image in my system. It is a managed
 * wrapper of an OpenGL Texture. This means that if you try to create a second
 * textures from the same filename, the createTexture function will give you
 * the pointer to the old Texture rather than allocating memory for the same
 * image twice.
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_TEXTURE_H
#define HARP_TEXTURE_H

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
