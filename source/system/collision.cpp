#include <systems.h>

#include <cassert>
#include <cmath>

#include <ECS.h>
#include <globals.h>
#include <harpMath.h>
#include <Collider.h>

// At the very least, this should move the nextPosition component over to the
// position component

// I think I might lose my mind if i don't do this
typedef Vec<2, double> Vec2;

double collisionTimeBoxLine(Vec2 b, Vec2 db, Vec2 l, Vec2 dl, Vec2 v) {
    // First we should check that the box is heading in the right direction
    if (dot(perp(dl), v) >= 0) {
        return 1;
    }

    // Now we need to find the leading corner
    Vec2 p0 = b;
    Vec2 p1 = b;
    Vec2 p2 = b;

    Vec2 q0 = l;
    Vec2 q1 = l + dl;
    if (v[0] <= 0) {
        // Left
        if (v[1] <= 0) {
            // Down
            p0     = p0;
            p1[0] += db[0];
            p2[1] += db[1];
        }
        else {
            // Up
            p0[1] += db[1];
            p1     = p1;
            p2    += db;
        }
    }
    else {
        // Right
        if (v[1] <= 0) {
            // Down
            p0[0] += db[0];
            p1     = p1;
            p2    += db;
        }
        else {
            // Up
            p0    += db;
            p1[0] += db[0];
            p2[1] += db[1];
        }
    }

    // Finally, check all the collisions
    double t = 1;
    double tTest = 1;
    if (lineSegmentsIntersect(p0, p0+v, q0, q1, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(p1, p1+v, q0, q1, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(p2, p2+v, q0, q1, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(q0, q0-v, p0, p1, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(q0, q0-v, p0, p2, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(q1, q1-v, p0, p1, tTest)) {
        t = (tTest < t) ? tTest : t;
    }
    if (lineSegmentsIntersect(q1, q1-v, p0, p2, tTest)) {
        t = (tTest < t) ? tTest : t;
    }

    return t;
}

void system_collision(ECS& ecs) {
    // First we need to remove ALL the onSurface components
    for (auto it = ecs.begin({comp_onSurface}); it != ecs.end(); ++it) {
        Entity e = *it;
        ecs.removeComponent(e, comp_onSurface);
    }
    // Shouldn't need to update components just yet

    for (auto it = ecs.begin({comp_position, comp_nextPosition, comp_collider}); it != ecs.end(); ++it) {
        Entity ent = *it;

        // Skip if it is static, you can still collide WITH static things, but
        // a static thing itself can't itself collide
        if (ecs.getFlag(ent, flag_static)) continue;

        // Also skip if processing is done
        {
            auto partialStep = * (double *) ecs.getComponent(ent, comp_partialStep);
            if (partialStep == 0) continue;
        }

        auto entPos = * (Vec2*) ecs.getComponent(ent, comp_position);
        auto entVel = (* (Vec2*) ecs.getComponent(ent, comp_nextPosition)) - entPos;
        auto entCol = * (Collider*) ecs.getComponent(ent, comp_collider);

        double colTime = 1;
        Vec2   surfNorm;

        // We need to check all unordered pairs if they are both non-static
        for (auto it2 = it; it2 != ecs.end(); ++it2) {
            Entity tar = *it2;
            if (ecs.getFlag(tar, flag_static)) continue;

            auto tarCol = * (Collider*) ecs.getComponent(tar, comp_collider);
            // Non-Static Collisions

            //TODO// Write
        }

        // We want to check ALL static colliders as they are never the collidee
        for (auto it2 = ecs.begin({comp_position, comp_collider, flag_static}); it2 != ecs.end(); ++it2) {
            Entity tar = *it2;

            auto tarPos = * (Vec2*)     ecs.getComponent(tar, comp_position);
            auto tarCol = * (Collider*) ecs.getComponent(tar, comp_collider);
            // Static Collisions

            double testTime = 1;

            // The more collider types we provide, the more enormous this is
            // going to get, might want to factor it out a bit
            switch (tarCol.type) {
                case BOX_COLLIDER:
                    switch (entCol.type) {
                        case BOX_COLLIDER:
                            break;
                        case LINE_COLLIDER:
                            testTime = collisionTimeBoxLine(tarPos, tarCol.delta,
                                                            entPos, entCol.delta,
                                                           -entVel);
                            break;
                        default:
                            assert(false);
                    }
                    break;
                case LINE_COLLIDER:
                    switch (entCol.type) {
                        case BOX_COLLIDER:
                            testTime = collisionTimeBoxLine(entPos, entCol.delta,
                                                            tarPos, tarCol.delta,
                                                            entVel);
                            if (testTime < colTime) {
                                colTime = testTime;
                                surfNorm = perp(tarCol.delta);
                            }
                            break;
                        case LINE_COLLIDER:
                            break;
                        default:
                            assert(false);
                    }
                    break;
                default:
                    assert(false);
            }
        }

        // Regardless of if there is a collision or not, we need to set the
        // position to the same thing
        if (colTime < 1) {
            // Collision occured
            ecs.setComponent(ent, comp_onSurface, &surfNorm);

            // Fudge colTime a little to prevent phasing through stuff
            colTime -= 0.01;
        }
        double partialStep = * (double*) ecs.getComponent(ent, comp_partialStep);
        partialStep *= 1 - colTime;
        ecs.setComponent(ent, comp_partialStep, &partialStep);


        auto newPos = entPos + (colTime * entVel);
        ecs.setComponent(ent, comp_nextPosition, &newPos);
    }

    // If it has no collider, then it uses the entire partialStep
    for (auto it = ecs.begin({comp_partialStep}); it != ecs.end(); ++it) {
        Entity e = *it;
        if (ecs.getComponent(e, comp_collider)) continue;

        double zero = 0;
        ecs.setComponent(e, comp_partialStep, &zero);
    }
}
