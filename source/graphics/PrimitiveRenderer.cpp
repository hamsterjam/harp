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

uniform elipse{
    vec2 uCenter;
    float uRadiusX;
    float uRadiusY;
    float uLineWidth;
    float uTheta1;
    float uTheta2;
};

void main(void) {
    if (uElipse) {
        vec4 trueRadial = gl_FragCoord - vec4(uCenter, 0.0, 1.0);

        vec4 outerRadial = trueRadial / vec4(uRadiusX, uRadiusY, 0.0, 1.0);
        vec4 innerRadial = trueRadial / vec4(uRadiusX - uLineWidth, uRadiusY - uLineWidth, 0.0, 1.0);

        float outerRadius = dot(outerRadial, outerRadial);
        float innerRadius = dot(innerRadial, innerRadial);
        float angle = degrees(atan(trueRadial.y, trueRadial.x));

        if (outerRadius <= 1.0 && innerRadius >= 1.0 && angle >= uTheta1 && angle <= uTheta2) {
            gl_FragColor = uColor;
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
