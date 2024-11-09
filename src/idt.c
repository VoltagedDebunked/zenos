#include "idt.h"

// IDT entries
struct idt_entry idt[256]; // 256 entries for interrupts
struct idt_ptr idtp;        // IDT pointer

// Function to set an IDT entry
void idt_set_entry(int num, uint32_t base, uint16_t selector, uint8_t type_attr) {
    idt[num].base_low = (base & 0xFFFF);         // Lower 16 bits of the base address
    idt[num].base_high = (base >> 16) & 0xFFFF;  // Upper 16 bits of the base address
    idt[num].selector = selector;                 // Segment selector
    idt[num].zero = 0;                           // Must be zero
    idt[num].type_attr = type_attr;               // Type and attributes
}

// Function to initialize the IDT
void idt_init() {
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1; // Size of IDT - 1
    idtp.base = (uint32_t)&idt; // Address of the first idt_entry

    // Clear the IDT
    for (int i = 0; i < 256; i++) {
        idt_set_entry(i, 0, 0, 0); // Initialize all entries to zero
    }

    // Load the IDT
    asm volatile("lidt %0" : : "m"(idtp));
}