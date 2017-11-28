#include <systems.h>

#include <ECS.h>
#include <FunctionWrapper.h>
#include <luaInterface.h>
#include <Console.h>
#include <globals.h>

void system_input(ECS& ecs) {
    for (auto it = ecs.begin({comp_inputFunc}); it != ecs.end(); ++it) {
        Entity e = *it;
        auto inputFunc = * (FunctionWrapper *) ecs.getComponent(e, comp_inputFunc);

        if (inputFunc.isLua) {
            getWeakLuaRef(L, inputFunc.luaFunc);
            auto luaref = * (int*) ecs.getComponent(e, meta_luaRef);
            getWeakLuaRef(L, luaref);

            if (lua_pcall(L, 1, 0, 0)) {
                // Something bad happened
                const char* errorMessage = lua_tostring(L, -1);
                Console::getInstance().log("EE " + std::string(errorMessage));
                lua_pop(L, 1);
            }
        }
        else {
            auto func = (void(*)(Entity)) inputFunc.cFunc;
            func(e);
        }
    }
}
