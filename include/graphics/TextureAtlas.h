/*
 * HARP
 * graphics/TextureAtlas.h
 *
 * This is essentially just a Texture with some extra information that allows
 * you to automatically make sprites out of a sprite sheet (sometimes called
 * a texture atlas)
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_TEXTURE_ATLAS_H
#define HARP_TEXTURE_ATLAS_H

#include <SDL_opengl.h>

struct Texture;
class Sprite;

class TextureAtlas {
    private:
        Texture* tex;

        // This is a 3D array, UVCoords[y][x][component]
        unsigned int* UVCoords;

        unsigned int numTilesW;
        unsigned int numTilesH;

    public:
        TextureAtlas(const char* filename, int tileW, int tileH, int padding, int border);
        TextureAtlas(Texture* tex, int tileW, int tileH, int padding, int border);
        ~TextureAtlas();

        void addTextureToSprite(Sprite& spr, int tileX, int tileY);
        void addTextureToSprite(Sprite& spr, int i);

        void addTextureToSprite(Sprite& spr, const char* texUniform, const char* UVAttrib, int tileX, int tileY);
        void addTextureToSprite(Sprite& spr, const char* texUniform, const char* UVAttrib, int i);
};

#endif
