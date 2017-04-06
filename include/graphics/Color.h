#ifndef __GALAXY_FORT_COLOR_H
#define __GALAXY_FORT_COLOR_H

#include <SDL_opengl.h>

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
    GLfloat a;
};

Color rgbaToColor(float r, float g, float b, float a);
Color rgbToColor(float r, float g, float b);

// Hue is measured in degrees!
Color hsvaToColor(float h, float s, float v, float a);
Color hsvToColor(float h, float s, float v);

Color hslaToColor(float h, float s, float l, float a);
Color hslToColor(float h, float s, float l);

#endif
