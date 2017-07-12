#include <systems.h>

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

        // If it is hidden, do nothing
        if (harp->getFlagComponent(e, comp_hidden)) continue;

        auto& pos  = * (Vector<2, double>*) harp->getComponent(e, comp_position);
        auto& spec = * (VisualSpec*) harp->getComponent(e, comp_visual);

        for (auto jt = spec.begin(); jt != spec.end(); ++jt) {
            auto& ele = *jt;
            switch (ele.type) {
                case (DrawType::SPRITE):
                    ele.shd->drawSprite(*ele.spr, pos[0]+ele.p1, pos[1]+ele.p2);
                    break;
                case (DrawType::RECTANGLE):
                    ele.prim->drawRectangle(pos[0]+ele.p1, pos[1]+ele.p2, ele.p3, ele.p4, ele.p5, ele.color);
                    break;
                case (DrawType::ROUNDED_RECTANGLE):
                    ele.prim->drawRoundedRectangle(pos[0]+ele.p1, pos[1]+ele.p2, ele.p3, ele.p4, ele.p5, ele.p6, ele.color);
                    break;
                case (DrawType::ELIPSE):
                    ele.prim->drawElipseArc(pos[0]+ele.p1, pos[1]+ele.p2, ele.p3, ele.p4, ele.p5, ele.p6, ele.p7, ele.color);
                    break;
                case (DrawType::TRIANGLE):
                    ele.prim->drawTriangle(pos[0]+ele.p1, pos[1]+ele.p2, pos[0]+ele.p3, pos[1]+ele.p4, pos[0]+ele.p5, pos[1]+ele.p6, ele.p7, ele.color);
                    break;
                case (DrawType::LINE):
                    ele.prim->drawLine(pos[0]+ele.p1, pos[1]+ele.p2, pos[0]+ele.p3, pos[1]+ele.p4, ele.p5, ele.color);
                    break;
                case (DrawType::GLYPH):
                    ele.font->drawGlyph(*ele.glyph, pos[0]+ele.p1, pos[1]+ele.p2);
                case (DrawType::TEXT):
                    ele.font->drawString(ele.text, pos[0]+ele.p1, pos[1]+ele.p2);
                    break;
                default:
                    break;
            }
        }
    }
}
