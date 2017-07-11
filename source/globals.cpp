#include <globals.h>
#include <ECS.h>
#include <harpMath.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/VisualSpec.h>

void initGlobals() {
    harp = new ECS(32, 8, 128);

    Component comp_position     = harp->createComponentType(sizeof(Vector<2, double>));
    Component comp_velocoty     = harp->createComponentType(sizeof(Vector<2, double>));
    Component comp_acceleration = harp->createComponentType(sizeof(Vector<2, double>));

    Component comp_sprite = harp->createComponentType(sizeof(Sprite*));
    Component comp_visual = harp->createComponentType(sizeof(VisualSpec));

    defaultShader = new Shader();
    defaultPrimitiveShader = new Shader(defaultPrimitiveVertSource, defaultPrimitiveFragSource);
}

void cleanupGlobals() {
    delete harp;

    delete defaultShader;
    delete defaultPrimitiveShader;
}

ECS* harp = 0;

Component comp_position = 0;
Component comp_velocity = 0;
Component comp_acceleration = 0;

Component comp_sprite = 0;
Component comp_visual = 0;

Shader* defaultShader = 0;
Shader* defaultPrimitiveShader = 0;

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
