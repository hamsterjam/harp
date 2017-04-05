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
    float uLineWidth;
};

void main(void) {
    if (uElipse) {
        vec2 trueRadial = gl_FragCoord.xy - uCenter;
        vec2 outerRadial = trueRadial / vec2(uRadiusX, uRadiusY);
        vec2 innerRadial = trueRadial / vec2(uRadiusX - uLineWidth, uRadiusY - uLineWidth);

        float angle = degrees(atan(trueRadial.y, trueRadial.x));
        angle = mod(angle, 360);

        bool rTest1 = dot(outerRadial, outerRadial) <= 1;
        bool rTest2 = dot(innerRadial, innerRadial) >= 1;
        bool aTest = angle >= uTheta1 && angle <= uTheta2;

        if (rTest1 && rTest2 && aTest) {
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

void PrimitiveRenderer::setElipseSO(float x, float y, float rx, float ry, float theta1, float theta2, float lineWidth) {
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

    GLfloat wwidth = (GLfloat) lineWidth;
    elipse.setUniform("uLineWidth", sizeof(GLfloat), (void*) &wwidth);
}

void PrimitiveRenderer::drawRectangleFill(float x, float y, float w, float h, Color color) {
    setAllPrimsSO(false, color);
    shd->use(allPrims);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x, y, w, h);
}

// All the circle/elipse drawing functions are really just specialisations of this
void PrimitiveRenderer::drawElipseArc(float x, float y, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    setAllPrimsSO(true, color);
    shd->use(allPrims);

    setElipseSO(x, y, rx, ry, theta1, theta2, lineWidth);
    shd->use(elipse);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x-rx-lineWidth/2, y-ry-lineWidth/2, 2*rx+lineWidth, 2*ry+lineWidth);
}

// Circles
void PrimitiveRenderer::drawCircleFill(float x, float y, float r, Color color) {
    drawElipseArc(x, y, r, r, 0, 360, r, color);
}
void PrimitiveRenderer::drawCircle(float x, float y, float r, float lineWidth, Color color) {
    drawElipseArc(x, y, r, r, 0, 360, lineWidth, color);
}
void PrimitiveRenderer::drawSegment(float x, float y, float r, float theta1, float theta2, Color color) {
    drawElipseArc(x, y, r, r, theta1, theta2, r, color);
}
void PrimitiveRenderer::drawArc(float x, float y, float r, float theta1, float theta2, float lineWidth, Color color) {
    drawElipseArc(x, y, r, r, theta1, theta2, lineWidth, color);
}

// Elipses
void PrimitiveRenderer::drawElipseFill(float x, float y, float rx, float ry, Color color) {
    drawElipseArc(x, y, rx, ry, 0, 360, (rx > ry) ? rx : ry, color);
}
void PrimitiveRenderer::drawElipse(float x, float y, float rx, float ry, float lineWidth, Color color) {
    drawElipseArc(x, y, rx, ry, 0, 360, lineWidth, color);
}
void PrimitiveRenderer::drawElipseSegment(float x, float y, float rx, float ry, float theta1, float theta2, Color color) {
    drawElipseArc(x, y, rx, ry, theta1, theta2, (rx > ry) ? rx : ry, color);
}
