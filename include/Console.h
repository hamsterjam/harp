#ifndef HARP_CONSOLE_H
#define HARP_CONSOLE_H

#include <ECS.h>
#include <graphics/VisualSpec.h>

class PrimitiveRenderer;
class FontRenderer;

class Console {
    private:
        Entity id;
        char* inputBuffer;
        VisualSpec spec;

    public:
        Console(PrimitiveRenderer& prim, FontRenderer& font);
        ~Console();
};

#endif
