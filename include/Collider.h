#ifndef HARP_COLLIDER_H
#define HARP_COLLIDER_H

#include <harpMath.h>

enum ColliderType {
    BOX_COLLIDER,
    LINE_COLLIDER
};

struct Collider {
    ColliderType type;
    Vec<2, double> delta;
};

#endif
