#include <GL/glew.h>
#include <SDL_opengl.h>

#include <cstdlib>

#include <iostream>

#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/Texture.h>

#include <globals.h>

const char* defaultVertSource = R"(
attribute vec2 aVertPos;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main(void) {
    vTexCoord = aTexCoord;
    gl_Position = vec4(aVertPos, 0.0, 1.0);
}
)";

const char* defaultFragSource = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D uTexture;

varying vec2 vTexCoord;

void main(void) {
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
)";

Shader::Shader() {
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &defaultVertSource, NULL);
    glShaderSource(fragShader, 1, &defaultFragSource, NULL);

    GLint compileStatus;

    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = (GLchar*) malloc(sizeof(GLchar) * (logLength + 1));
        glGetShaderInfoLog(fragShader, logLength, NULL, log);

        std::cerr << "Failed to compile vertex shader: " << log << std::endl;
        free(log);
        exit(1);
    }

    glCompileShader(fragShader);
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = (GLchar*) malloc(sizeof(GLchar) * (logLength + 1));
        glGetShaderInfoLog(fragShader, logLength, NULL, log);

        std::cerr << "Failed to compile fragment shader: " << log << std::endl;
        free(log);
        exit(1);
    }

    programID = glCreateProgram();
    glAttachShader(programID, vertShader);
    glAttachShader(programID, fragShader);

    glLinkProgram(programID);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_FALSE) {
        GLint logLength;
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = (GLchar*) malloc(sizeof(GLchar) * (logLength + 1));
        glGetProgramInfoLog(programID, logLength, NULL, log);

        std::cerr << "Failed to link shader program: " << log << std::endl;
        free(log);
        exit(1);
    }

    // Make some VBO's for pos coords and texture coords
    glGenBuffers(1, &vertPosBuffer);
    glGenBuffers(1, &texCoordBuffer);
}

Shader::~Shader() {
    glDeleteProgram(programID);

    glDeleteBuffers(1, &vertPosBuffer);
    glDeleteBuffers(1, &texCoordBuffer);
}

void Shader::draw(Sprite spr, int x, int y) {
    int texWidth  = spr.tex->w;
    int texHeight = spr.tex->h;

    // First fill our VBOs
    float x1 = (float) x / (float) SCREEN_WIDTH;
    float y1 = (float) y / (float) SCREEN_HEIGHT;
    float x2 = (float) (x + texWidth)  / (float) SCREEN_WIDTH;
    float y2 = (float) (y + texHeight) / (float) SCREEN_HEIGHT;

    x1 = x1 * 2.0 - 1.0;
    x2 = x2 * 2.0 - 1.0;
    y1 = y1 * 2.0 - 1.0;
    y2 = y2 * 2.0 - 1.0;

    // Note the order
    float pos[] = {
        x1, y1,
        x1, y2,
        x2, y1,
        x2, y2
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, &pos, GL_STATIC_DRAW);

    float u1 = spr.u1;
    float v1 = spr.v1;
    float u2 = spr.u2;
    float v2 = spr.v2;

    float texCoord[] = {
        u1, v1,
        u1, v2,
        u2, v1,
        u2, v2
    };

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, &texCoord, GL_STATIC_DRAW);

    // Don't forget this...
    glUseProgram(programID);

    // Get our attributes and uniforms
    GLint aVertPos  = glGetAttribLocation( programID, "aVertPos");
    GLint aTexCoord = glGetAttribLocation( programID, "aTexCoord");
    GLint uTexture  = glGetUniformLocation(programID, "uTexture");

    if (aVertPos < 0)  std::cerr << "Failed to find 'aVertPos'"  << std::endl;
    if (aTexCoord < 0) std::cerr << "Failed to find 'aTexCoord'" << std::endl;
    if (uTexture < 0)  std::cerr << "Failed to find 'uTexture'"  << std::endl;

    // Activate
    glEnableVertexAttribArray(aVertPos);
    glEnableVertexAttribArray(aTexCoord);

    // Bind attributes
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glVertexAttribPointer(aVertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glVertexAttribPointer(aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Bind the texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spr.tex->textureID);
    glUniform1i(uTexture, 1);

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
