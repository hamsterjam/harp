/*
 * HARP
 * PoolAllocator.h
 *
 * This is a structure for allocating memory in a way ht is (sometimes) more
 * efficent. It calls malloc to allocate a large block of memory (a pool), it
 * then assigns memory by returning the first free byte in the pool. You cannot
 * free individual pointers allocated by this, instead you must free the entire
 * pool at once. If you try to allocate more memory than is left in the pool,
 * alloc() will return a null pointer
 *
 * - Callum Nicholson (hamsterjam)
 *
 */
#ifndef HARP_POOL_ALLOCATOR_H
#define HARP_POOL_ALLOCATOR_H

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

        bool failed();
};

#endif
