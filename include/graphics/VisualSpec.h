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
#include <string>
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

struct VisualSpec {
    // General Parameters
    DrawType type;
    float p1, p2, p3, p4, p5, p6, p7;

    // Sprite
    Shader* shd;
    Sprite* spr;

    // Primitives
    PrimitiveRenderer* prim;
    Color color;

    // Text
    FontRenderer* font;
    char* glyph;
    std::string* text;
};

VisualSpec getSpriteSpec(Shader& shd, Sprite& spr, float dx, float dy);

VisualSpec getRectangleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, Color color);
VisualSpec getRectangleSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float lineWidth, Color color);
VisualSpec getRoundedRectangleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, Color color);
VisualSpec getRoundedRectangleSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, float lineWidth, Color color);

VisualSpec getCircleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float r, Color color);
VisualSpec getCircleSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float lineWidth, Color color);
VisualSpec getSegmentSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, Color color);
VisualSpec getArcSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color);

VisualSpec getElipseFillSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, Color color);
VisualSpec getElipseSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float lineWidth, Color color);
VisualSpec getElipseSegmentSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color);
VisualSpec getElipseArcSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color);

VisualSpec getTriangleFillSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, Color color);
VisualSpec getTriangleSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color);

VisualSpec getLineSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float lineWidth, Color color);

VisualSpec getGlyphSpec(FontRenderer& font, char& glyph, float dx, float dy);
VisualSpec getTextSpec(FontRenderer& font, std::string& text, float dx, float dy);

#endif
