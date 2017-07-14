#include <systems.h>

#include <ECS.h>
#include <globals.h>

void system_parent(ECS& ecs) {
    for (auto it = ecs.begin({comp_parent}); it != ecs.end(); ++it) {
        Entity e = *it;

        Entity* nextPar = (Entity*) ecs.getComponent(e, comp_parent);
        Entity* par;

        while (nextPar) {
            par = nextPar;
            nextPar = (Entity*) ecs.getComponent(*par, comp_parent);
        }

        Entity& p = *par;

        void* ePos = ecs.getComponent(e, comp_position);
        void* pPos = ecs.getComponent(p, comp_position);
        if (ePos && pPos) ecs.setComponent(e, comp_position, pPos);

        ecs.setFlag(e, flag_hidden, ecs.getFlag(p, flag_hidden));
    }
}
