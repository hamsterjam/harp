#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

const double omega = 0.01;

void system_fudge(ECS& ecs) {
    for (auto it = ecs.begin({comp_velocity}); it != ecs.end(); ++it) {
        Entity e = *it;

        auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);

        // Fudge velocity to be zero if it is very close to zero
        bool velChanged = false;
        for (int i = 0; i < 2; ++i) {
            double abs = (vel[i] < 0) ? -vel[i] : vel[i];
            if (abs < omega) {
                vel[i] = 0;
                velChanged = true;
            }
        }

        if (velChanged) ecs.setComponent(e, comp_velocity, &vel);
    }
}
