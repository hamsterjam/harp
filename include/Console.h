/*
 * HARP
 * Console.h
 *
 * This is a console like the ones you find in a lot of games when you hit the
 * back tick (`) key. It doesn't do anything besides a really ghetto exit
 * function for now.
 *
 */

#ifndef HARP_CONSOLE_H
#define HARP_CONSOLE_H

#include <string>

#include <ECS.h>
#include <graphics/VisualSpec.h>

class PrimitiveRenderer;
class FontRenderer;

const unsigned int logLines = 35;

class Console {
    private:
        Entity id;

        Entity inputBoxID;

        Entity inputID;
        Entity logLineID[logLines];

        std::string inputBuffer;
        std::string logBuffer[logLines];

        bool open;

    public:
        Console(PrimitiveRenderer& prim, FontRenderer& font);
        ~Console();

        void toggle();
        void update();

        void log(std::string message);
        void process();

        void appendToInput(std::string text);
        void backspace();
};

#endif
