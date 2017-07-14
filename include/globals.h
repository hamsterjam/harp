/*
 * HARP
 * globals.h
 *
 * This just defines some global variables, that's all
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_GLOBALS_H
#define HARP_GLOBALS_H

#include <ECS.h> // For Component typedef

class ECS;
class Console;
class Shader;
class PrimitiveRenderer;
class FontRenderer;

void initGlobals();
void cleanupGlobals();

extern ECS* harp;

extern Component comp_position;
extern Component comp_velocity;
extern Component comp_acceleration;
extern Component flag_hidden;
extern Component comp_sprite;
extern Component comp_visual;

extern Console* console;

extern Shader* defaultShader;
extern Shader* defaultPrimitiveShader;

extern PrimitiveRenderer* defaultPrim;
extern FontRenderer*      consoleFont;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

extern bool shouldExit;

#endif
