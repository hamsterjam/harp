/*
 * HARP
 * graphics/Sprite.h
 *
 * This is the abstraction of a image that items will actually draw to the
 * screen. It contains a set of Texture Specifiers that each store a pointer to
 * a Texture and some UV coordinates so you can crop it to a subimage. A Sprite
 * can hold multiple of these as a Shader could have multiple samplers per
 * draw. It also contains a SceneObject so you can set uniforms from here also.
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_SPRITE_H
#define HARP_SPRITE_H

#include <vector>

#include <SDL_opengl.h>

class Texture;
class Shader;
class SceneObject;

extern const char* DEFAULT_TEXTURE_UNIFORM;
extern const char* DEFAULT_UV_ATTRIBUTE;

class Sprite {
    private:
        friend Shader;

        struct texSpecifier {
            Texture* tex;

            // As these are passed straight to OpenGL, flip the y
            GLfloat u1, u2, v1, v2;

            const char* texUniform;
            const char* UVAttrib;

            GLuint UVBuffer;
        };

        int w, h;
        bool needsBufferUpdates;

        SceneObject* auxData;

        std::vector<texSpecifier> textures;

        texSpecifier defaultSpec(Texture* tex);

    public:
        Sprite();
        Sprite(const char* filename);
        Sprite(Texture* tex);
        // Sub image constructors
        Sprite(const char* filename, unsigned int x, unsigned int y, int w, int h);
        Sprite(Texture* tex, unsigned int x, unsigned int y, int w, int h);

        ~Sprite();

        int getWidth();
        int getHeight();

        void addImage(const char* filename);
        void addImage(const char* filename, const char* texUniform, const char* UVAttrib);

        void addTexture(Texture* tex);
        void addTexture(Texture* tex, const char* texUniform, const char* UVAttrib);

        // Note that these explicitly allow negative widths and heights
        void addSubImage(const char* filename, unsigned int x, unsigned int y, int w, int h);
        void addSubImage(const char* filename, const char* texUniform, const char* UVAttrib,
                         unsigned int x, unsigned int y, int w, int h);

        void addSubTexture(Texture* tex, unsigned int x, unsigned int y, int w, int h);
        void addSubTexture(Texture* tex, const char* texUniform, const char* UVAttrib,
                           unsigned int x, unsigned int y, int w, int h);

        void setAuxData(SceneObject& auxData);

        void updateBuffers();
};

#endif
