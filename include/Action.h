#ifndef HARP_ACTION_H
#define HARP_ACTION_H

extern "C" {
#include <lua.h>
}

enum class Action {
    NO_ACTION,
    JUMP,
    WALK_LEFT,
    WALK_RIGHT,
    FAST_DROP
};

#endif
