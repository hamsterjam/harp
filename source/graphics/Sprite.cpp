#include <graphics/Sprite.h>
#include <graphics/Texture.h>

const char* DEFAULT_TEXTURE_UNIFORM = "uTexture";
const char* DEFAULT_UV_ATTRIBUTE    = "aTexCoord";

typedef unsigned int uint;

Sprite::Sprite(const char* filename) {
    texSpecifier spec;
    spec.tex = createTexture(filename);
    spec.texUniform = DEFAULT_TEXTURE_UNIFORM;

    spec.u1 = 0;
    spec.u2 = 1;

    // Remember to flip the y-axis
    spec.v1 = 1;
    spec.v2 = 0;

    spec.UVAttrib = DEFAULT_UV_ATTRIBUTE;

    textures.push_back(spec);

    w = spec.tex->w;
    h = spec.tex->h;
}


