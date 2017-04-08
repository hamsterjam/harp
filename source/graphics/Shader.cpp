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

Shader::Shader() : Shader(defaultVertSource, defaultFragSource) {
    // That's all
}

Shader::Shader(const char* vertSource, const char* fragSource) {
    lineWidth = 0;

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

void toInternalCoord(GLfloat& x, GLfloat& y) {
    x = x / (GLfloat) SCREEN_WIDTH;
    y = y / (GLfloat) SCREEN_HEIGHT;

    x = x * 2.0 - 1.0;
    y = y * 2.0 - 1.0;
}

void fillRectDataArray(float x1, float y1, float x2, float y2, GLfloat* data) {
    GLfloat xx1 = (GLfloat) x1;
    GLfloat xx2 = (GLfloat) x2;
    GLfloat yy1 = (GLfloat) y1;
    GLfloat yy2 = (GLfloat) y2;

    toInternalCoord(xx1, yy1);
    toInternalCoord(xx2, yy2);

    GLfloat newData[] = {
        xx1, yy1,
        xx2, yy2,
        xx1, yy2,

        xx1, yy1,
        xx2, yy1,
        xx2, yy2
    };

    memcpy(data, &newData, sizeof(GLfloat) * 12);
}

void Shader::bindTextures(Sprite& spr) {
    unsigned int currTex = 0;
    for (auto spec : spr.textures) {

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
}

void Shader::draw(DrawMode mode, unsigned int numElements) {
    glUseProgram(programID);

    // Set aVertPos
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    GLint aVertPos  = glGetAttribLocation(programID, "aVertPos");
    if (aVertPos < 0)  {
        std::cerr << "Failed to find 'aVertPos'" << std::endl;
        exit(1);
    }
    glEnableVertexAttribArray(aVertPos);
    glVertexAttribPointer(aVertPos, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // SceneObjects
    GLuint bindingPoint = 0;
    for (auto so : sceneObjects) {
        so->updateBuffer(*this, bindingPoint);
        ++bindingPoint;
    }
    sceneObjects.clear();

    if (lineWidth == 0) {
        mode = (DrawMode) (mode | FILL);
    }
    else {
        mode = (DrawMode) (mode | OUTLINE);
    }

    // Draw
    switch (mode) {
        case (RECT | FILL):
            glDrawArrays(GL_TRIANGLES, 0, 6*numElements);
            break;
        case (RECT | OUTLINE):
            glLineWidth(lineWidth);
            glDrawArrays(GL_LINE_LOOP, 1, 4);
            break;
        case (TRI | FILL):
            glDrawArrays(GL_TRIANGLES, 0, 3*numElements);
            break;
        case (TRI | OUTLINE):
            glLineWidth(lineWidth);
            glDrawArrays(GL_LINE_LOOP, 0, 3*numElements);
            break;
        case (LINE | OUTLINE):
            glLineWidth(lineWidth);
            glDrawArrays(GL_LINES, 0, 2*numElements);
            break;
        case POINT:
            glDrawArrays(GL_POINTS, 0, 1*numElements);
    }

    // Cleanup
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Shader::drawSprite(Sprite& spr, float x, float y) {
    // Update the sprite if it needs it
    if (spr.needsBufferUpdates) {
        spr.updateBuffers();
    }

    // Grab the aux data if it exists
    if (spr.auxData) {
        sceneObjects.push_back(spr.auxData);
    }

    bindTextures(spr);

    drawRectangle(x, y, spr.w, spr.h);
}

void Shader::drawRectangle(float x, float y, float w, float h) {
    // I shouldn't need a malloc here... but decalring it on the stack wasn't working
    GLfloat* pos = (GLfloat*) malloc(sizeof(GLfloat) * 12);
    fillRectDataArray(x, y, x+w, y+h, pos);

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12, pos, GL_DYNAMIC_DRAW);

    free(pos);

    draw(RECT, 1);
}

void Shader::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3) {
    GLfloat xx1 = (GLfloat) x1;
    GLfloat xx2 = (GLfloat) x2;
    GLfloat xx3 = (GLfloat) x3;

    GLfloat yy1 = (GLfloat) y1;
    GLfloat yy2 = (GLfloat) y2;
    GLfloat yy3 = (GLfloat) y3;

    toInternalCoord(xx1, yy1);
    toInternalCoord(xx2, yy2);
    toInternalCoord(xx3, yy3);

    GLfloat pos[] = {
        xx1, yy1,
        xx2, yy2,
        xx3, yy3
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6, &pos, GL_DYNAMIC_DRAW);

    draw(TRI, 1);
}

void Shader::drawLine(float x1, float y1, float x2, float y2) {
    GLfloat xx1 = (GLfloat) x1;
    GLfloat xx2 = (GLfloat) x2;
    GLfloat yy1 = (GLfloat) y1;
    GLfloat yy2 = (GLfloat) y2;

    toInternalCoord(xx1, yy1);
    toInternalCoord(xx2, yy2);

    GLfloat pos[] = {
        xx1, yy1,
        xx2, yy2
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*4, &pos, GL_DYNAMIC_DRAW);

    draw(LINE, 1);
}

void Shader::drawPoint(float x, float y) {
    GLfloat xx = (GLfloat) x / (GLfloat) SCREEN_WIDTH;
    GLfloat yy = (GLfloat) y / (GLfloat) SCREEN_HEIGHT;

    toInternalCoord(xx, yy);

    GLfloat pos[] = {
        xx, yy
    };

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2, &pos, GL_DYNAMIC_DRAW);

    draw(POINT, 1);
}

void Shader::batchQueue(Sprite& spr, float x, float y) {
    batchSprites.push_back(&spr);
    Pos pos;
    pos.x = x;
    pos.y = y;
    batchPositions.push_back(pos);
}

void Shader::batchDraw() {
    Sprite& first = *batchSprites.front();
    unsigned int numSprites = batchSprites.size();
    unsigned int numTextures = first.textures.size();

    // Updating the sprites isn't nesecary
    // We aren't going to use the pre-buffered data anyway

    // Only use the aux data from the first sprite
    if (first.auxData) {
        sceneObjects.push_back(first.auxData);
    }

    // Build the vert pos VBO
    int texWidth  = first.w;
    int texHeight = first.h;

    GLfloat* posData = (GLfloat*) malloc(sizeof(GLfloat) * 12 * numSprites);

    unsigned int currSpr = 0;
    for (auto pos : batchPositions) {
        float x = pos.x;
        float y = pos.y;
        fillRectDataArray(x, y, x+texWidth, y+texHeight, posData + currSpr * 12);
        ++currSpr;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12*numSprites, posData, GL_DYNAMIC_DRAW);

    free(posData);

    // Now for UVs and textures

    // This array is like this:
    // [spr1_tex1, spr2_tex1, ... , sprN_tex1, spr1_tex2, spr2_tex2, ...]
    GLfloat* UVData = (GLfloat*) malloc(sizeof(GLfloat) * 12 * numSprites * numTextures);

    currSpr = 0;
    unsigned int currTex = 0;
    for (auto spr  : batchSprites) {
        currTex = 0;
        for (auto spec : spr->textures) {
            unsigned int offset = 0;

            offset += currTex;
            offset *= numSprites;

            offset += currSpr;
            offset *= 12;

            fillRectDataArray(spec.u1, spec.v1, spec.u2, spec.v2, UVData + offset);
            ++currTex;
        }
        ++currSpr;
    }

    // Buffer all this into first's buffers
    for (auto spec : first.textures) {
        glBindBuffer(GL_ARRAY_BUFFER, spec.UVBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*12*numSprites, UVData + currTex*numSprites*12, GL_DYNAMIC_DRAW);
    }

    free(UVData);
    bindTextures(first);

    draw(RECT, numSprites);

    // We borrowed the buffers in first, so it needs an update
    first.needsBufferUpdates = true;
}

void Shader::use(SceneObject& so) {
    sceneObjects.push_back(&so);
}

void Shader::setLineWidth(float width) {
    lineWidth = (GLfloat) width;
}

GLuint Shader::getProgramID() {
    return programID;
}
