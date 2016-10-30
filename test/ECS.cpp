#include <iostream>
#include <cassert>
#include <ECS.h>

using namespace std;

struct vec {
    int x;
    int y;
    int z;
};

int main() {
    // First create the ECS
    // Usign entVecLength = compVecLength = 1 to test auto expansion
    ECS ecs(1, 1);

    // Create some components, one with a primative, the other with a struct
    Component intComp = ecs.createComponent(sizeof(int));
    Component vecComp = ecs.createComponent(sizeof(vec));

    // Just make a bunch of entities
    Entity ent1 = ecs.createEntity();
    Entity ent2 = ecs.createEntity();
    Entity ent3 = ecs.createEntity();
    Entity ent4 = ecs.createEntity();

    // Before anything else, we need to make sure these don't have components
    {
        void* intOut = ecs.getComponent(ent1, intComp);
        void* vecOut = ecs.getComponent(ent1, vecComp);

        assert(!intOut);
        assert(!vecOut);
    }

    // Simple store and recall test
    {
        int input = 1;
        ecs.setComponent(ent1, intComp, &input);

        // This looks a little awkward, remember that it gives you a void* which you
        // have to cast and dereference yourself, thats all I'm doing
        //
        // While I'm on the subject, it's a good idea to check that it doesn't return null.
        int output = *((int*) ecs.getComponent(ent1, intComp));

        assert(input == output);
    }
    // Same thing with the struct component
    {
        vec input;
        input.x = 11;
        input.y = 12;
        input.z = 13;
        ecs.setComponent(ent1, vecComp, &input);

        vec output = *((vec*)ecs.getComponent(ent1, vecComp));

        assert(input.x == output.x);
        assert(input.y == output.y);
        assert(input.z == output.z);
    }

    // Now test removing components
    {
        ecs.removeComponent(ent1, intComp);
        void* intOut = ecs.getComponent(ent1, intComp);

        assert(!intOut);
    }

    // Now delete the entity
    {
        ecs.deleteEntity(ent1);
        void* vecOut = ecs.getComponent(ent1, vecComp);

        assert(!vecOut);
    }

    // Great, lets quickly populate the rest of the ents
    {
        int inputInt;
        vec inputVec;

        inputInt = 2;
        ecs.setComponent(ent2, intComp, &inputInt);

        inputInt = 3;
        ecs.setComponent(ent3, intComp, &inputInt);

        inputVec.x = 31;
        inputVec.y = 32;
        inputVec.z = 33;
        ecs.setComponent(ent3, vecComp, &inputVec);

        inputVec.x = 41;
        inputVec.y = 42;
        inputVec.z = 43;
        ecs.setComponent(ent4, vecComp, &inputVec);
    }

    // Now check to see if the system functions work
    {
        int entCount = 0;
        for (auto it = ecs.begin({intComp}); it != ecs.end(); it++) {
            // We only care about which entities are being selected, the component
            // values are correct (from previous tests)
            Entity e = *it;
            ++entCount;

            assert(e == ent2 || e == ent3);
        }
        // Should also get exactly 2 entities (2 and 3)
        assert(entCount == 2);
    }

    // Same thing for a requiring two components
    {
        int entCount = 0;
        for (auto it = ecs.begin({intComp, vecComp}); it != ecs.end(); it++) {
            Entity e = *it;
            ++entCount;

            assert(e == ent3);
        }
        assert(entCount == 1);
    }

    // Could probably be more exhaustive, but that's enough I think.
    cout << "Everything looks good!" << endl;

    return 0;
}
