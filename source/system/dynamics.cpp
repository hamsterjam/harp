#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

void system_dynamics(ECS& ecs) {
    for (auto it = ecs.begin({comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getFlag(e, flag_static)) continue;

        // If no partialStep component, then this is the first loop and we need
        // to generate it
        if (!ecs.getComponent(e, comp_partialStep)) {
            double partialStep = 1;
            ecs.setComponent(e, comp_partialStep, &partialStep);
        }
        else {
            // If it is 0 then we are done processing
            auto partialStep = * (double *) ecs.getComponent(e, comp_partialStep);
            if (partialStep == 0) continue;
        }

        auto acc = * (Vec<2, double> *) ecs.getComponent(e, comp_acceleration);

        if (ecs.getComponent(e, comp_onSurface)) {
            // On a surface

            // Friction
            if (ecs.getComponent(e, comp_velocity)) {
                auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);
                acc -= 2 * vel;
            }
        }
        else {
            // Not on a surface

            // Gravity
            double gravData[] = {
                   0,
                -200
            };
            acc += Vec<2, double>(gravData);

            // Friction
            if (ecs.getComponent(e, comp_velocity)) {
                auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);
                acc -= 0.1 * vel;
            }
        }


        ecs.setComponent(e, comp_acceleration, &acc);
    }
}
