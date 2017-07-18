#include <ECS.h>

#include <cstdlib>
#include <cstring> // for memcpy

#include <DynamicPoolAllocator.h>

using namespace std;

bool ECS::hasComponent(Entity ent, Component comp) {
    if (hasComp[comp][ent]) return true;

    // Otherwise go up the parent chain looking for one
    if (parent[ent] == ent) return false;
    return hasComponent(parent[ent], comp);
}

ECS::ECS(unsigned int entVecLength, unsigned int compVecLength, size_t poolSize):
        changePool(poolSize) {
    this->entVecLength  = entVecLength;
    this->compVecLength = compVecLength;

    nextEnt  = 0;
    nextComp = 0;

    changeQueue  = 0;
    newestChange = 0;

    // Allocate arrays
    compSize = (size_t*) malloc(sizeof(size_t) * compVecLength);
    hasComp  = (bool**)  malloc(sizeof(bool*)  * compVecLength);
    parent   = (Entity*) malloc(sizeof(Entity) * entVecLength);
    data     = (void**)  malloc(sizeof(void*)  * compVecLength);

    for (int i=0; i < compVecLength; ++i) {
        hasComp[i] = (bool*) malloc(sizeof(bool) * entVecLength);
    }

    // hasComp will be init'd by createEntity, as will parent
}

ECS::~ECS() {
    free(compSize);
    free(parent);

    for (int i=0; i < nextComp; ++i) {
        free(hasComp[i]);
        free(data[i]);
    }

    free(hasComp);
    free(data);
}

//
// Entity functions
//

Entity ECS::createEntity() {
    // Check our recycle queue first
    if (!entRecycleQueue.empty()) {
        // Don't have to resize or init anything, that's all taken care of when it
        // was originally init'd
        Entity ret = entRecycleQueue.front();
        entRecycleQueue.pop();
        return ret;
    }

    // Resize arrays if we need to
    if (nextEnt >= entVecLength) {
        entVecLength *= 2;

        parent = (Entity*) realloc(parent, sizeof(Entity) * entVecLength);
        for(int i=0; i < nextComp; ++i) {
            hasComp[i] = (bool*) realloc(hasComp[i], sizeof(bool) * entVecLength);
            data[i] = realloc(data[i], compSize[i] * entVecLength);
        }
    }

    // init the new ent's entries in hasComp
    for (int i=0; i < nextComp; ++i) {
        hasComp[i][nextEnt] = false;
    }

    // init the parent (to itself)
    parent[nextEnt] = nextEnt;

    return nextEnt++;
}

void ECS::deleteEntity(Entity ent) {
    // Just setting it to have no components and no parent
    for (int i=0; i < nextComp; ++i) {
        hasComp[i][ent] = false;
    }
    parent[ent] = ent;

    // Add the entity to the recycle queue
    entRecycleQueue.push(ent);
}

//
// Component Functions
//

Component ECS::createComponentType(size_t size) {
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
    // We need to allocate new memory for the data so we have ownership

    size_t size = compSize[comp];

    // Create the change request and add it to the queue
    ChangeRequest* req = (ChangeRequest*) changePool.alloc(sizeof(ChangeRequest) + size);
    req->ent = ent;
    req->comp = comp;
    req->next = 0;
    req->remove = false;
    memcpy(&req->val, val, size);

    if (!changeQueue) {
        changeQueue = req;
    }
    else {
        newestChange->next = req;
    }
    newestChange = req;
}

void* ECS::getComponent(Entity ent, Component comp) {
    // First thing to do is to check to see if it actually HAS the comp
    if (!hasComponent(ent, comp)) {
        return NULL;
    }
    if (parent[ent] != ent) {
        void* ret = getComponent(parent[ent], comp);
        if (ret) return ret;
    }

    size_t size = compSize[comp];

    // Cast to char to do clean pointer arithmetic
    return (char*) data[comp] + size * ent;
}

void ECS::removeComponent(Entity ent, Component comp) {
    // Just set the hasComp to false, don't bother with the data

    ChangeRequest* req = (ChangeRequest*) changePool.alloc(sizeof(ChangeRequest));
    req->ent = ent;
    req->comp = comp;
    req->next = 0;
    req->remove = true;

    if (!changeQueue) {
        changeQueue = req;
    }
    else {
        newestChange->next = req;
    }
    newestChange = req;
}

void ECS::updateComponents() {
    while(changeQueue) {
        ChangeRequest& req = *changeQueue;

        if (req.remove) {
            hasComp[req.comp][req.ent] = false;
        }
        else {
            hasComp[req.comp][req.ent] = true;
            size_t size = compSize[req.comp];
            memcpy(((char*) data[req.comp]) + size * req.ent, &req.val, size);
        }

        changeQueue = req.next;
        // Don't need to free anything, this is a pool allocator
    }

    changePool.freeAll();
    changeQueue  = 0;
    newestChange = 0;
}

//
// Zero sized components (Flags)
//

Component ECS::createFlagType() {
    return createComponentType(0);
}

void ECS::setFlag(Entity ent, Component flag, bool val) {
    if (val) {
        setComponent(ent, flag, NULL);
    }
    else {
        removeComponent(ent, flag);
    }
}

bool ECS::getFlag(Entity ent, Component flag) {
    return hasComponent(ent, flag);
}

//
// Parents
//

void ECS::setParent(Entity ent, Entity par) {
    parent[ent] = par;
}

void ECS::removeParent(Entity ent) {
    parent[ent] = ent;
}

void* ECS::getChildComponent(Entity ent, Component comp) {
    // Make sure we check the entry for ent WITHOUT going up the parent chain
    if (!hasComp[comp][ent]) {
        return NULL;
    }

    size_t size = compSize[comp];
    // Cast to char* to do clean pointer arithmetic!
    return (char*) data[comp] + size * ent;
}

bool ECS::getChildFlag(Entity ent, Component flag) {
    return getChildComponent(ent, flag);
}

//
// System functions
//

ECS::EntityIterator ECS::begin(initializer_list<Component> comps) {
    EntityIterator ret(comps, nextEnt, false, this);
    return ++ret;
}

ECS::EntityIterator ECS::beginParented(initializer_list<Component> comps) {
    EntityIterator ret(comps, nextEnt, true, this);
    return ++ret;
}

ECS::EntityIterator ECS::end() {
    EntityIterator ret({}, nextEnt, false, this);
    return ret;
}

//
// EntityIterator
//

ECS::EntityIterator::EntityIterator(initializer_list<Component> compList, Entity ent, bool parented, ECS* par):
    comps(compList)
{
    this->ent = ent;
    this->parented = parented;
    this->par = par;
}

bool ECS::EntityIterator::operator==(EntityIterator rhs) {
    EntityIterator lhs = *this;
    return lhs.ent == rhs.ent;
}

ECS::EntityIterator ECS::EntityIterator::operator++() {
    // Just iterate through to find the next one

    Entity checkFrom = (this->ent == par->nextEnt) ? 0 : this->ent + 1;
    this->ent = par->nextEnt;

    for (Entity e = checkFrom; e < par->nextEnt; ++e) {

        bool exit = true;

        for (Component c: this->comps) {
            if ((parented) ? !par->hasComponent(e, c) : !par->hasComp[c][e]) {
                exit = false;
                break;
            }
        }

        if (exit) {
            this->ent = e;
            break;
        }
    }

    return *this;
}

ECS::EntityIterator ECS::EntityIterator::operator++(int) {
    EntityIterator ret = *this;
    ++(*this);
    return ret;
}
