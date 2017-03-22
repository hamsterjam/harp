#ifndef __GALAXY_FORT_SPRITE_H
#define __GALAXY_FORT_SPRITE_H

#include <vector>

class Texture;
class Shader;

// For now, each sprite only has one texture. In the future they could have
// normal maps, bump maps, whatever

class Sprite {
    private:
        friend Shader;

        struct texSpecifier {
            Texture* tex;

            // As these are passed straight to OpenGL, flip the y
            float u1, u2, v1, v2;

            const char* texUniform;
            const char* UVAttrib;
        };

        std::vector<texSpecifier> textures;

        int w, h;

        texSpecifier defaultSpec(Texture* tex);

    public:
        Sprite();
        Sprite(const char* filename);
        Sprite(Texture* tex);

        void addImage(const char* filename, const char* texUniform, const char* UVAttrib);
        void addTexture(Texture* tex, const char* texUniform, const char* UVAttrib);
};

#endif
