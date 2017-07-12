#ifndef HARP_GLOBALS_H
#define HARP_GLOBALS_H

#include <ECS.h> // For Component typedef

class ECS;
class Shader;

void initGlobals();
void cleanupGlobals();

extern ECS* harp;

extern Component comp_position;
extern Component comp_velocity;
extern Component comp_acceleration;

extern Component comp_hidden;
extern Component comp_sprite;
extern Component comp_visual;

extern Shader* defaultShader;
extern Shader* defaultPrimitiveShader;

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

#endif
