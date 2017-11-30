#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

void system_dynamics(ECS& ecs) {
    for (auto it = ecs.begin({comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;

        auto acc = * (Vec<2, double> *) ecs.getComponent(e, comp_acceleration);

        // Gravity
        double gravData[] = {
               0,
            -100
        };
        acc += Vec<2, double>(gravData);

        // Friction
        if (ecs.getComponent(e, comp_velocity)) {
            auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);
            acc -= vel;
        }

        ecs.setComponent(e, comp_acceleration, &acc);
    }
}
