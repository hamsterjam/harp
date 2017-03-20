#ifndef __GALAXY_FORT_SPRITE_H
#define __GALAXY_FORT_SPRITE_H

class Texture;

// For now, each sprite only has one texture. In the future they could have
// normal maps, bump maps, whatever

class Sprite {
    private:
        Texture* tex;
        // These are the values that OpenGL expects, the main difference is
        // they are measured from the LOWER left, you need to flip v
        double u1, u2, v1, v2;

    public:
        Sprite(const char* filename);
        Sprite(Texture* tex);
        Sprite(Texture* tex, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
};

#endif
