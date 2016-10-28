#ifndef __GALAXY_FORT_ECS_H
#define __GALAXY_FORT_ECS_H

#include <initializer_list>

typedef unsigned int Entity;
typedef unsigned int Component;

class ECS {
    private:
        Entity    nextEnt;
        Component nextComp;

        unsigned int entVecLength;
        unsigned int compVecLength;

        std::size_t* compSize;
        bool** hasComp;
        void** data;

    public:
        ECS(unsigned int entVecLength, unsigned int compVecLength);
        ~ECS();

        Entity createEntity();
        void destroyEntity(Entity ent);

        Component createComponent(std::size_t size);

        void  setComponent(Entity ent, Component comp, void* val);
        void* getComponent(Entity ent, Component comp);
        void  removeComponent(Entity ent, Component comp);

        Entity begin(std::initializer_list<Component> comps);
        Entity next(std::initializer_list<Component> comps);
        Entity end();
};

#endif
