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

void VisualSpec::addElipse(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color) {
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
