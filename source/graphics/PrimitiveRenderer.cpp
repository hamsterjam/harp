#include <graphics/PrimitiveRenderer.h>
#include <graphics/Shader.h>
#include <graphics/Color.h>
#include <globals.h>

PrimitiveRenderer::PrimitiveRenderer() : PrimitiveRenderer(*defaultPrimitiveShader) {
    // That's all
}

PrimitiveRenderer::PrimitiveRenderer(Shader& shd) :
    commonUniforms("allPrims"),
    elipseUniforms("elipse"),
    roundRectUniforms("roundRect")
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

// Rectangles

void PrimitiveRenderer::drawRectangleFill(float x, float y, float w, float h, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(0);
    shd->drawRectangle(x, y, w, h);
}

void PrimitiveRenderer::drawRectangle(float x, float y, float w, float h, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->drawRectangle(x, y, w, h);
}

// Triangles

void PrimitiveRenderer::drawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(0);
    shd->drawTriangle(x1, y1, x2, y2, x3, y3);
}

void PrimitiveRenderer::drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->drawTriangle(x1, y1, x2, y2, x3, y3);
}

// Lines

void PrimitiveRenderer::drawLine(float x1, float y1, float x2, float y2, float lineWidth, Color color) {
    setCommonSO(OTHER, color);
    shd->use(commonUniforms);

    shd->setLineWidth(lineWidth);
    shd->drawLine(x1, y1, x2, y2);
}

// Rounded Rectangles

void PrimitiveRenderer::drawRoundedRectangle(float x, float y, float w, float h, float r, float lineWidth, Color color) {
    setCommonSO(ROUND_RECT, color);

    GLfloat roundPoint1[] = {
        (GLfloat) (x + r),
        (GLfloat) (y + r)
    };
    roundRectUniforms.setUniform("uRoundPoint1", sizeof(GLfloat)*2, (void*) &roundPoint1);

    GLfloat roundPoint2[] = {
        (GLfloat) (x + w - r),
        (GLfloat) (y + h - r)
    };
    roundRectUniforms.setUniform("uRoundPoint2", sizeof(GLfloat)*2, (void*) &roundPoint2);

    GLfloat rr = (GLfloat) r;
    roundRectUniforms.setUniform("uRadius", sizeof(GLfloat), (void*) &rr);

    GLfloat wwidth = (GLfloat) lineWidth;
    roundRectUniforms.setUniform("uLineWidthRR", sizeof(GLfloat), (void*) &wwidth);

    shd->use(commonUniforms);
    shd->use(roundRectUniforms);

    shd->setLineWidth(0);
    shd->drawRectangle(x-lineWidth/2, y-lineWidth/2, w+lineWidth, h+lineWidth);
}

void PrimitiveRenderer::drawRoundedRectangleFill(float x, float y, float w, float h, float r, Color color) {
    drawRoundedRectangle(x, y, w, h, r, 0, color);
}

// Circles and Elipses (they are really the same things)

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
    elipseUniforms.setUniform("uLineWidthE", sizeof(GLfloat), (void*) &wwidth);

    shd->use(commonUniforms);
    shd->use(elipseUniforms);

    shd->setLineWidth(0);
    shd->drawRectangle(x-rx-lineWidth/2, y-ry-lineWidth/2, 2*rx+lineWidth, 2*ry+lineWidth);
}

// Circles
void PrimitiveRenderer::drawCircleFill(float x, float y, float r, Color color) {
    drawElipseArc(x, y, r, r, 0, 360, 0, color);
}
void PrimitiveRenderer::drawCircle(float x, float y, float r, float lineWidth, Color color) {
    drawElipseArc(x, y, r, r, 0, 360, lineWidth, color);
}
void PrimitiveRenderer::drawSegment(float x, float y, float r, float theta1, float theta2, Color color) {
    drawElipseArc(x, y, r, r, theta1, theta2, 0, color);
}
void PrimitiveRenderer::drawArc(float x, float y, float r, float theta1, float theta2, float lineWidth, Color color) {
    drawElipseArc(x, y, r, r, theta1, theta2, lineWidth, color);
}

// Elipses
void PrimitiveRenderer::drawElipseFill(float x, float y, float rx, float ry, Color color) {
    drawElipseArc(x, y, rx, ry, 0, 360, 0, color);
}
void PrimitiveRenderer::drawElipse(float x, float y, float rx, float ry, float lineWidth, Color color) {
    drawElipseArc(x, y, rx, ry, 0, 360, lineWidth, color);
}
void PrimitiveRenderer::drawElipseSegment(float x, float y, float rx, float ry, float theta1, float theta2, Color color) {
    drawElipseArc(x, y, rx, ry, theta1, theta2, 0, color);
}
