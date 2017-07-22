/*
 * HARP
 * graphics/FontRenderer.h
 *
 * This is a class that stores a Sprite for each glyph in a font as well as a
 * Shader to draw those glyphs. It allows you to pass a string to it and it
 * will batch draw the glyphs with the proper order and spacing.
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

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

        void drawText(const char* text, float x, float y);
        void drawGlyph(char glyph, float x, float y);
};

#endif
