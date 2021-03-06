#include <iostream>
#include <string>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <globals.h>
#include <harpMath.h>
#include <graphics/Texture.h>

#include <systems.h>
#include <graphics/VisualSpec.h>
#include <graphics/AnimationManager.h>
#include <Console.h>

using namespace std;

enum GameMode {
    GAME,
    CONSOLE,
};

const unsigned int FRAME_RATE = 60;

static GameMode mode = GAME;

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

void init() {
    initGlobals();
    harp.updateComponents();
}

void update(unsigned int deltaT) {
    Console::getInstance().update();

    system_zeroAcceleration(harp);
    harp.updateComponents();

    bool allFinished = false;
    while (!allFinished) {
        system_input(harp, mode == GAME);
        harp.updateComponents();

        system_dynamics(harp);
        harp.updateComponents();

        system_proposeKinematics(harp, deltaT);
        harp.updateComponents();

        system_collision(harp);
        harp.updateComponents();

        system_resolveKinematics(harp);
        harp.updateComponents();

        system_fudge(harp);
        harp.updateComponents();

        allFinished = true;
        for (auto it = harp.begin({comp_partialStep}); it != harp.end(); ++it) {
            Entity e = *it;
            auto partialStep = * (double *) harp.getComponent(e, comp_partialStep);
            if (partialStep != 0) {
                allFinished = false;
                break;
            }
        }
    }

    for (auto it = harp.begin({comp_partialStep}); it != harp.end(); ++it) {
        Entity e = *it;
        harp.removeComponent(e, comp_partialStep);
    }
    harp.updateComponents();

    // Update animations
    for (auto it = harp.begin({comp_visual}); it != harp.end(); ++it) {
        Entity e = *it;
        auto& vis = * (VisualSpec*) harp.getComponent(e, comp_visual);
        if (vis.type == DrawType::ANIMATION) {
            vis.anim.man.update(e, deltaT);
        }
    }
}

void draw() {
    SDL_GL_MakeCurrent(window, gl_context);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    system_draw(harp);

    SDL_GL_SwapWindow(window);
}

void cleanup() {
    destroyTextures();
    cleanupGlobals();
}

void gameModeEvent(SDL_Event e);
void consoleModeEvent(SDL_Event e);

void handleEvent(SDL_Event e) {
    if (e.type == SDL_QUIT) {
        shouldExit = true;
        return;
    }

    switch (mode) {
        case GAME:
            gameModeEvent(e);
            break;

        case CONSOLE:
            consoleModeEvent(e);
            break;
    }
}

void gameModeEvent(SDL_Event e) {
    switch (e.type) {
        case SDL_QUIT:
            shouldExit = true;
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {

                case SDLK_CARET:
                    if (SDL_GetModState() & KMOD_SHIFT) { // Bitwise and is intenitonal
                        Console::getInstance().toggle();
                        SDL_StartTextInput();
                        mode = CONSOLE;
                    }
                    break;

                case SDLK_ESCAPE:
                    shouldExit = true;
                    break;
            }
            break;
    }
}

void consoleModeEvent(SDL_Event e) {
    Console& console = Console::getInstance();
    switch (e.type) {
        case SDL_KEYDOWN:
            switch (e.key.keysym.sym) {
                case SDLK_CARET:
                    if (SDL_GetModState() & KMOD_SHIFT) { // Bitwise and is intentional
                        console.toggle();
                        SDL_StopTextInput();
                        mode = GAME;
                    }
                    break;

                case SDLK_BACKSPACE:
                    console.backspace();
                    break;

                case SDLK_RETURN:
                    console.process();
                    break;
            }
            break;

        case SDL_TEXTINPUT:
            console.appendToInput(e.text.text);
            break;
    }
}

int main(int argc, char** argv) {
    readConfig();

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    // Apparently SDL inits with text input active, We don't want that
    SDL_StopTextInput();
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);

    gl_context = SDL_GL_CreateContext(window);

    if (!gl_context) {
        cerr << "Failed to create OpenGL context :" << SDL_GetError() << endl;
        return 1;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        cerr << "Failed to initialize glew: " << glewGetErrorString(err) << endl;
        return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    init();

    unsigned int prevTime = 0;

    while(!shouldExit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) handleEvent(e);

        unsigned int currTime = SDL_GetTicks();
        if (prevTime == 0) {
            prevTime = currTime - 1000 / FRAME_RATE;
        }

        update(currTime - prevTime);

        draw();

        // Frame limiter
        while (SDL_GetTicks() - prevTime < 1000 / FRAME_RATE);

        // You might think my timing stuff is ordered weird, but it's
        // like this so update has an accurate deltaT
        prevTime = currTime;
    }

    cleanup();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
