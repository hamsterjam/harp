/*
 * HARP
 * graphics/VisualSpec.h
 *
 * This stores all the information you need to make a single draw call from
 * either a Shader, a PrimitiveRenderer, or a FontRenderer, and packages it all
 * together so you can store it in an ECS (allowing for a draw system)
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_VISUAL_SPEC_H
#define HARP_VISUAL_SPEC_H

#include <vector>
#include <graphics/Color.h>

class Shader;
class Sprite;
class PrimitiveRenderer;
class FontRenderer;

enum class DrawType{
    SPRITE,

    RECTANGLE,
    ROUNDED_RECTANGLE,
    ELIPSE,
    TRIANGLE,
    LINE,

    GLYPH,
    TEXT
};

struct PrimitiveSpec {
    PrimitiveRenderer* prim;
    float p1, p2, p3, p4, p5, p6, p7;
    Color color;
};

struct SpriteSpec {
    Shader* shd;
    Sprite* spr;
    float x, y;
};

struct TextSpec {
    FontRenderer* font;
    char glyph;
    const char* text;
    float x, y;
};

struct VisualSpec {
    DrawType type;
    float layer;
    union {
        PrimitiveSpec prim;
        SpriteSpec sprite;
        TextSpec text;
    };
};

bool operator<(const VisualSpec& lhs, const VisualSpec& rhs);

VisualSpec getSpriteSpec(Sprite& spr, float dx, float dy, float layer, Shader& shd);

VisualSpec getRectangleFillSpec(float dx, float dy, float w, float h, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getRectangleSpec(float dx, float dy, float w, float h, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getRoundedRectangleFillSpec(float dx, float dy, float w, float h, float r, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getRoundedRectangleSpec(float dx, float dy, float w, float h, float r, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);

VisualSpec getCircleFillSpec(float dx, float dy, float r, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getCircleSpec(float dx, float dy, float r, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getSegmentSpec(float dx, float dy, float r, float theta1, float theta2, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getArcSpec(float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);

VisualSpec getElipseFillSpec(float dx, float dy, float rx, float ry, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getElipseSpec(float dx, float dy, float rx, float ry, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getElipseSegmentSpec(float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getElipseArcSpec(float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);

VisualSpec getTriangleFillSpec(float x1, float y1, float x2, float y2, float x3, float y3, Color color, float layer, PrimitiveRenderer& prim);
VisualSpec getTriangleSpec(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);

VisualSpec getLineSpec(float x1, float y1, float x2, float y2, float lineWidth, Color color, float layer, PrimitiveRenderer& prim);

VisualSpec getGlyphSpec(char glyph, float dx, float dy, float layer, FontRenderer& font);
VisualSpec getTextSpec(const char* text, float dx, float dy, float layer, FontRenderer& font);

#endif
