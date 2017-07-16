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
            case (DrawType::SPRITE):
                spec.shd->drawSprite(*spec.spr, pos[0]+spec.p1, pos[1]+spec.p2);
                break;
            case (DrawType::RECTANGLE):
                spec.prim->drawRectangle(pos[0]+spec.p1, pos[1]+spec.p2, spec.p3, spec.p4, spec.p5, spec.color);
                break;
            case (DrawType::ROUNDED_RECTANGLE):
                spec.prim->drawRoundedRectangle(pos[0]+spec.p1, pos[1]+spec.p2, spec.p3, spec.p4, spec.p5, spec.p6, spec.color);
                break;
            case (DrawType::ELIPSE):
                spec.prim->drawElipseArc(pos[0]+spec.p1, pos[1]+spec.p2, spec.p3, spec.p4, spec.p5, spec.p6, spec.p7, spec.color);
                break;
            case (DrawType::TRIANGLE):
                spec.prim->drawTriangle(pos[0]+spec.p1, pos[1]+spec.p2, pos[0]+spec.p3, pos[1]+spec.p4, pos[0]+spec.p5, pos[1]+spec.p6, spec.p7, spec.color);
                break;
            case (DrawType::LINE):
                spec.prim->drawLine(pos[0]+spec.p1, pos[1]+spec.p2, pos[0]+spec.p3, pos[1]+spec.p4, spec.p5, spec.color);
                break;
            case (DrawType::GLYPH):
                spec.font->drawGlyph(*spec.glyph, pos[0]+spec.p1, pos[1]+spec.p2);
            case (DrawType::TEXT):
                spec.font->drawText(*spec.text, pos[0]+spec.p1, pos[1]+spec.p2);
                break;
            default:
                break;
        }
    }
}
