#ifndef HARP_FUNCTION_WRAPPER_H
#define HARP_FUNCTION_WRAPPER_H

extern "C" {
#include <lua.h>
}

struct FunctionWrapper {
    bool isLua;
    union {
        int luaFunc;
        void* cFunc;
    };
};

#endif
