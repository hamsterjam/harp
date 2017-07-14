/*
 * HARP
 * DynamicPoolAllocator.h
 *
 * This is like a PoolAllocator, but if we run out of room in the pool, we
 * don't just return NULL, instead we allocate a second pool. There are rules
 * for changing the size of pools as well as the number of pools that result in
 * it attempting to maintain 2 pools giving us enough space for all our data.
 * The rules are as follows:
 *
 * alloc()
 * -------
 * If we run out of space, double the number of pools.
 *
 *
 * freeAll()
 * ---------
 * If we have 1 or 2 pools and we are using less than a quarter of available
 * space then half the size of the pools.
 *
 * If we have 4 or more pools and less than a quarter of them are full, then
 * half the number of pools.
 *
 * If we have 4 or more pools and over a quarter of them are full, then half
 * the number of pools, but double their size.
 *
 * - Callum Nicholson (hamsterjam)
 *
 */

#ifndef HARP_DYNAMIC_POOL_ALLOCATOR_H
#define HARP_DYNAMIC_POOL_ALLOCATOR_H

#include <cstddef>

class DynamicPoolAllocator {
    private:
        void** pools;

        unsigned int numPools;
        std::size_t poolSize;
        std::size_t originalPoolSize;

        unsigned int filledPools;
        std::size_t currentPoolAllocatedSize;

        void increasePools();
        void decreasePools();
        void consolidatePools();
        void splitPools();

    public:
        DynamicPoolAllocator(std::size_t poolSize);
        ~DynamicPoolAllocator();

        void* alloc(std::size_t size);
        void freeAll();
};

#endif
