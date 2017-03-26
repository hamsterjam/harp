#include <iostream>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <graphics/Sprite.h>
#include <graphics/Texture.h>
#include <graphics/SceneObject.h>

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

    glGenBuffers(1, &spec.UVBuffer);

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

    auxData = NULL;
    needsBufferUpdates = true;
}

Sprite::Sprite(const char* filename, uint x, uint y, int w, int h) : Sprite(createTexture(filename), x, y, w, h) {
    // That's all
}

Sprite::Sprite(Texture* tex, uint x, uint y, int w, int h) {
    texSpecifier spec = defaultSpec(tex);

    int texW = tex->w;
    int texH = tex->h;

    GLfloat u1 = (GLfloat) x / (GLfloat) texW;
    GLfloat v1 = (GLfloat) y / (GLfloat) texH;
    GLfloat u2 = (GLfloat) (x + w) / (GLfloat) texW;
    GLfloat v2 = (GLfloat) (y + h) / (GLfloat) texH;

    // Remember to flip the y axis
    v1 = 1.0 - v1;
    v2 = 1.0 - v2;

    spec.u1 = u1;
    spec.u2 = u2;
    spec.v1 = v1;
    spec.v2 = v2;

    textures.push_back(spec);

    this->w = w;
    this->h = h;

    auxData = NULL;
    needsBufferUpdates = true;
}

Sprite::~Sprite() {
    delete auxData;
}

void Sprite::addImage(const char* filename, const char* texUniform, const char* UVAttrib) {
    addTexture(createTexture(filename), texUniform, UVAttrib);
}

void Sprite::addTexture(Texture* tex, const char* texUniform, const char* UVAttrib) {
    texSpecifier spec = defaultSpec(tex);
    spec.texUniform = texUniform;
    spec.UVAttrib = UVAttrib;

    textures.push_back(spec);

    if (w == 0 && h == 0) {
        w = spec.tex->w;
        h = spec.tex->h;
    }

    needsBufferUpdates = true;
}

void Sprite::addSubImage(const char* filename, const char* texUniform, const char* UVAttrib,
                         uint x, uint y, int w, int h) {
    addSubTexture(createTexture(filename), texUniform, UVAttrib, x, y, w, h);
}

void Sprite::addSubTexture(Texture* tex, const char* texUniform, const char* UVAttrib,
                           uint x, uint y, int w, int h) {
    if (this->w == 0 && this->h == 0) {
        this->w = w;
        this->h = h;
    }

    texSpecifier spec;

    spec.tex = tex;
    spec.texUniform = texUniform;
    spec.UVAttrib   = UVAttrib;

    glGenBuffers(1, &spec.UVBuffer);

    int texW = tex->w;
    int texH = tex->h;

    GLfloat u1 = (GLfloat) x / (GLfloat) texW;
    GLfloat v1 = (GLfloat) y / (GLfloat) texH;
    GLfloat u2 = (GLfloat) (x + w) / (GLfloat) texW;
    GLfloat v2 = (GLfloat) (y + h) / (GLfloat) texH;

    // Remember to flip the y axis
    v1 = 1.0 - v1;
    v2 = 1.0 - v2;

    spec.u1 = u1;
    spec.u2 = u2;
    spec.v1 = v1;
    spec.v2 = v2;

    textures.push_back(spec);

    needsBufferUpdates = true;
}

void Sprite::setAuxData(SceneObject& auxData) {
    if (this->auxData) {
        delete this->auxData;
    }
    this->auxData = new SceneObject(auxData);
}

void Sprite::updateBuffers() {
    for (auto it = textures.begin(); it != textures.end(); ++it) {
        auto spec = *it;
        float data[] = {
            spec.u1, spec.v1,
            spec.u1, spec.v2,
            spec.u2, spec.v1,
            spec.u2, spec.v2
        };

        glBindBuffer(GL_ARRAY_BUFFER, spec.UVBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*8, &data, GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    needsBufferUpdates = false;
}
