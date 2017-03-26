#ifndef __GALAXY_FORT_SPRITE_H
#define __GALAXY_FORT_SPRITE_H

#include <vector>

#include <SDL_opengl.h>

class Texture;
class Shader;

class Sprite {
    private:
        friend Shader;

        struct texSpecifier {
            Texture* tex;

            // As these are passed straight to OpenGL, flip the y
            float u1, u2, v1, v2;

            const char* texUniform;
            const char* UVAttrib;

            GLuint UVBuffer;
        };

        int w, h;
        std::vector<texSpecifier> textures;

        bool needsBufferUpdates;

        texSpecifier defaultSpec(Texture* tex);

    public:
        Sprite();
        Sprite(const char* filename);
        Sprite(Texture* tex);
        // Sub image constructors
        Sprite(const char* filename, unsigned int x, unsigned int y, int w, int h);
        Sprite(Texture* tex, unsigned int x, unsigned int y, int w, int h);

        void addImage(const char* filename, const char* texUniform, const char* UVAttrib);
        void addTexture(Texture* tex, const char* texUniform, const char* UVAttrib);

        // Note that these explicitly allow negative widths and heights
        void addSubImage(const char* filename, const char* texUniform, const char* UVAttrib,
                         unsigned int x, unsigned int y, int w, int h);

        void addSubTexture(Texture* tex, const char* texUniform, const char* UVAttrib,
                           unsigned int x, unsigned int y, int w, int h);

        void updateBuffers();
};

#endif
