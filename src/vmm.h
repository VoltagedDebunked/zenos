#ifndef VMM_H
#define VMM_H

#include "lib/types.h"

// Page table entry structure
// Page directory entry structure
struct page_directory_entry {
    uint32_t present    : 1;
    uint32_t rw        : 1;
    uint32_t user      : 1;
    uint32_t pwt       : 1;    // Write-through
    uint32_t pcd       : 1;    // Cache disable
    uint32_t accessed  : 1;
    uint32_t reserved  : 1;    // Must be 0
    uint32_t ps        : 1;    // Page size (0 = 4KB, 1 = 4MB)
    uint32_t global    : 1;    // Global page
    uint32_t available : 3;    // Available for system use
    uint32_t frame     : 20;   // Frame address
} __attribute__((packed));

// Page table entry structure
struct page_table_entry {
    uint32_t present    : 1;
    uint32_t rw        : 1;
    uint32_t user      : 1;
    uint32_t pwt       : 1;    // Write-through
    uint32_t pcd       : 1;    // Cache disable
    uint32_t accessed  : 1;
    uint32_t dirty     : 1;
    uint32_t pat       : 1;    // Page Attribute Table
    uint32_t global    : 1;    // Global page
    uint32_t available : 3;    // Available for system use
    uint32_t frame     : 20;   // Frame address
} __attribute__((packed));

// Function prototypes
void vmm_init();
void* vmm_alloc(size_t size);
void vmm_free(void* ptr);

#endif // VMM_H