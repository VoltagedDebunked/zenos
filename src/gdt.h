#ifndef GDT_H
#define GDT_H

#include "lib/types.h"

// GDT descriptor structure
struct gdt_entry {
    uint16_t limit_low;    // Lower 16 bits of the limit
    uint16_t base_low;     // Lower 16 bits of the base
    uint8_t base_middle;    // Next 8 bits of the base
    uint8_t access;        // Access flags
    uint8_t granularity;   // Granularity flags
    uint8_t base_high;     // Last 8 bits of the base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    uint16_t limit;        // Size of the GDT - 1
    uint32_t base;         // Address of the first gdt_entry
} __attribute__((packed));

// Function to initialize the GDT
void gdt_init();

#endif // GDT_H