
#include <GL/glew.h>

#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>

#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <globals.h>

using namespace std;

const unsigned int FRAME_RATE = 60;

static bool shouldExit = false;

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

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

    SDL_GL_MakeCurrent(window, gl_context);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // The fun stuff starts here
    //

    Shader shd;
    Sprite spr("res/test.png");

    shd.draw(spr, 100, 100);

    //
    // And ends here...
    //

    SDL_GL_SwapWindow(window);

    while(!shouldExit) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                shouldExit = true;
            }
        }
    }

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
