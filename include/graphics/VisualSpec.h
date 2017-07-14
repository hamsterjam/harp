/*
 * HARP
 * graphics/VisualSpec.h
 *
 * This is my way of storing a general drawing operation data only in an ECS.
 * The important thing is, because it uses dynamicly allocated memory (this
 * isn't really how an ECS should work... But most things only need a Sprite)
 * it is important that the object that creates it has ownership so that the
 * memory allocated by the vector isn't freed.
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

// It's important you hold on to this somewhere, you can't just put it in the ECS and let
// the std::vector deallocate, The ECS never owns pointers, it just stores bits

class VisualSpec {
    // Never thought I'd forward declare INSIDE a class
    public: struct ElementSpec;

    private:
        std::vector<ElementSpec> elements;

    public:
        struct ElementSpec {
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

        std::vector<ElementSpec>::iterator begin();
        std::vector<ElementSpec>::iterator end();

        void addSprite(Shader& shd, Sprite& spr, float dx, float dy);

        void addRectangleFill(PrimitiveRenderer& prim, float dx, float dy, float w, float h, Color color);
        void addRectangle(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float lineWidth, Color color);
        void addRoundedRectangleFill(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, Color color);
        void addRoundedRectangle(PrimitiveRenderer& prim, float dx, float dy, float w, float h, float r, float lineWidth, Color color);

        void addCircleFill(PrimitiveRenderer& prim, float dx, float dy, float r, Color color);
        void addCircle(PrimitiveRenderer& prim, float dx, float dy, float r, float lineWidth, Color color);
        void addSegment(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, Color color);
        void addArc(PrimitiveRenderer& prim, float dx, float dy, float r, float theta1, float theta2, float lineWidth, Color color);

        void addElipseFill(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, Color color);
        void addElipse(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float lineWidth, Color color);
        void addElipseSegment(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color);
        void addElipseArc(PrimitiveRenderer& prim, float dx, float dy, float rx, float ry, float theta1, float theta2, float lineWidth, Color color);

        void addTriangleFill(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, Color color);
        void addTriangle(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float x3, float y3, float lineWidth, Color color);

        void addLine(PrimitiveRenderer& prim, float x1, float y1, float x2, float y2, float lineWidth, Color color);

        void addGlyph(FontRenderer& font, char& glyph, float dx, float dy);
        void addText(FontRenderer& font, std::string& text, float dx, float dy);
};

#endif
