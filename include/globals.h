#ifndef __GALAXY_FORT_GLOBALS_H
#define __GALAXY_FORT_GLOBALS_H

class Shader;

void initGlobals();
void cleanupGlobals();

extern Shader* defaultShader;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

#endif
