#include <PoolAllocator.h>
#include <cstdlib>

using namespace std;

PoolAllocator::PoolAllocator(size_t poolSize) {
    this->poolSize = poolSize;
    pool = malloc(poolSize);
    if (pool == NULL) {
        this->poolSize = 0;
    }
    allocatedSize = 0;
}

PoolAllocator::~PoolAllocator() {
    if (pool) free(pool);
}

void* PoolAllocator::alloc(size_t size) {
    if (allocatedSize + size > poolSize) {
        return NULL;
    }

    // Returning null makes this more safe.
    if (size == 0) {
        return NULL;
    }

    // else

    // Cast to char* and back as pointer arithmetic on void* is technically UB.
    void* ret = (void*) ((char*)pool + allocatedSize);
    allocatedSize += size;

    return ret;
}

void PoolAllocator::freeAll() {
    allocatedSize = 0;
}

bool PoolAllocator::failed() {
    return !pool;
}
