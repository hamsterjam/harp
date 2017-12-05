#include <systems.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>

void system_kinematics(ECS& ecs, unsigned int deltaT) {
    double sDeltaT = (double) deltaT / 1000.0;
    double sDeltaT2 = sDeltaT * sDeltaT;

    // Update the position based on velocity
    for (auto it = ecs.begin({comp_position, comp_velocity}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getFlag(e, flag_static)) continue;
        auto pos = * (Vec<2, double>*) ecs.getComponent(e, comp_position);
        auto vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto acc = zeroVec<2, double>();

        if (ecs.getComponent(e, comp_acceleration)) {
            acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);
        }

        pos += (sDeltaT * vel) + (0.5 * sDeltaT2 * acc);

        ecs.setComponent(e, comp_position, &pos);
    }

    // Update the velocity based on acceleration
    for (auto it = ecs.begin({comp_velocity, comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getFlag(e, flag_static)) continue;
        auto vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);

        vel += sDeltaT * acc;

        ecs.setComponent(e, comp_velocity, &vel);
    }
}
