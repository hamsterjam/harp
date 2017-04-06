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
    uint uShape;
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

const uint OTHER = 0u;
const uint ELIPSE = 1u;
const uint ROUND_RECT = 2u;

void main(void) {
    switch (uShape) {
        case OTHER:
            gl_FragColor = uColor;
            break;
        case ELIPSE:
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
            break;
        case ROUND_RECT:
            gl_FragColor = vec4(0.0);
            break;
        default:
            gl_FragColor = vec4(0.0);
            break;
    }
}
)";

PrimitiveRenderer::PrimitiveRenderer() : PrimitiveRenderer(*defaultPrimitiveShader) {
    // That's all
}

PrimitiveRenderer::PrimitiveRenderer(Shader& shd) :
    commonUniforms("allPrims"),
    elipseUniforms("elipse")
{
    this->shd = &shd;
}

void PrimitiveRenderer::setCommonSO(Shape shape, Color color) {
    GLuint sshape = (GLuint) shape;
    commonUniforms.setUniform("uShape", sizeof(GLuint), (void*) &sshape);

    GLfloat colorArray[4] = {
        (GLfloat) color.r,
        (GLfloat) color.g,
        (GLfloat) color.b,
        (GLfloat) color.a
    };
    commonUniforms.setUniform("uColor", sizeof(GLfloat)*4, (void*) &colorArray);
}

void PrimitiveRenderer::drawRectangleFill(float x, float y, float w, float h, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setDrawMode(RECT_FILL);
    shd->drawRect(x, y, w, h);
}

void PrimitiveRenderer::drawRectangle(float x, float y, float w, float h, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->setDrawMode(RECT);
    shd->drawRect(x, y, w, h);
}

void PrimitiveRenderer::drawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setDrawMode(TRIANGLE_FILL);
    shd->drawTriangle(x1, y1, x2, y2, x3, y3);
}

void PrimitiveRenderer::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->setDrawMode(TRIANGLE);
    shd->drawTriangle(x1, y1, x2, y2, x3, y3);
}

void PrimitiveRenderer::drawLine(float x1, float y1, float x2, float y2, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->setDrawMode(LINE);
    shd->drawLine(x1, y1, x2, y2);
}

// All the circle/elipse drawing functions are really just specialisations of this
void PrimitiveRenderer::drawElipseArc(float x, float y, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    setCommonSO(ELIPSE, color);

    GLfloat center[] = {
        (GLfloat) x,
        (GLfloat) y
    };
    elipseUniforms.setUniform("uCenter", sizeof(GLfloat)*2, (void*) &center);

    GLfloat rrx = (GLfloat) rx;
    elipseUniforms.setUniform("uRadiusX", sizeof(GLfloat), (void*) &rrx);

    GLfloat rry = (GLfloat) ry;
    elipseUniforms.setUniform("uRadiusY", sizeof(GLfloat), (void*) &rry);

    GLfloat ttheta1 = (GLfloat) theta1;
    elipseUniforms.setUniform("uTheta1", sizeof(GLfloat), (void*) &ttheta1);

    GLfloat ttheta2 = (GLfloat) theta2;
    elipseUniforms.setUniform("uTheta2", sizeof(GLfloat), (void*) &ttheta2);

    GLfloat wwidth = (GLfloat) lineWidth;
    elipseUniforms.setUniform("uLineWidth", sizeof(GLfloat), (void*) &wwidth);

    shd->use(commonUniforms);
    shd->use(elipseUniforms);

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
