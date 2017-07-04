#ifndef HARP_GLOBALS_H
#define HARP_GLOBALS_H

class Shader;

void initGlobals();
void cleanupGlobals();

extern Shader* defaultShader;
extern Shader* defaultPrimitiveShader;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

#endif
