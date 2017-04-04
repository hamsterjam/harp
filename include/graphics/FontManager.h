#ifndef __GALAXY_FORT_FONT_MANAGER_H
#define __GALAXY_FORT_FONT_MANAGER_H

#include <map>

class TextureAtlas;
class Sprite;
class Shader;

class FontManager {
    private:
        std::map<char, Sprite*> spriteMap;

    public:
        FontManager(TextureAtlas& atlas, char firstChar, char lastChar);
        FontManager(TextureAtlas& atlas, const char* charDef);
        ~FontManager();

        void drawString(const char* string, int x, int y);
        void drawString(const char* string, int x, int y, Shader& shd);
};

#endif
