#ifndef clox_memory_h
#define clox_memory_h

#include "object.h"

#define GROW_CAPACITY(capacity) \
    ((capacity) < 8 ? 8 : (capacity)*2)

#define GROW_ARRAY(pointer, type, oldCount, count) \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (count))

#define FREE_ARRAY(pointer, type, count) \
    reallocate(pointer, sizeof(type) * (count), 0)

#define ALLOCATE(type, count) \
    (type *)reallocate(NULL, 0, sizeof(type) * (count))

#define FREE(pointer, type) \
    reallocate(pointer, sizeof(type), 0)

void *reallocate(void *previous, size_t oldSize, size_t newSize);
void freeObjects();

#endif
