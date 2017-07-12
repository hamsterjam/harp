#include <systems.h>

#include <ECS.h>
#include <harpMath.h>
#include <globals.h>

void system_kinematics(unsigned int deltaT) {
    double sDeltaT = (double) deltaT / 1000.0;

    // Update the position based on velocity
    for (auto it = harp->begin({comp_position, comp_velocity}); it != harp->end(); ++it) {
        Entity e = *it;
        auto& pos = * (Vector<2, double>*) harp->getComponent(e, comp_position);
        auto& vel = * (Vector<2, double>*) harp->getComponent(e, comp_velocity);

        pos += sDeltaT * vel;
    }

    // Update the velocity based on acceleration
    for (auto it = harp->begin({comp_velocity, comp_acceleration}); it != harp->end(); ++it) {
        Entity e = *it;
        auto& vel = * (Vector<2, double>*) harp->getComponent(e, comp_velocity);
        auto& acc = * (Vector<2, double>*) harp->getComponent(e, comp_acceleration);

        vel += sDeltaT * acc;
    }
}
