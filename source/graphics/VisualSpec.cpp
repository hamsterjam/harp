#include <string>

#include <graphics/VisualSpec.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/Color.h>

// These are the most general form of the shape in question

VisualSpec getSpriteSpec(Shader& shd, Sprite& spr, float dx, float dy) {
    VisualSpec spec;

    spec.type = DrawType::SPRITE;
    spec.p1   = dx;
    spec.p2   = dy;
    spec.shd  = &shd;
    spec.spr  = &spr;

    return spec;
}

VisualSpec getRectangleSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float lineWidth, Color color) {
    VisualSpec spec;

    spec.type  = DrawType::RECTANGLE;
    spec.p1    = dx;
    spec.p2    = dy;
    spec.p3    = w;
    spec.p4    = h;
    spec.p5    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    return spec;
}

VisualSpec getRoundedRectangleSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, float lineWidth, Color color) {
    VisualSpec spec;

    spec.type  = DrawType::ROUNDED_RECTANGLE;
    spec.p1    = dx;
    spec.p2    = dy;
    spec.p3    = w;
    spec.p4    = h;
    spec.p5    = r;
    spec.p6    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    return spec;
}

VisualSpec getElipseArcSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    VisualSpec spec;

    spec.type  = DrawType::ELIPSE;
    spec.p1    = dx;
    spec.p2    = dy;
    spec.p3    = rx;
    spec.p4    = ry;
    spec.p5    = theta1;
    spec.p6    = theta2;
    spec.p7    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    return spec;
}

VisualSpec getTriangleSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color) {
    VisualSpec spec;

    spec.type  = DrawType::TRIANGLE;
    spec.p1    = x1;
    spec.p2    = y1;
    spec.p3    = x2;
    spec.p4    = y2;
    spec.p5    = x3;
    spec.p6    = y3;
    spec.p7    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    return spec;
}

VisualSpec getLineSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float lineWidth, Color color) {
    VisualSpec spec;

    spec.type  = DrawType::LINE;
    spec.p1    = x1;
    spec.p2    = y1;
    spec.p3    = x2;
    spec.p4    = y2;
    spec.p5    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    return spec;
}

VisualSpec getGlyphSpec(FontRenderer& font, char& glyph, float dx, float dy) {
    VisualSpec spec;

    spec.type  = DrawType::GLYPH;
    spec.font  = &font;
    spec.glyph = &glyph;

    return spec;
}

VisualSpec getTextSpec(FontRenderer& font, std::string& text, float dx, float dy) {
    VisualSpec spec;

    spec.type = DrawType::TEXT;
    spec.p1   = dx;
    spec.p2   = dy;
    spec.font = &font;
    spec.text = &text;

    return spec;
}

// These functions that call the more general versions

// Rectangles
VisualSpec getRectangleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, Color color) {
    return getRectangleSpec(prim, dx, dy, w, h, 0, color);
}
VisualSpec getRoundedRectangleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, Color color) {
    return getRoundedRectangleSpec(prim, dx, dy, w, h, r, 0, color);
}

// Circles
VisualSpec getCircleFillSpec(PrimitiveRenderer& prim, float dx, float dy, float r, Color color) {
    return getElipseArcSpec(prim, dx, dy, r, r, 0, 360, 0, color);
}
VisualSpec getCircleSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float lineWidth, Color color) {
    return getElipseArcSpec(prim, dx, dy, r, r, 0, 360, lineWidth, color);
}
VisualSpec getSegmentSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, Color color) {
    return getElipseArcSpec(prim, dx, dy, r, r, theta1, theta2, 0, color);
}
VisualSpec getArcSpec(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color) {
    return getElipseArcSpec(prim, dx, dy, r, r, theta1, theta2, lineWidth, color);
}

// Elipses
VisualSpec getElipseFillSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, Color color) {
    return getElipseArcSpec(prim, dx, dy, rx, ry, 0, 360, 0, color);
}
VisualSpec getElipseSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float lineWidth, Color color) {
    return getElipseArcSpec(prim, dx, dy, rx, ry, 0, 360, lineWidth, color);
}
VisualSpec getElipseSegmentSpec(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    return getElipseArcSpec(prim, dx, dy, rx, ry, theta1, theta2, 0, color);
}

// Triangles
VisualSpec getTriangleFillSpec(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, Color color) {
    return getTriangleSpec(prim, x1, y1, x2, y2, x3, y3, 0, color);
}
