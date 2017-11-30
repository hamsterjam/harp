#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

void system_collision(ECS& ecs) {
    for (auto it = ecs.begin({comp_velocity, comp_position}); it != ecs.end(); ++it) {
        Entity e = *it;

        auto pos = * (Vec<2, double> *) ecs.getComponent(e, comp_position);
        auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);

        if (pos[1] < 0) {
            pos[1] = 0;
            vel[1] = 0;

            ecs.setComponent(e, comp_position, &pos);
            ecs.setComponent(e, comp_velocity, &vel);
        }
    }
}
