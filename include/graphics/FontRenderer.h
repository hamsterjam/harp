#ifndef HARP_FONT_MANAGER_H
#define HARP_FONT_MANAGER_H

#include <map>
#include <string>

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

        void drawText(std::string text, float x, float y);
        void drawGlyph(char glyph, float x, float y);
};

#endif
