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

        texSpecifier defaultSpec(Texture* tex);

    public:
        Sprite();
        Sprite(const char* filename);
        Sprite(Texture* tex);

        void addImage(const char* filename, const char* texUniform, const char* UVAttrib);
        void addTexture(Texture* tex, const char* texUniform, const char* UVAttrib);

        void updateBuffers();
};

#endif
