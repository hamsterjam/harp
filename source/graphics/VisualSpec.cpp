#include <graphics/VisualSpec.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>
#include <graphics/Color.h>

std::vector<VisualSpec::ElementSpec>::iterator VisualSpec::begin() {
    return elements.begin();
}

std::vector<VisualSpec::ElementSpec>::iterator VisualSpec::end() {
    return elements.end();
}

// These are the most general form of the shape in question

void VisualSpec::addSprite(Shader& shd, Sprite& spr, float dx, float dy) {
    ElementSpec spec;

    spec.type = DrawType::SPRITE;
    spec.p1   = dx;
    spec.p2   = dy;
    spec.shd  = &shd;
    spec.spr  = &spr;

    elements.push_back(spec);
}

void VisualSpec::addRectangle(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float lineWidth, Color color) {
    ElementSpec spec;

    spec.type  = DrawType::RECTANGLE;
    spec.p1    = dx;
    spec.p2    = dy;
    spec.p3    = w;
    spec.p4    = h;
    spec.p5    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    elements.push_back(spec);
}

void VisualSpec::addRoundedRectangle(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, float lineWidth, Color color) {
    ElementSpec spec;

    spec.type  = DrawType::ROUNDED_RECTANGLE;
    spec.p1    = dx;
    spec.p2    = dy;
    spec.p3    = w;
    spec.p4    = h;
    spec.p5    = r;
    spec.p6    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    elements.push_back(spec);
}

void VisualSpec::addElipseArc(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    ElementSpec spec;

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

    elements.push_back(spec);
}

void VisualSpec::addTriangle(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color) {
    ElementSpec spec;

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

    elements.push_back(spec);
}

void VisualSpec::addLine(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float lineWidth, Color color) {
    ElementSpec spec;

    spec.type  = DrawType::LINE;
    spec.p1    = x1;
    spec.p2    = y1;
    spec.p3    = x2;
    spec.p4    = y2;
    spec.p5    = lineWidth;
    spec.prim  = &prim;
    spec.color = color;

    elements.push_back(spec);
}

void VisualSpec::addGlyph(FontRenderer& font, char& glyph, float dx, float dy) {
    ElementSpec spec;

    spec.type  = DrawType::GLYPH;
    spec.font  = &font;
    spec.glyph = &glyph;

    elements.push_back(spec);
}

void VisualSpec::addText(FontRenderer& font, char* text, float dx, float dy) {
    ElementSpec spec;

    spec.type = DrawType::TEXT;
    spec.p1   = dx;
    spec.p2   = dy;
    spec.font = &font;
    spec.text = text;

    elements.push_back(spec);
}

// These functions that call the more general versions

// Rectangles
void VisualSpec::addRectangleFill(PrimitiveRenderer& prim, float dx, float dy, float w, float h, Color color) {
    addRectangle(prim, dx, dy, w, h, 0, color);
}
void VisualSpec::addRoundedRectangleFill(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, Color color) {
    addRoundedRectangle(prim, dx, dy, w, h, r, 0, color);
}

// Circles
void VisualSpec::addCircleFill(PrimitiveRenderer& prim, float dx, float dy, float r, Color color) {
    addElipseArc(prim, dx, dy, r, r, 0, 360, 0, color);
}
void VisualSpec::addCircle(PrimitiveRenderer& prim, float dx, float dy, float r, float lineWidth, Color color) {
    addElipseArc(prim, dx, dy, r, r, 0, 360, lineWidth, color);
}
void VisualSpec::addSegment(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, Color color) {
    addElipseArc(prim, dx, dy, r, r, theta1, theta2, 0, color);
}
void VisualSpec::addArc(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color) {
    addElipseArc(prim, dx, dy, r, r, theta1, theta2, lineWidth, color);
}

// Elipses
void VisualSpec::addElipseFill(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, Color color) {
    addElipseArc(prim, dx, dy, rx, ry, 0, 360, 0, color);
}
void VisualSpec::addElipse(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float lineWidth, Color color) {
    addElipseArc(prim, dx, dy, rx, ry, 0, 360, lineWidth, color);
}
void VisualSpec::addElipseSegment(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
    addElipseArc(prim, dx, dy, rx, ry, theta1, theta2, 0, color);
}

// Triangles
void VisualSpec::addTriangleFill(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, Color color) {
    addTriangle(prim, x1, y1, x2, y2, x3, y3, 0, color);
}
