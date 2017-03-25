#ifndef __GALAXY_FORT_POOL_ALLOCATOR_H
#define __GALAXY_FORT_POOL_ALLOCATOR_H

#include <cstddef>

class PoolAllocator {
    private:
        void* pool;
        std::size_t poolSize;
        std::size_t allocatedSize;

    public:
        PoolAllocator(std::size_t poolSize);
        ~PoolAllocator();

        void* alloc(std::size_t size);
        void freeAll();
};

#endif
