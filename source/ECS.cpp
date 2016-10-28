#include <ECS.h>
#include <cstdlib>
#include <cstring> // for memcpy

using namespace std;

ECS::ECS(unsigned int entVecLength, unsigned int compVecLength) {
    this->entVecLength  = entVecLength;
    this->compVecLength = compVecLength;

    nextEnt  = 0;
    nextComp = 0;

    // Allocate arrays
    compSize = (size_t*) malloc(sizeof(size_t) * compVecLength);
    hasComp = (bool**) malloc(sizeof(bool*) * compVecLength);
    data = (void**) malloc(sizeof(void*) * compVecLength);

    for (int i=0; i < compVecLength; ++i) {
        hasComp[i] = (bool*) malloc(sizeof(bool) * entVecLength);
    }

    // hasComp will be init'd by createEntity
}

ECS::~ECS() {
    free(compSize);

    for (int i=0; i < nextComp; ++i) {
        free(hasComp[i]);
        free(data[i]);
    }

    free(hasComp);
    free(data);
}

Entity ECS::createEntity() {
    // Resize arrays if we need to
    if (nextEnt >= entVecLength) {
        entVecLength *= 2;
        for(int i=0; i < nextComp; ++i) {
            hasComp[i] = (bool*) realloc(hasComp[i], sizeof(bool) * entVecLength);
            data[i] = realloc(data[i], compSize[i] * entVecLength);
        }
    }

    // init the new ent's entries in hasComp
    for (int i=0; i < nextComp; ++i) {
        hasComp[i][nextEnt] = false;
    }

    return nextEnt++;
}

void ECS::destroyEntity(Entity ent) {
    // Just setting it to having no components should be good enough
    // Later you want to add this ID to a list so it can be reassigned
    for (int i=0; i < nextComp; ++i) {
        hasComp[i][ent] = false;
    }
}

Component ECS::createComponent(size_t size) {
    // Resize arrays if we need to
    if (nextComp >= compVecLength) {
        compVecLength *= 2;

        compSize = (size_t*) realloc(compSize, sizeof(size_t) * compVecLength);
        hasComp = (bool**) realloc(hasComp, sizeof(bool*) * compVecLength);
        data = (void**) realloc(data, sizeof(void*) * compVecLength);

        for (int i = nextComp; i < compVecLength; ++i) {
            hasComp[i] = (bool*) malloc(sizeof(bool) * entVecLength);
        }
    }

    // Make sure all entities don't have this
    for (int i=0; i < nextEnt; ++i) {
        hasComp[nextComp][i] = false;
    }

    // Allocate our data yo
    compSize[nextComp] = size;
    data[nextComp] = malloc(size * entVecLength);

    return nextComp++;
}

void ECS::setComponent(Entity ent, Component comp, void* val) {
    // Make sure you set it to actually having the component
    hasComp[comp][ent] = true;

    // Now just memcpy it into place, easy
    size_t size = compSize[comp];
    // Cast to char to do clean pointer arithmetic
    memcpy(((char*) data[comp]) + size * ent, val, size);
}

void* ECS::getComponent(Entity ent, Component comp) {
    // First thing to do is to check to see if it actually HAS the comp
    if (!hasComp[comp][ent]) {
        return NULL;
    }
    size_t size = compSize[comp];

    // Cast to char to do clean pointer arithmetic
    return ((char*) data[comp]) + size * ent;
}

void ECS::removeComponent(Entity ent, Component comp) {
    // Just set the hasComp to false, don't bother with the data

    hasComp[comp][ent] = false;
}
