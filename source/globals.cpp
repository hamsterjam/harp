#include <globals.h>
#include <graphics/Shader.h>

void initGlobals() {
    defaultShader = new Shader();
}

void cleanupGlobals() {
    delete defaultShader;
}

Shader* defaultShader = 0;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
