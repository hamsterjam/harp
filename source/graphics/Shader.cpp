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
    glGenBuffers(1, &vertPosVBO);
    glGenBuffers(1, &texCoordVBO);
}

Shader::~Shader() {
    glDeleteProgram(programID);

    glDeleteBuffers(1, &vertPosVBO);
    glDeleteBuffers(1, &texCoordVBO);
}

void Shader::draw(Sprite spr, int x, int y) {
    // Make sure we use the shader
    glUseProgram(programID);

    // First, let's fill our vertPos VBO
    int texWidth  = spr.w;
    int texHeight = spr.h;

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

    glBindBuffer(GL_ARRAY_BUFFER, vertPosVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, &pos, GL_STATIC_DRAW);

    // And then set the attribute
    GLint aVertPos  = glGetAttribLocation( programID, "aVertPos");
    if (aVertPos < 0)  {
        std::cerr << "Failed to find 'aVertPos'"  << std::endl;
        exit(1);
    }
    glEnableVertexAttribArray(aVertPos);
    glVertexAttribPointer(aVertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Now let's load the stuff from the textures
    for (auto it = spr.textures.begin(); it != spr.textures.end(); ++it) {
        auto spec = *it;

        // First fill the texCoordVBO
        float uv[] = {
            spec.u1, spec.v1,
            spec.u1, spec.v2,
            spec.u2, spec.v1,
            spec.u2, spec.v2
        };

        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*8, &uv, GL_STATIC_DRAW);

        // Set the attribute
        GLint UVAttribID = glGetAttribLocation(programID, spec.UVAttrib);
        if (UVAttribID < 0) {
            std::cerr << "Failed to find attribute '" << spec.UVAttrib << "'" << std::endl;
            exit(1);
        }
        glEnableVertexAttribArray(UVAttribID);
        glVertexAttribPointer(UVAttribID, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Now set the texture to the uniform
        GLint texUniformID = glGetUniformLocation(programID, spec.texUniform);
        if (texUniformID < 0) {
            std::cerr << "Failed to find uniform '" << spec.texUniform << "'" << std::endl;
            exit(1);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, spec.tex->textureID);
        glUniform1i(texUniformID, 0);
    }

    // Draw
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
