#include <cstdlib>

#include <SDL_opengl.h>

#include <globals.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Texture.h>
#include <graphics/Sprite.h>

TextureAtlas::TextureAtlas(const char* filename, int tileW, int tileH, int padding, int border) :
    TextureAtlas(createTexture(filename), tileW, tileH, padding, border) {
    // That's all
}

TextureAtlas::TextureAtlas(Texture* tex, int tileW, int tileH, int padding, int border) {
    this->tex = tex;

    numTilesW = (tex->w - border + padding) / (tileW + padding);
    numTilesH = (tex->h - border + padding) / (tileH + padding);

    UVCoords = (unsigned int*) malloc(sizeof(unsigned int) * numTilesW * numTilesH * 4);

    // Now to fill up UVCoords
    for (int x = 0; x < numTilesW; ++x) {
        for (int y = 0; y < numTilesH; ++y) {
            unsigned int offset = 0;

            offset += x;
            offset *= numTilesH;
            offset += y;
            offset *= 4;

            unsigned int* curr = UVCoords + offset;

            // You would be right to ask why im storing tileW and tileH over and over
            // the answer as to why is that I want this to be variable in the future
            curr[0] = border + x * (tileW + padding);
            curr[1] = border + y * (tileH + padding);
            curr[2] = tileW;
            curr[3] = tileH;
        }
    }
}

TextureAtlas::~TextureAtlas() {
    free(UVCoords);
}

void TextureAtlas::addTextureToSprite(Sprite& spr, int tileX, int tileY) {
    addTextureToSprite(spr, DEFAULT_TEXTURE_UNIFORM, DEFAULT_UV_ATTRIBUTE, tileX, tileY);
}

void TextureAtlas::addTextureToSprite(Sprite& spr, int i) {
    addTextureToSprite(spr, DEFAULT_TEXTURE_UNIFORM, DEFAULT_UV_ATTRIBUTE, i);
}

void TextureAtlas::addTextureToSprite(Sprite& spr, const char* texUniform, const char* UVAttrib, int tileX, int tileY) {
    unsigned int offset = 0;

    offset += tileX;
    offset *= numTilesH;
    offset += tileY;
    offset *= 4;

    int pixelX = UVCoords[offset + 0];
    int pixelY = UVCoords[offset + 1];
    int texW   = UVCoords[offset + 2];
    int texH   = UVCoords[offset + 3];

    spr.addSubTexture(tex, texUniform, UVAttrib, pixelX, pixelY, texW, texH);
}

void TextureAtlas::addTextureToSprite(Sprite& spr, const char* texUniform, const char* UVAttrib, int i) {
    unsigned int offset = 0;

    offset += i;
    offset *= 4;

    int pixelX = UVCoords[offset + 0];
    int pixelY = UVCoords[offset + 1];
    int texW   = UVCoords[offset + 2];
    int texH   = UVCoords[offset + 3];

    spr.addSubTexture(tex, texUniform, UVAttrib, pixelX, pixelY, texW, texH);
}

