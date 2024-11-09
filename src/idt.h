#ifndef IDT_H
#define IDT_H

#include "lib/types.h"

// IDT entry structure
struct idt_entry {
    uint16_t base_low;    // Lower 16 bits of the handler function address
    uint16_t selector;     // Kernel segment selector
    uint8_t  zero;         // This must always be zero
    uint8_t  type_attr;    // Type and attributes
    uint16_t base_high;    // Upper 16 bits of the handler function address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    uint16_t limit;        // Size of the IDT - 1
    uint32_t base;         // Address of the first idt_entry
} __attribute__((packed));

// Function to initialize the IDT
void idt_init();

// Function to set an IDT entry
void idt_set_entry(int num, uint32_t base, uint16_t selector, uint8_t type_attr);

#endif // IDT_H