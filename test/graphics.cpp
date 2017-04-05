#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <globals.h>
#include <graphics/Texture.h>
#include <graphics/TextureAtlas.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/SceneObject.h>
#include <graphics/FontRenderer.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/Color.h>

using namespace std;

const unsigned int FRAME_RATE = 60;

static bool shouldExit = false;

static SDL_Window *window = NULL;
static SDL_GLContext gl_context;

const char* vertSource = R"(
attribute vec2 aVertPos;
attribute vec2 aTexCoord;
attribute vec2 aAuxTexCoord;

varying vec2 vTexCoord;
varying vec2 vAuxTexCoord;

void main(void) {
    vTexCoord = aTexCoord;
    vAuxTexCoord = aAuxTexCoord;
    gl_Position = vec4(aVertPos, 0.0, 1.0);
}
)";

const char* fragSource = R"(
#version 130
#extension GL_ARB_uniform_buffer_object : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D uTexture;
uniform sampler2D uTexture2;

uniform mixRatios {
    float uMix1;
    float uMix2;
};

varying vec2 vTexCoord;
varying vec2 vAuxTexCoord;

void main(void) {
    vec4 color1 = texture2D(uTexture, vTexCoord);
    vec4 color2 = texture2D(uTexture2, vAuxTexCoord);

    gl_FragColor = uMix1 * color1 + uMix2 * color2;
}
)";

int main(int argc, char** argv) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
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

    initGlobals();

    SDL_GL_MakeCurrent(window, gl_context);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //
    // The fun stuff starts here
    //

    {
        Shader shd(vertSource, fragSource, 2);

        Sprite spr("res/test.png", 64, 64, 128, 128);
        spr.addSubImage("res/crane.png", "uTexture2", "aAuxTexCoord", 64, 64, 128, 128);

        SceneObject so("mixRatios");
        GLfloat mix1 = 0.5;
        GLfloat mix2 = 0.5;
        so.setUniform("uMix1", sizeof(GLfloat), (void*) &mix1);
        so.setUniform("uMix2", sizeof(GLfloat), (void*) &mix2);

        spr.setAuxData(so);

        shd.drawSprite(spr, 100, 100);

        // Testing fonts
        TextureAtlas fontAtlas("res/testfont.png", 8, 12, 0, 0);
        FontRenderer testFont(fontAtlas, ' ', '~');

        testFont.drawString("Princess Luna sees your lack of progress!", 132, 132);

        // Testing primitives
        PrimitiveRenderer prim;
        //prim.drawRectangleFill(200, 200, 50, 50, rgbToColor(0.1, 0.8, 0.8));
        prim.drawCircleFill(400, 100, 50, rgbToColor(0.2, 0.2, 0.9));
    }

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

    destroyTextures();
    cleanupGlobals();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
