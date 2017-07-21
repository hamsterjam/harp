#include <systems.h>

#include <cassert>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>
#include <graphics/VisualSpec.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/Color.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>

void system_draw(ECS& ecs) {
    for (auto it = ecs.beginParented({comp_position, comp_visual}); it != ecs.end(); ++it) {
        Entity e = *it;

        // If it is hidden, do nothing
        if (ecs.getFlag(e, flag_hidden)) continue;

        // Use the local visual spec, (but position is inherited)
        void* s = ecs.getChildComponent(e, comp_visual);
        if (!s) continue;

        auto& spec = * (VisualSpec*) s;

        auto& pos  = * (Vec<2, double>*) ecs.getComponent(e, comp_position);

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
                char& glyph = *spec.text.glyph;

                font.drawGlyph(glyph, x, y);
            }
            case (DrawType::TEXT): {
                FontRenderer& font = *spec.text.font;
                float x = pos[0] + spec.text.x;
                float y = pos[1] + spec.text.y;
                std::string& text = *spec.text.text;

                font.drawText(text, x, y);
                break;
            }
            default:
                assert(false);
        }
    }
}
