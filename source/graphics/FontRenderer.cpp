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
        free(item.second);
    }
}

void FontRenderer::drawString(const char* string, int x, int y) {
    for (int i = 0; string[i] != 0; ++i) {
        Sprite* currSprite = spriteMap[string[i]];
        shd->batchQueue(*currSprite, x, y);
        x += currSprite->getWidth();
    }
    shd->batchDraw();
}
