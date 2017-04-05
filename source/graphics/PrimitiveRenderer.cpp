#include <graphics/PrimitiveRenderer.h>
#include <graphics/Shader.h>
#include <graphics/Color.h>
#include <globals.h>

const char* defaultPrimitiveVertSource = R"(
attribute vec2 aVertPos;

void main(void) {
    gl_Position = vec4(aVertPos, 0.0, 1.0);
}
)";

const char* defaultPrimitiveFragSource = R"(
#version 130
#extension GL_ARB_uniform_buffer_object : enable

#ifdef GL_ES
precision mediump float;
#endif

uniform allPrims {
    bool uElipse;
    vec4 uColor;
};

uniform elipse {
    float uRadius;
    vec2 uCenter;
};

void main(void) {
    if (uElipse) {
        vec2 radial = gl_FragCoord.xy - uCenter;

        if (dot(radial, radial) <= uRadius*uRadius) {
            gl_FragColor = uColor;
        }
        else {
            gl_FragColor = vec4(0.0);
        }
    }
    else {
        gl_FragColor = uColor;
    }
}
)";

PrimitiveRenderer::PrimitiveRenderer() : PrimitiveRenderer(*defaultPrimitiveShader) {
    // That's all
}

PrimitiveRenderer::PrimitiveRenderer(Shader& shd) :
    allPrims("allPrims"),
    elipse("elipse")
{
    this->shd = &shd;
}

void PrimitiveRenderer::setAllPrims(bool elipse, Color color) {
    GLboolean isElipse = (GLboolean) elipse;
    allPrims.setUniform("uElipse", sizeof(GLboolean), (void*) &isElipse);

    GLfloat colorArray[4] = {
        (GLfloat) color.r,
        (GLfloat) color.g,
        (GLfloat) color.b,
        (GLfloat) color.a
    };
    allPrims.setUniform("uColor", sizeof(GLfloat)*4, (void*) &colorArray);
}

void PrimitiveRenderer::drawRectangleFill(float x, float y, float w, float h, Color color) {
    setAllPrims(false, color);

    shd->use(allPrims);
    shd->drawRect(x, y, w, h);
}

void PrimitiveRenderer::drawCircleFill(float x, float y, float r, Color color) {
    setAllPrims(true, color);

    GLfloat center[] = {
        (GLfloat) x,
        (GLfloat) y
    };
    elipse.setUniform("uCenter", sizeof(GLfloat)*2, (void*) &center);

    GLfloat rr = (GLfloat) r;
    elipse.setUniform("uRadius", sizeof(GLfloat), (void*) &rr);

    shd->use(allPrims);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-r, y-r, 2*r, 2*r);
}
