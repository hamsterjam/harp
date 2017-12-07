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

extern "C" {
#include <lua.h>
}

#include <ECS.h> // For Component typedef

class ECS;
class Console;
class Shader;
class PrimitiveRenderer;
class FontRenderer;

void readConfig();
void initGlobals();
void cleanupGlobals();

extern lua_State* L;

extern ECS harp;

extern Component comp_position;
extern Component comp_velocity;
extern Component comp_acceleration;
extern Component comp_collider;
extern Component comp_visual;
extern Component comp_layer;
extern Component comp_inputFunction;

extern Component comp_nextPosition;
extern Component comp_partialStep;
extern Component comp_onSurface;

extern Component flag_hidden;
extern Component flag_static;

extern Component meta_luaRef;

extern Shader* defaultShader;
extern Shader* defaultPrimitiveShader;

extern PrimitiveRenderer* defaultPrim;
extern FontRenderer*      consoleFont;

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern bool shouldExit;

#endif
