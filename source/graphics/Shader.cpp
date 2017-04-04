#include <cstdlib>
#include <cstring> // memcpy

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL_opengl.h>

#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/Texture.h>
#include <graphics/SceneObject.h>

#include <globals.h>

static const char* defaultVertSource = R"(
attribute vec2 aVertPos;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main(void) {
    vTexCoord = aTexCoord;
    gl_Position = vec4(aVertPos, 0.0, 1.0);
}
)";

static const char* defaultFragSource = R"(
#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D uTexture;

varying vec2 vTexCoord;

void main(void) {
    gl_FragColor = texture2D(uTexture, vTexCoord);
}
)";

Shader::Shader() : Shader(defaultVertSource, defaultFragSource, 1) {
    // That's all
}

Shader::Shader(const char* vertSource, const char* fragSource, unsigned int numTextures) {
    this->numTextures = numTextures;

    currDrawMode = GF_FILL;
    lineWidth = 1;

    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vertSource, NULL);
    glShaderSource(fragShader, 1, &fragSource, NULL);

    GLint compileStatus;

    glCompileShader(vertShader);
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint logLength;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);

        GLchar* log = (GLchar*) malloc(sizeof(GLchar) * logLength);
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

        GLchar* log = (GLchar*) malloc(sizeof(GLchar) * logLength);
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
}

Shader::~Shader() {
    glDeleteProgram(programID);

    glDeleteBuffers(1, &vertPosBuffer);
}

void Shader::draw(Sprite& spr, int x, int y) {
    // Update the sprite if it needs it
    if (spr.needsBufferUpdates) {
        spr.updateBuffers();
    }

    // Grab the aux data if it exists
    if (spr.auxData) {
        sceneObjects.push_back(spr.auxData);
    }

    // Make sure we use the shader
    glUseProgram(programID);

    // First, let's fill our vertPos VBO
    int texWidth  = spr.w;
    int texHeight = spr.h;

    GLfloat x1 = (GLfloat) x / (GLfloat) SCREEN_WIDTH;
    GLfloat y1 = (GLfloat) y / (GLfloat) SCREEN_HEIGHT;
    GLfloat x2 = (GLfloat) (x + texWidth)  / (GLfloat) SCREEN_WIDTH;
    GLfloat y2 = (GLfloat) (y + texHeight) / (GLfloat) SCREEN_HEIGHT;

    x1 = x1 * 2.0 - 1.0;
    x2 = x2 * 2.0 - 1.0;
    y1 = y1 * 2.0 - 1.0;
    y2 = y2 * 2.0 - 1.0;

    // Note the order
    GLfloat pos[] = {
        x1, y1,
        x2, y2,
        x1, y2,
        x1, y1,
        x2, y1,
        x2, y2
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, &pos, GL_STATIC_DRAW);

    // And then set the attribute
    GLint aVertPos  = glGetAttribLocation( programID, "aVertPos");
    if (aVertPos < 0)  {
        std::cerr << "Failed to find 'aVertPos'"  << std::endl;
        exit(1);
    }
    glEnableVertexAttribArray(aVertPos);
    glVertexAttribPointer(aVertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Now let's load the stuff from the textures
    unsigned int currTex = 0;
    for (auto it = spr.textures.begin(); it != spr.textures.end(); ++it) {
        if (currTex >= numTextures) {
            std::cerr << "Too many textures in Sprite for this Shader" << std::endl;
            std::cerr << "Expected: " << numTextures << std::endl;
            exit(1);
        }
        auto spec = *it;

        // Now the UVs
        glBindBuffer(GL_ARRAY_BUFFER, spec.UVBuffer);

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

        glActiveTexture(GL_TEXTURE0 + currTex);
        glBindTexture(GL_TEXTURE_2D, spec.tex->textureID);
        glUniform1i(texUniformID, currTex);

        ++currTex;
    }

    if (currTex != numTextures) {
        std::cerr << "Too few textures in Sprite for this Shader." << std::endl;
        std::cerr << "Expected: " << numTextures << ", found: " << currTex << std::endl;
        exit(1);
    }

    // SceneObjects
    for (auto so : sceneObjects) {
        so->updateBuffer(*this);
    }
    sceneObjects.clear();

    // Draw
    switch (currDrawMode) {
        case GF_BOX:
            glLineWidth(lineWidth);
            glDrawArrays(GL_LINE_LOOP, 1, 4);
            break;
        case GF_LINE:
            glLineWidth(lineWidth);
            glDrawArrays(GL_LINES, 0, 2);
            break;
        case GF_FILL:
            glDrawArrays(GL_TRIANGLES, 0, 6);
            break;
    }

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Shader::use(SceneObject& so) {
    sceneObjects.push_back(&so);
}

void Shader::batchQueue(Sprite& spr, int x, int y) {
    batchSprites.push_back(&spr);
    Pos pos;
    pos.x = x;
    pos.y = y;
    batchPositions.push_back(pos);
}

void Shader::batchDraw() {
    // I'll just write this from scratch for now, It is probably possible to
    // consolidate it with draw()

    Sprite& first = *batchSprites.front();
    unsigned int numSprites = batchSprites.size();

    // Updating the sprites isn't nesecary

    // Check that the sprites actually work with this shader
    if (first.textures.size() != numTextures) {
        std::cerr << "Sprite has incorrect number of textures for this Shader" << std::endl;
        std::cerr << "Expected: " << numTextures << ". Found: " << first.textures.size() << std::endl;
        exit(1);
    }

    // Only use the aux data from the first sprite
    if (first.auxData) {
        sceneObjects.push_back(first.auxData);
    }

    glUseProgram(programID);

    // Build the vert pos VBO
    int texWidth  = first.w;
    int texHeight = first.h;

    GLfloat* posData = (GLfloat*) malloc(sizeof(GLfloat) * 12 * numSprites);

    unsigned int currSpr = 0;
    for (auto pos : batchPositions) {
        int x = pos.x;
        int y = pos.y;

        GLfloat x1 = (GLfloat) x / (GLfloat) SCREEN_WIDTH;
        GLfloat y1 = (GLfloat) y / (GLfloat) SCREEN_HEIGHT;
        GLfloat x2 = (GLfloat) (x + texWidth)  / (GLfloat) SCREEN_WIDTH;
        GLfloat y2 = (GLfloat) (y + texHeight) / (GLfloat) SCREEN_HEIGHT;

        x1 = x1 * 2.0 - 1.0;
        x2 = x2 * 2.0 - 1.0;
        y1 = y1 * 2.0 - 1.0;
        y2 = y2 * 2.0 - 1.0;

        GLfloat currPos[] = {
            x1, y1,
            x2, y2,
            x1, y2,

            x1, y1,
            x2, y1,
            x2, y2
        };

        memcpy(posData + currSpr * 12, &currPos, sizeof(GLfloat)*12);
        ++currSpr;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12*numSprites, posData, GL_DYNAMIC_DRAW);

    free(posData);

    GLint aVertPos = glGetAttribLocation(programID, "aVertPos");
    if (aVertPos < 0) {
        std::cerr << "Failed to find 'aVertPos'"  << std::endl;
        exit(1);
    }
    glEnableVertexAttribArray(aVertPos);
    glVertexAttribPointer(aVertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Now for UVs and textures

    // This array is like this:
    // [spr1_tex1, spr2_tex1, ... , sprN_tex1, spr1_tex2, spr2_tex2, ...]
    GLfloat* UVData = (GLfloat*) malloc(sizeof(GLfloat) * 12 * numSprites * numTextures);

    currSpr = 0;
    unsigned int currTex = 0;
    for (auto spr  : batchSprites) {
        currTex = 0;
        for (auto spec : spr->textures) {
            GLfloat currUV[] = {
                spec.u1, spec.v1,
                spec.u2, spec.v2,
                spec.u1, spec.v2,

                spec.u1, spec.v1,
                spec.u2, spec.v1,
                spec.u2, spec.v2
            };

            memcpy(UVData + currSpr*12 + currTex*numSprites*12, &currUV, sizeof(GLfloat)*12);

            ++currTex;
        }
        ++currSpr;
    }

    currTex = 0;
    for (auto spec : first.textures) {
        // UV
        glBindBuffer(GL_ARRAY_BUFFER, spec.UVBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12*numSprites, UVData + currTex*numSprites*12, GL_DYNAMIC_DRAW);

        GLint UVAttribID = glGetAttribLocation(programID, spec.UVAttrib);
        if (UVAttribID < 0) {
            std::cerr << "Failed to find attribute '" << spec.UVAttrib << "'" << std::endl;
            exit(1);
        }
        glEnableVertexAttribArray(UVAttribID);
        glVertexAttribPointer(UVAttribID, 2, GL_FLOAT, GL_FALSE, 0, 0);

        // Now the texture
        GLint texUniformID = glGetUniformLocation(programID, spec.texUniform);
        if (texUniformID < 0) {
            std::cerr << "Failed to find uniform '" << spec.texUniform << "'" << std::endl;
            exit(1);
        }

        glActiveTexture(GL_TEXTURE0 + currTex);
        glBindTexture(GL_TEXTURE_2D, spec.tex->textureID);
        glUniform1i(texUniformID, currTex);

        ++currTex;
    }

    free(UVData);

    // SceneObjects
    for (auto so : sceneObjects) {
        so->updateBuffer(*this);
    }
    sceneObjects.clear();

    // Draw, only fill mode is allowed
    glDrawArrays(GL_TRIANGLES, 0, 6*numSprites);

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    batchSprites.clear();
    batchPositions.clear();

    // We borrowed the buffers in first, so it needs an update
    first.needsBufferUpdates = true;
}

void Shader::setDrawMode(DrawMode mode) {
    currDrawMode = mode;
}

void Shader::setLineWidth(float width) {
    lineWidth = (GLfloat) width;
}

GLuint Shader::getProgramID() {
    return programID;
}
