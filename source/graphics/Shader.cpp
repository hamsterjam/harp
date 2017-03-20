#include <GL/glew.h>
#include <SDL_opengl.h>

#include <cstdlib>

#include <iostream>

#include <graphics/Shader.h>

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
precision mediump float;

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
}

Shader::~Shader() {
    glDeleteProgram(programID);
}
