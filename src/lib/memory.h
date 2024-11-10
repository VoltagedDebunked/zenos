#include "types.h"

#define MEMORY_POOL_SIZE 1024 * 1024  // 1MB
static uint8_t kernel_memory_pool[MEMORY_POOL_SIZE];
static size_t kernel_memory_offset = 0;

void* malloc(size_t size) {
    // Align size to 4-byte boundary
    size = (size + 3) & ~3;
    // Check if we have enough space
    if (kernel_memory_offset + size > MEMORY_POOL_SIZE) {
        return NULL;  // Out of memory
    }

    void* ptr = &kernel_memory_pool[kernel_memory_offset];
    kernel_memory_offset += size;
    return ptr;
}

void* memset(void* dest, int val, size_t count) {
    uint8_t* d = (uint8_t*)dest;
    for (size_t i = 0; i < count; i++) {
        d[i] = (uint8_t)val;
    }
    return dest;
}