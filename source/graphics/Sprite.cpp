#include <graphics/Sprite.h>
#include <graphics/Texture.h>

typedef unsigned int uint;

Sprite::Sprite(const char* filename) : Sprite(createTexture(filename)) {}

Sprite::Sprite(Texture* tex) {
    this->tex = tex;
    u1 = 0;
    u2 = 1;
    // Make sure to flip the y axis
    v1 = 1;
    v2 = 0;
}

Sprite::Sprite(Texture* tex, uint x, uint y, uint w, uint h) : Sprite(tex) {
    float texWidth  = (float) tex->w;
    float texHeight = (float) tex->h;

    // Values outside of [0, 1] aren't errors per se, but they may behave strange

    u1 = (float) x / texWidth;
    u2 = (float) (x + w) / texWidth;
    v1 = (float) y / texHeight;
    v2 = (float) (y + h) / texHeight;

    // Again, make sure to flip the y axis
    v1 = 1.0 - v1;
    v2 = 1.0 - v2;
}

