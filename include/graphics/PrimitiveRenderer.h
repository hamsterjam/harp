/*
 * HARP
 * graphics/PrimitiveRenderer.h
 *
 * A class for drawing primitives to the screen. This requires a Shader with
 * more uniforms than defaultShader so it provides its own default Shader. Here
 * is a list of all the attributes and uniforms you need:
 *
 * Vertex Shader:
 *     attribute vec2 aVertPos
 *
 * Fragment Shader:
 *     uniform allPrims {
 *         uint uShape
 *         vec4 uColor
 *     }
 *     uniform elipse {
 *         vec2  uCenter
 *         float uRadiusX
 *         float uRadiusY
 *         float uTheta1
 *         float uTheta2
 *         float uLineWidthE
 *     }
 *     uniform roundRect {
 *         vec2  uRoundPoint1
 *         vec2  uRoundPoint2
 *         float uRadius
 *         float uLineWidthRR
 *     }
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_PRIMITIVE_RENDERER_H
#define HARP_PRIMITIVE_RENDERER_H

#include <graphics/SceneObject.h>

struct Color;
class Shader;

extern const char* defaultPrimitiveVertSource;
extern const char* defaultPrimitiveFragSource;

class PrimitiveRenderer {
    private:
        enum Shape {
            OTHER      = 0,
            ELIPSE     = 1,
            ROUND_RECT = 2
        };

        Shader* shd;
        SceneObject commonUniforms;
        SceneObject elipseUniforms;
        SceneObject roundRectUniforms;

        void setCommonSO(Shape shape, Color color);

    public:
        PrimitiveRenderer();
        PrimitiveRenderer(Shader& shd);

        void drawRectangleFill(float x, float y, float w, float h, Color color);
        void drawRectangle(float x, float y, float w, float h, float lineWidth, Color color);
        void drawRoundedRectangleFill(float x, float y, float w, float h, float r, Color color);
        void drawRoundedRectangle(float x, float y, float w, float h, float r, float lineWidth, Color color);

        void drawCircleFill(float x, float y, float r, Color color);
        void drawCircle(float x, float y, float r, float lineWidth, Color color);
        void drawSegment(float x, float y, float r, float theta1, float theta2, Color color);
        void drawArc(float x, float y, float r, float theta1, float theta2, float lineWidth, Color color);

        void drawElipseFill(float x, float y, float rx, float ry, Color color);
        void drawElipse(float x, float y, float rx, float ry, float lineWidth, Color color);
        void drawElipseSegment(float x, float y, float rx, float ry, float theta1, float theta2, Color color);
        void drawElipseArc(float x, float y, float rx, float ry, float theta1, float theta2, float lineWidth, Color color);

        void drawTriangleFill(float x1, float y1, float x2, float y2, float x3, float y3, Color color);
        void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color);

        void drawLine(float x1, float y1, float x2, float y2, float lineWidth, Color color);
};

#endif
