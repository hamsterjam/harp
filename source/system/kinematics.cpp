#include <systems.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>

void system_kinematics(ECS& ecs, unsigned int deltaT, bool partialStep) {
    double sDeltaT = (double) deltaT / 1000.0;

    // Update the position based on velocity
    for (auto it = ecs.begin({comp_position, comp_velocity}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getFlag(e, flag_static)) continue;

        double stepT = sDeltaT;

        if (partialStep) {
            if (!ecs.getComponent(e, comp_partialStep)) continue;
            stepT *= * (double*) ecs.getComponent(e, comp_partialStep);
        }

        auto pos = * (Vec<2, double>*) ecs.getComponent(e, comp_position);
        auto vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto acc = zeroVec<2, double>();

        if (ecs.getComponent(e, comp_acceleration)) {
            acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);
        }

        pos += (stepT * vel) + (0.5 * stepT * stepT * acc);

        // Setting nextPosition so the collision system knows both where we
        // are going, and where we came from so we can do better collisions
        // than standard clipping collisions
        ecs.setComponent(e, comp_nextPosition, &pos);
    }

    // Update the velocity based on acceleration
    for (auto it = ecs.begin({comp_velocity, comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getFlag(e, flag_static)) continue;

        double stepT = sDeltaT;

        if (partialStep) {
            if (!ecs.getComponent(e, comp_partialStep)) continue;
            stepT *= * (double*) ecs.getComponent(e, comp_partialStep);
        }

        auto vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);

        vel += stepT * acc;

        ecs.setComponent(e, comp_velocity, &vel);
    }
}
