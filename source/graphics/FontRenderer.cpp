#include <map>
#include <utility>

#include <globals.h>
#include <graphics/FontRenderer.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Sprite.h>
#include <graphics/Shader.h>

FontRenderer::FontRenderer(TextureAtlas& atlas, char firstChar, char lastChar) :
    FontRenderer(atlas, firstChar, lastChar, *defaultShader)
{
    // That's all
}

FontRenderer::FontRenderer(TextureAtlas& atlas, const char* charDef) :
    FontRenderer(atlas, charDef, *defaultShader)
{
    // That's all
}

FontRenderer::FontRenderer(TextureAtlas& atlas, char firstChar, char lastChar, Shader& shd) {
    this->shd = &shd;
    for (int i = 0; i < lastChar - firstChar; ++i) {
        Sprite* spr = new Sprite();
        atlas.addTextureToSprite(*spr, i);
        spriteMap[firstChar + i] = spr;
    }
}

FontRenderer::FontRenderer(TextureAtlas& atlas, const char* charDef, Shader& shd) {
    this->shd = &shd;
    for (int i = 0; charDef[i] != 0; ++i) {
        Sprite* spr = new Sprite();
        atlas.addTextureToSprite(*spr, i);
        spriteMap[charDef[i]] = spr;
    }
}

FontRenderer::~FontRenderer() {
    for (auto item : spriteMap) {
        Sprite* spr = item.second;
        delete spr;
    }
}

void FontRenderer::drawText(const char* text, float x, float y) {
    if (!text[0]) return;

    for (int i = 0; text[i] != 0; ++i) {
        auto it = spriteMap.find(text[i]);
        if (it == spriteMap.end()) continue;

        Sprite* currSprite = (*it).second;
        shd->batchQueue(*currSprite, x, y);
        x += (float) currSprite->getWidth();
    }
    shd->batchDraw();

    /*
     * Changing back to C strings (so Lua works)
     *
    if (text.size() == 0) return;

    for (auto it = text.begin(); it != text.end(); ++it) {
        char nextGlyph = *it;
        Sprite* currSprite = spriteMap[nextGlyph];
        shd->batchQueue(*currSprite, x, y);
        x += (float) currSprite->getWidth();
    }
    shd->batchDraw();
    */
}

void FontRenderer::drawGlyph(char glyph, float x, float y) {
    Sprite* glyphSprite = spriteMap[glyph];
    shd->drawSprite(*glyphSprite, x, y);
}
