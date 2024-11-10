#include "gdt.h"

// GDT entries
struct gdt_entry gdt[3]; // 3 entries: null, code, data
struct gdt_ptr gdtp;      // GDT pointer

// Function to set a GDT entry
void gdt_set_entry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F) | (granularity & 0xF0);
    gdt[num].access = access;
}

// Function to initialize the GDT
void gdt_init() {
    gdtp.limit = (sizeof(struct gdt_entry) * 3) - 1; // Size of GDT - 1
    gdtp.base = (uint32_t)&gdt; // Address of the first gdt_entry

    // Null descriptor
    gdt_set_entry(0, 0, 0, 0, 0);

    // Code segment descriptor
    gdt_set_entry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Present, executable, readable

    // Data segment descriptor
    gdt_set_entry(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Present, writable

    // Load the GDT
    asm volatile("lgdt (%0)" : : "r"(&gdtp));
    
    // Update segment registers
    asm volatile("mov $0x10, %%ax; \
              mov %%ax, %%ds; \
              mov %%ax, %%es; \
              mov %%ax, %%fs; \
              mov %%ax, %%gs; \
              ljmp $0x08, $next; \
              next:": : : "eax");
}