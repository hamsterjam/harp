#include <iostream>

#include <graphics/Sprite.h>
#include <graphics/Texture.h>

const char* DEFAULT_TEXTURE_UNIFORM = "uTexture";
const char* DEFAULT_UV_ATTRIBUTE    = "aTexCoord";

typedef unsigned int uint;

Sprite::texSpecifier Sprite::defaultSpec(Texture* tex) {
    texSpecifier spec;
    spec.tex = tex;
    spec.texUniform = DEFAULT_TEXTURE_UNIFORM;

    spec.u1 = 0;
    spec.u2 = 1;

    // Remember to flip the y-axis
    spec.v1 = 1;
    spec.v2 = 0;

    spec.UVAttrib = DEFAULT_UV_ATTRIBUTE;

    return spec;
}

Sprite::Sprite() {
    w = 0;
    h = 0;
}

Sprite::Sprite(const char* filename) : Sprite(createTexture(filename)){
    // That's all
}

Sprite::Sprite(Texture* tex) {
    texSpecifier spec = defaultSpec(tex);

    textures.push_back(spec);

    w = spec.tex->w;
    h = spec.tex->h;
}

void Sprite::addImage(const char* filename, const char* texUniform, const char* UVAttrib) {
    addTexture(createTexture(filename), texUniform, UVAttrib);
}

void Sprite::addTexture(Texture* tex, const char* texUniform, const char* UVAttrib) {
    texSpecifier spec = defaultSpec(tex);
    spec.texUniform = texUniform;
    spec.UVAttrib = UVAttrib;

    textures.push_back(spec);
}
