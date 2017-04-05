#ifndef __GALAXY_FORT_PRIMITIVE_RENDERER_H
#define __GALAXY_FORT_PRIMITIVE_RENDERER_H

#include <graphics/SceneObject.h>

struct Color;
class Shader;

extern const char* defaultPrimitiveVertSource;
extern const char* defaultPrimitiveFragSource;

class PrimitiveRenderer {
    private:
        Shader* shd;
        SceneObject allPrims;
        SceneObject elipse;

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
