#include <map>
#include <utility>

#include <graphics/FontManager.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Sprite.h>
#include <graphics/Shader.h>

// Might be a good idea to have a global default shader or something...

FontManager::FontManager(TextureAtlas& atlas, char firstChar, char lastChar) {
    for (int i = 0; i < lastChar - firstChar; ++i) {
        Sprite* spr = new Sprite();
        atlas.addTextureToSprite(*spr, i);
        spriteMap[firstChar + i] = spr;
    }
}

FontManager::FontManager(TextureAtlas& atlas, const char* charDef) {
    for (int i = 0; charDef[i] != 0; ++i) {
        Sprite* spr = new Sprite();
        atlas.addTextureToSprite(*spr, i);
        spriteMap[charDef[i]] = spr;
    }
}

FontManager::~FontManager() {
    for (auto item : spriteMap) {
        free(item.second);
    }
}

void FontManager::drawString(const char* string, int x, int y) {
    Shader shd;
    for (int i = 0; string[i] != 0; ++i) {
        Sprite* currSprite = spriteMap[string[i]];
        shd.batchQueue(*currSprite, x, y);
        x += currSprite->getWidth();
    }
    shd.batchDraw();
}
