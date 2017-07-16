#include <systems.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>

void system_kinematics(ECS& ecs, unsigned int deltaT) {
    double sDeltaT = (double) deltaT / 1000.0;

    // Update the position based on velocity
    for (auto it = ecs.begin({comp_position, comp_velocity}); it != ecs.end(); ++it) {
        Entity e = *it;
        auto& pos = * (Vec<2, double>*) ecs.getComponent(e, comp_position);
        auto& vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);

        pos += sDeltaT * vel;
    }

    // Update the velocity based on acceleration
    for (auto it = ecs.begin({comp_velocity, comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;
        auto& vel = * (Vec<2, double>*) ecs.getComponent(e, comp_velocity);
        auto& acc = * (Vec<2, double>*) ecs.getComponent(e, comp_acceleration);

        vel += sDeltaT * acc;
    }
}
