#ifndef PMM_H
#define PMM_H

#include "lib/types.h"

// Function prototypes
void pmm_init(uint32_t total_memory);
void* pmm_alloc(size_t size);
void pmm_free(void* ptr);

#endif // PMM_H