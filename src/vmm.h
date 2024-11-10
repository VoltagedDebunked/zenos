#ifndef VMM_H
#define VMM_H

#include "lib/types.h"

// Page table entry structure
struct page_table_entry {
    uint32_t present : 1;  // Page present in memory
    uint32_t rw : 1;       // Read/Write
    uint32_t user : 1;     // User/Supervisor
    uint32_t accessed : 1; // Accessed
    uint32_t dirty : 1;    // Dirty
    uint32_t frame : 20;   // Frame address
} __attribute__((packed));

// Page directory entry structure
struct page_directory_entry {
    uint32_t present : 1;  // Page directory present
    uint32_t rw : 1;       // Read/Write
    uint32_t user : 1;     // User/Supervisor
    uint32_t accessed : 1; // Accessed
    uint32_t reserved : 1; // Reserved
    uint32_t frame : 20;   // Frame address
} __attribute__((packed));

// Function prototypes
void vmm_init();
void* vmm_alloc(size_t size);
void vmm_free(void* ptr);

#endif // VMM_H