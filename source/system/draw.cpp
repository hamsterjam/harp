#include <systems.h>

#include <cassert>
#include <queue>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>
#include <graphics/VisualSpec.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/Color.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>

struct SpecWrapper {
    VisualSpec* spec;
    Vec<2, double>* pos;
};

bool operator<(const SpecWrapper& lhs, const SpecWrapper& rhs) {
    // Define it to be >= instead of a < meaning the thing with the lowest
    // layer will get drawn first

    return *(lhs.spec) < *(rhs.spec);
}


void system_draw(ECS& ecs) {
    std::priority_queue<SpecWrapper> drawQueue;

    for (auto it = ecs.beginParented({comp_visual, comp_position}); it != ecs.end(); ++it) {
        Entity e = *it;

        // If it is hidden, do nothing
        if (ecs.getFlag(e, flag_hidden)) continue;

        // Use the local visual spec, (but position is inherited)
        void* s = ecs.getChildComponent(e, comp_visual);
        if (!s) continue;

        SpecWrapper newWrap;

        newWrap.spec = (VisualSpec*) s;
        newWrap.pos  = (Vec<2, double>*) ecs.getComponent(e, comp_position);

        drawQueue.push(newWrap);
    }

    while (!drawQueue.empty()) {

        auto next = drawQueue.top();
        drawQueue.pop();

        auto& spec = * next.spec;
        auto& pos  = * next.pos;

        switch (spec.type) {
            case (DrawType::SPRITE): {
                Shader& shd = *spec.sprite.shd;
                Sprite& spr = *spec.sprite.spr;
                float x = pos[0] + spec.sprite.x;
                float y = pos[1] + spec.sprite.y;

                shd.drawSprite(spr, x, y);
                break;
            }
            case (DrawType::RECTANGLE): {
                PrimitiveRenderer& prim = *spec.prim.prim;
                float x = pos[0] + spec.prim.p1;
                float y = pos[1] + spec.prim.p2;
                float w = spec.prim.p3;
                float h = spec.prim.p4;
                float lineW = spec.prim.p5;
                Color color = spec.prim.color;

                prim.drawRectangle(x, y, w, h, lineW, color);
                break;
            }
            case (DrawType::ROUNDED_RECTANGLE): {
                PrimitiveRenderer& prim = *spec.prim.prim;
                float x = pos[0] + spec.prim.p1;
                float y = pos[1] + spec.prim.p2;
                float w = spec.prim.p3;
                float h = spec.prim.p4;
                float r = spec.prim.p5;
                float lineW = spec.prim.p6;
                Color color = spec.prim.color;

                prim.drawRoundedRectangle(x, y, w, h, r, lineW, color);
                break;
            }
            case (DrawType::ELIPSE): {
                PrimitiveRenderer& prim = *spec.prim.prim;
                float x  = pos[0] + spec.prim.p1;
                float y  = pos[1] + spec.prim.p2;
                float rx = spec.prim.p3;
                float ry = spec.prim.p4;
                float theta1 = spec.prim.p5;
                float theta2 = spec.prim.p6;
                float lineW  = spec.prim.p7;
                Color color  = spec.prim.color;

                prim.drawElipseArc(x, y, rx, ry, theta1, theta2, lineW, color);
                break;
            }
            case (DrawType::TRIANGLE): {
                PrimitiveRenderer& prim = *spec.prim.prim;
                float x1 = pos[0] + spec.prim.p1;
                float y1 = pos[1] + spec.prim.p2;
                float x2 = pos[0] + spec.prim.p3;
                float y2 = pos[1] + spec.prim.p4;
                float x3 = pos[0] + spec.prim.p5;
                float y3 = pos[1] + spec.prim.p6;
                float lineW = spec.prim.p7;
                Color color = spec.prim.color;

                prim.drawTriangle(x1, y1, x2, y2, x3, y3, lineW, color);
                break;
            }
            case (DrawType::LINE): {
                PrimitiveRenderer& prim = *spec.prim.prim;
                float x1 = pos[0] + spec.prim.p1;
                float y1 = pos[1] + spec.prim.p2;
                float x2 = pos[0] + spec.prim.p3;
                float y2 = pos[1] + spec.prim.p4;
                float lineW = spec.prim.p5;
                Color color = spec.prim.color;

                prim.drawLine(x1, y1, x2, y2, lineW, color);
                break;
            }
            case (DrawType::GLYPH): {
                FontRenderer& font = *spec.text.font;
                float x = pos[0] + spec.text.x;
                float y = pos[1] + spec.text.y;
                char glyph = spec.text.glyph;

                font.drawGlyph(glyph, x, y);
                break;
            }
            case (DrawType::TEXT): {
                FontRenderer& font = *spec.text.font;
                float x = pos[0] + spec.text.x;
                float y = pos[1] + spec.text.y;
                const char* text = spec.text.text;

                font.drawText(text, x, y);
                break;
            }
            default:
                assert(false);
        }
    }
}
