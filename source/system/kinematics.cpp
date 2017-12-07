#include <systems.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>

void system_proposeKinematics(ECS& ecs, unsigned int deltaT) {
    double sDeltaT = (double) deltaT / 1000.0;

    // Update the position based on velocity
    for (auto it = ecs.begin({comp_position, comp_velocity}); it != ecs.end(); ++it) {
        Entity e = *it;

        if (ecs.getFlag(e, flag_static)) continue;
        // Might still not have a partial step, (if it has no acceleration)
        double partialStep = 1;
        if (!ecs.getComponent(e, comp_partialStep)) {
            ecs.setComponent(e, comp_partialStep, &partialStep);
        }
        else {
            partialStep = * (double *) ecs.getComponent(e, comp_partialStep);
            if (partialStep == 0) continue;
        }

        double stepT = partialStep * sDeltaT;

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
        if (ecs.getComponent(e, comp_partialStep)) {
            auto partialStep = * (double *) ecs.getComponent(e, comp_partialStep);
            if (partialStep == 0) continue;
        }

        double stepT = sDeltaT;
        stepT *= * (double *) ecs.getComponent(e, comp_partialStep);

        auto vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);

        vel += stepT * acc;

        ecs.setComponent(e, comp_velocity, &vel);
    }
}

void system_resolveKinematics(ECS& ecs) {
    // If we are on a surface, remove all acc and vel perpindicular to that surface
    for (auto it = ecs.begin({comp_onSurface}); it!= ecs.end(); ++it) {
        Entity e = *it;
        auto surfNorm = * (Vec<2, double> *) ecs.getComponent(e, comp_onSurface);

        if (ecs.getComponent(e, comp_velocity)) {
            auto vel = * (Vec<2, double> *) ecs.getComponent(e, comp_velocity);
            vel -= proj(vel, surfNorm);
            ecs.setComponent(e, comp_velocity, &vel);
        }
        if (ecs.getComponent(e, comp_acceleration)) {
            auto acc = * (Vec<2, double> *) ecs.getComponent(e, comp_acceleration);
            acc -= proj(acc, surfNorm);
            ecs.setComponent(e, comp_acceleration, &acc);
        }
    }

    // Perform the partial step
    for (auto it = ecs.begin({comp_position, comp_nextPosition}); it != ecs.end(); ++it) {
        Entity e = *it;
        ecs.setComponent(e, comp_position, ecs.getComponent(e, comp_nextPosition));
        ecs.removeComponent(e, comp_nextPosition);
    }
}
