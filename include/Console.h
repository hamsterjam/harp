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

extern "C" {
#include <lua.h>
}

#include <ECS.h>
#include <graphics/VisualSpec.h>

class PrimitiveRenderer;
class FontRenderer;

const unsigned int logLines = 35;

class Console {
    private:
        static Console* instance;

        lua_State* L;
        bool open;

        std::string inputBuffer;
        std::string logBuffer[logLines];

        Entity id;
        Entity inputBoxID;
        Entity inputID;
        Entity logLineID[logLines];

        Console(PrimitiveRenderer& prim, FontRenderer& font);
        ~Console();

    public:
        static void init(PrimitiveRenderer& prim, FontRenderer& font);
        static void cleanup();
        static Console& getInstance();

        Console(Console const&)        = delete;
        void operator=(Console const&) = delete;

        void toggle();
        void update();

        void log(std::string message);
        void process();

        void appendToInput(std::string text);
        void backspace();
};

#endif
