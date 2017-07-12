#include <system/draw.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>
#include <graphics/VisualSpec.h>
#include <graphics/Shader.h>
#include <graphics/Sprite.h>
#include <graphics/Color.h>
#include <graphics/PrimitiveRenderer.h>
#include <graphics/FontRenderer.h>

void system_draw() {
    for (auto it = harp->begin({comp_position, comp_visual}); it != harp->end(); ++it) {
        Entity e = *it;
        auto& pos  = * (Vector<2, double>*) harp->getComponent(e, comp_position);
        auto& spec = * (VisualSpec*) harp->getComponent(e, comp_visual);

        for (auto jt = spec.begin(); jt != spec.end(); ++jt) {
            auto& ele = *jt;
            switch (ele.type) {
                case (DrawType::SPRITE):
                    ele.shd->drawSprite(*ele.spr, ele.p1, ele.p2);
                    break;
                case (DrawType::RECTANGLE):
                    ele.prim->drawRectangle(ele.p1, ele.p2, ele.p3, ele.p4, ele.p5, ele.color);
                    break;
                case (DrawType::ROUNDED_RECTANGLE):
                    ele.prim->drawRoundedRectangle(ele.p1, ele.p2, ele.p3, ele.p4, ele.p5, ele.p6, ele.color);
                    break;
                case (DrawType::ELIPSE):
                    ele.prim->drawElipseArc(ele.p1, ele.p2, ele.p3, ele.p4, ele.p5, ele.p6, ele.p7, ele.color);
                    break;
                case (DrawType::TRIANGLE):
                    ele.prim->drawTriangle(ele.p1, ele.p2, ele.p3, ele.p4, ele.p5, ele.p6, ele.p7, ele.color);
                    break;
                case (DrawType::LINE):
                    ele.prim->drawLine(ele.p1, ele.p2, ele.p3, ele.p4, ele.p5, ele.color);
                    break;
                case (DrawType::GLYPH):
                    ele.font->drawGlyph(*ele.glyph, ele.p1, ele.p2);
                case (DrawType::TEXT):
                    ele.font->drawString(ele.text, ele.p1, ele.p2);
                    break;
                default:
                    break;
            }
        }
    }
}
