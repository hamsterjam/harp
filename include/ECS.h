/*
 * HARP
 * ECS.h
 *
 * This is my implementation of an Entity Component System pattern. For each
 * Entity it assigns enough space to store one of each Component. Because it
 * assigns space for ALL components, it is probably a good idea to use a
 * seperate ECS for entities of different functions (for example, you could
 * have one for UI elements, and one for game elements). Changes are cached (in
 * a DynamicPoolAllocator) and only actually updated when updateComponents() is
 * called. This means you can run systems on the data in any order you like and
 * it should be easy to make thread safe if I want to (which I probably do)
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_ECS_H
#define HARP_ECS_H
#include <cstddef>
#include <initializer_list>
#include <vector>
#include <queue>

#include <DynamicPoolAllocator.h>

typedef unsigned int Entity;
typedef unsigned int Component;

class ECS {
    private:
        struct ChangeRequest {
            Entity ent;
            Component comp;
            ChangeRequest* next;
            bool remove;
            char val[0];
        };

        Entity    nextEnt;
        Component nextComp;

        unsigned int entVecLength;
        unsigned int compVecLength;

        std::queue<Entity> entRecycleQueue;
        ChangeRequest* changeQueue;
        ChangeRequest* newestChange;

        DynamicPoolAllocator changePool;

        std::size_t* compSize;
        bool**       hasComp;
        Entity*      parent;
        void**       data;

        bool hasComponent(Entity ent, Component comp);

    public:
        class EntityIterator {
            private:
                friend class ECS;

                std::vector<Component> comps;

                Entity ent;
                ECS*   par;
                bool   parented;
            public:
                EntityIterator(std::initializer_list<Component> compList, Entity ent, bool parented, ECS* par);
                bool operator==(EntityIterator rhs);
                bool operator!=(EntityIterator rhs) {return !((*this)==rhs);}
                EntityIterator operator++();    // Prefix
                EntityIterator operator++(int); // Postfix
                Entity operator*() {return ent;}
        };

        ECS(unsigned int entVecLength, unsigned int compVecLength, std::size_t poolSize);
        ~ECS();

        Entity createEntity();
        void deleteEntity(Entity ent);

        Component createComponentType(std::size_t size);

        void  setComponent(Entity ent, Component comp, void* val);
        void* getComponent(Entity ent, Component comp);
        void  removeComponent(Entity ent, Component comp);

        void  updateComponents();

        // Zero sized components
        //
        // Technically these work by passing in NULL for the value and 0 for the size, but
        // adding in some wrappers makes it more... nice? I guess?

        Component createFlagType();
        void setFlag(Entity ent, Component flag, bool val);
        bool getFlag(Entity ent, Component flag);

        //
        // Parents
        //

        void  setParent(Entity ent, Entity par);
        void  removeParent(Entity ent);
        void* getChildComponent(Entity ent, Component comp);
        bool  getChildFlag(Entity ent, Component flag);

        EntityIterator begin(std::initializer_list<Component> comps);
        EntityIterator beginParented(std::initializer_list<Component> comps);
        EntityIterator end();
};

#endif
