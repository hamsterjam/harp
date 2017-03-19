#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

using namespace std;

const unsigned int FRAME_RATE = 120;

static bool shouldExit = false;

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

void update(unsigned int deltaT) {
    cout << "Frame time: " << deltaT << endl;
}

void draw() {
    SDL_GL_MakeCurrent(window, gl_context);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    SDL_GL_SwapWindow(window);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cerr << "Failed to initialize SDL: " << SDL_GetError() << endl;
        return 1;
    }

    window = SDL_CreateWindow("test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL);

    gl_context = SDL_GL_CreateContext(window);

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

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
