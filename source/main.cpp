#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <globals.h>
#include <graphics/Texture.h>

using namespace std;

const unsigned int FRAME_RATE = 60;

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

void init() {
    initGlobals();
}

void update(unsigned int deltaT) {
}

void draw() {
    SDL_GL_MakeCurrent(window, gl_context);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}

void cleanup() {
    destroyTextures();
    cleanupGlobals();
}

int main(int argc, char** argv) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

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
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                shouldExit = true;
            }
        }

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
