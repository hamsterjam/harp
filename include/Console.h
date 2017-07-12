#ifndef HARP_CONSOLE_H
#define HARP_CONSOLE_H

#include <string>

#include <ECS.h>
#include <graphics/VisualSpec.h>

class PrimitiveRenderer;
class FontRenderer;

class Console {
    private:
        Entity id;
        std::string inputBuffer;
        VisualSpec spec;

        bool open;

    public:
        Console(PrimitiveRenderer& prim, FontRenderer& font);
        ~Console();

        void toggle();
        void update();

        void append(std::string text);
        void backspace();

        void process();
};

#endif
