#include <systems.h>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>

void system_zeroAcceleration(ECS& ecs) {
    Vec<2, double> zero = zeroVec<2, double>();
    for (auto it = ecs.begin({comp_acceleration}); it != ecs.end(); ++it) {
        Entity e = *it;
        ecs.setComponent(e, comp_acceleration, &zero);
    }
}
