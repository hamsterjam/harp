#include <graphics/VisualSpec.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/Color.h>

// These are the most general form of the shape in question

bool operator<(const VisualSpec& lhs, const VisualSpec& rhs) {
    return lhs.layer < rhs.layer;
}

VisualSpec getSpriteSpec(Sprite& spr, float dx, float dy, int layer, Shader& shd) {
    VisualSpec spec;

    spec.type  = DrawType::SPRITE;
    spec.layer = layer;

    spec.sprite.x   = dx;
    spec.sprite.y   = dy;
    spec.sprite.shd = &shd;
    spec.sprite.spr = &spr;

    return spec;
}

VisualSpec getRectangleSpec(float dx, float dy, float w, float h, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    VisualSpec spec;

    spec.type  = DrawType::RECTANGLE;
    spec.layer = layer;

    spec.prim.p1    = dx;
    spec.prim.p2    = dy;
    spec.prim.p3    = w;
    spec.prim.p4    = h;
    spec.prim.p5    = lineWidth;
    spec.prim.prim  = &prim;
    spec.prim.color = color;

    return spec;
}

VisualSpec getRoundedRectangleSpec(float dx, float dy, float w, float h, float r, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    VisualSpec spec;

    spec.type  = DrawType::ROUNDED_RECTANGLE;
    spec.layer = layer;

    spec.prim.p1    = dx;
    spec.prim.p2    = dy;
    spec.prim.p3    = w;
    spec.prim.p4    = h;
    spec.prim.p5    = r;
    spec.prim.p6    = lineWidth;
    spec.prim.prim  = &prim;
    spec.prim.color = color;

    return spec;
}

VisualSpec getElipseArcSpec(float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    VisualSpec spec;

    spec.type  = DrawType::ELIPSE;
    spec.layer = layer;

    spec.prim.p1    = dx;
    spec.prim.p2    = dy;
    spec.prim.p3    = rx;
    spec.prim.p4    = ry;
    spec.prim.p5    = theta1;
    spec.prim.p6    = theta2;
    spec.prim.p7    = lineWidth;
    spec.prim.prim  = &prim;
    spec.prim.color = color;

    return spec;
}

VisualSpec getTriangleSpec(float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    VisualSpec spec;

    spec.type  = DrawType::TRIANGLE;
    spec.layer = layer;

    spec.prim.p1    = x1;
    spec.prim.p2    = y1;
    spec.prim.p3    = x2;
    spec.prim.p4    = y2;
    spec.prim.p5    = x3;
    spec.prim.p6    = y3;
    spec.prim.p7    = lineWidth;
    spec.prim.prim  = &prim;
    spec.prim.color = color;

    return spec;
}

VisualSpec getLineSpec(float x1, float y1, float x2, float y2, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    VisualSpec spec;

    spec.type  = DrawType::LINE;
    spec.layer = layer;

    spec.prim.p1    = x1;
    spec.prim.p2    = y1;
    spec.prim.p3    = x2;
    spec.prim.p4    = y2;
    spec.prim.p5    = lineWidth;
    spec.prim.prim  = &prim;
    spec.prim.color = color;

    return spec;
}

VisualSpec getGlyphSpec(char glyph, float dx, float dy, int layer, FontRenderer& font) {
    VisualSpec spec;

    spec.type  = DrawType::GLYPH;
    spec.layer = layer;

    spec.text.x     = dx;
    spec.text.y     = dy;
    spec.text.font  = &font;
    spec.text.glyph = glyph;

    return spec;
}

VisualSpec getTextSpec(const char* text, float dx, float dy, int layer, FontRenderer& font) {
    VisualSpec spec;

    spec.type  = DrawType::TEXT;
    spec.layer = layer;

    spec.text.x    = dx;
    spec.text.y    = dy;
    spec.text.font = &font;
    spec.text.text = text;

    return spec;
}

// These functions that call the more general versions

// Rectangles
VisualSpec getRectangleFillSpec(float dx, float dy, float w, float h, Color color, int layer, PrimitiveRenderer& prim) {
    return getRectangleSpec(dx, dy, w, h, 0, color, layer, prim);
}
VisualSpec getRoundedRectangleFillSpec(float dx, float dy, float w, float h, float r, Color color, int layer, PrimitiveRenderer& prim) {
    return getRoundedRectangleSpec(dx, dy, w, h, r, 0, color, layer, prim);
}

// Circles
VisualSpec getCircleFillSpec(float dx, float dy, float r, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, r, r, 0, 360, 0, color, layer, prim);
}
VisualSpec getCircleSpec(float dx, float dy, float r, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, r, r, 0, 360, lineWidth, color, layer, prim);
}
VisualSpec getSegmentSpec(float dx, float dy, float r, float theta1, float theta2, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, r, r, theta1, theta2, 0, color, layer, prim);
}
VisualSpec getArcSpec(float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, r, r, theta1, theta2, lineWidth, color, layer, prim);
}

// Elipses
VisualSpec getElipseFillSpec(float dx, float dy, float rx, float ry, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, rx, ry, 0, 360, 0, color, layer, prim);
}
VisualSpec getElipseSpec(float dx, float dy, float rx, float ry, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, rx, ry, 0, 360, lineWidth, color, layer, prim);
}
VisualSpec getElipseSegmentSpec(float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color, int layer, PrimitiveRenderer& prim) {
    return getElipseArcSpec(dx, dy, rx, ry, theta1, theta2, 0, color, layer, prim);
}

// Triangles
VisualSpec getTriangleFillSpec(float x1, float y1, float x2, float y2, float x3, float y3, Color color, int layer, PrimitiveRenderer& prim) {
    return getTriangleSpec(x1, y1, x2, y2, x3, y3, 0, color, layer, prim);
}
