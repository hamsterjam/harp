#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

// At the very least, this should move the nextPosition component over to the
// position component

void system_collision(ECS& ecs) {
    for (auto it = ecs.begin({comp_velocity, comp_position, comp_nextPosition}); it != ecs.end(); ++it) {
        Entity e = *it;

        auto currPos = * (Vec<2, double> *) ecs.getComponent(e, comp_position);
        auto nextPos = * (Vec<2, double> *) ecs.getComponent(e, comp_nextPosition);
        auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);

        if (nextPos[1] < 0) {;
            nextPos[1] = 0;
            vel[1] = 0;

            ecs.setComponent(e, comp_velocity, &vel);
        }

        ecs.setComponent(e, comp_position, &nextPos);
    }
}
