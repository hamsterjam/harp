#include <cmath>

#include <graphics/Color.h>

using namespace std;

Color rgbaToColor(float r, float g, float b, float a) {
    Color ret;
    ret.r = r;
    ret.g = g;
    ret.b = b;
    ret.a = a;

    return ret;
}

Color rgbToColor(float r, float g, float b) {
    return rgbaToColor(r, g, b, 1.0);
}

void chromaSwitch(float hh, float c, float x, float* r, float* g, float* b) {
    switch ((int) hh) {
        case 0:
            *r = c;
            *g = x;
            *b = 0;
            break;
        case 1:
            *r = x;
            *g = c;
            *b = 0;
            break;
        case 2:
            *r = 0;
            *g = c;
            *b = x;
            break;
        case 3:
            *r = 0;
            *g = x;
            *b = c;
            break;
        case 4:
            *r = x;
            *g = 0;
            *b = c;
            break;
        case 5:
            *r = c;
            *g = 0;
            *b = x;
            break;
        default:
            *r = 0;
            *g = 0;
            *b = 0;
    }
}

Color hsvaToColor(float h, float s, float v, float a) {
    h = fmod(h, 360);
    float c = v * s;
    float hh = h / 60.0;
    float x = c * (1 - abs(fmod(hh,2) - 1));

    float r, g, b;
    chromaSwitch(hh, c, x, &r, &g, &b);

    float m = v - c;
    return rgbaToColor(r+m, g+m, b+m, a);
}

Color hsvToColor(float h, float s, float v) {
    return hsvaToColor(h, s, v, 1.0);
}

Color hslaToColor(float h, float s, float l, float a) {
    h = fmod(h,360);
    float c = (1 - abs(2*l - 1)) * s;
    float hh = h / 60.0;
    float x = c * (1 - abs(fmod(hh,2) - 1));

    float r, g, b;
    chromaSwitch(hh, c, x, &r, &g, &b);

    float m = l - c/2.0;
    return rgbaToColor(r+m, g+m, b+m, a);
}

Color hslToColor(float h, float s, float l) {
    return hslaToColor(h, s, l, 1.0);
}
