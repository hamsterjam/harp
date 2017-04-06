#include <globals.h>
#include <graphics/Shader.h>
#include <graphics/PrimitiveRenderer.h>

void initGlobals() {
    defaultShader = new Shader();
    defaultPrimitiveShader = new Shader(defaultPrimitiveVertSource, defaultPrimitiveFragSource, 0);
}

void cleanupGlobals() {
    delete defaultShader;
    delete defaultPrimitiveShader;
}

Shader* defaultShader = 0;
Shader* defaultPrimitiveShader = 0;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
