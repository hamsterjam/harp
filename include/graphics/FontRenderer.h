#ifndef __GALAXY_FORT_FONT_MANAGER_H
#define __GALAXY_FORT_FONT_MANAGER_H

#include <map>

class TextureAtlas;
class Sprite;
class Shader;

class FontRenderer {
    private:
        Shader* shd;
        std::map<char, Sprite*> spriteMap;

    public:
        // Default shader
        FontRenderer(TextureAtlas& atlas, char firstChar, char lastChar);
        FontRenderer(TextureAtlas& atlas, const char* charDef);

        // Non default shader
        FontRenderer(TextureAtlas& atlas, char firstChar, char lastChar, Shader& shd);
        FontRenderer(TextureAtlas& atlas, const char* charDef, Shader& shd);

        ~FontRenderer();

        void drawString(const char* string, float x, float y);
};

#endif
