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
    vec2 uCenter;
    float uRadiusX;
    float uRadiusY;
    float uTheta1;
    float uTheta2;
};

void main(void) {
    if (uElipse) {
        vec2 trueRadial = gl_FragCoord.xy - uCenter;
        vec2 scaleRadial = trueRadial / vec2(uRadiusX, uRadiusY);

        float angle = degrees(atan(trueRadial.y, trueRadial.x));
        angle = mod(angle, 360);

        bool rTest = dot(scaleRadial, scaleRadial) <= 1;
        bool aTest = angle >= uTheta1 && angle <= uTheta2;

        if (rTest && aTest) {
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

void PrimitiveRenderer::setAllPrimsSO(bool elipse, Color color) {
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

void PrimitiveRenderer::setElipseSO(float x, float y, float rx, float ry, float theta1, float theta2) {
    GLfloat center[] = {
        (GLfloat) x,
        (GLfloat) y
    };
    elipse.setUniform("uCenter", sizeof(GLfloat)*2, (void*) &center);

    GLfloat rrx = (GLfloat) rx;
    elipse.setUniform("uRadiusX", sizeof(GLfloat), (void*) &rrx);

    GLfloat rry = (GLfloat) ry;
    elipse.setUniform("uRadiusY", sizeof(GLfloat), (void*) &rry);

    GLfloat ttheta1 = (GLfloat) theta1;
    elipse.setUniform("uTheta1", sizeof(GLfloat), (void*) &ttheta1);

    GLfloat ttheta2 = (GLfloat) theta2;
    elipse.setUniform("uTheta2", sizeof(GLfloat), (void*) &ttheta2);
}

void PrimitiveRenderer::drawRectangleFill(float x, float y, float w, float h, Color color) {
    setAllPrimsSO(false, color);
    shd->use(allPrims);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x, y, w, h);
}

void PrimitiveRenderer::drawCircleFill(float x, float y, float r, Color color) {
    setAllPrimsSO(true, color);
    shd->use(allPrims);

    setElipseSO(x, y, r, r, 0, 360);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-r, y-r, 2*r, 2*r);
}

void PrimitiveRenderer::drawSegment(float x, float y, float r, float theta1, float theta2, Color color) {
    setAllPrimsSO(true, color);
    shd->use(allPrims);

    setElipseSO(x, y, r, r, theta1, theta2);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-r, y-r, 2*r, 2*r);
}

void PrimitiveRenderer::drawElipseFill(float x, float y, float rx, float ry, Color color) {
    setAllPrimsSO(true, color);
    shd->use(allPrims);

    setElipseSO(x, y, rx, ry, 0, 360);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-rx, y-ry, 2*rx, 2*ry);
}

void PrimitiveRenderer::drawElipseSegment(float x, float y, float rx, float ry, float theta1, float theta2, Color color) {
    setAllPrimsSO(true, color);
    shd->use(allPrims);

    setElipseSO(x, y, rx, ry, theta1, theta2);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-rx, y-ry, 2*rx, 2*ry);
}

