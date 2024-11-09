#ifndef KERNEL_H
#define KERNEL_H

#include "lib/types.h"
#include "vga.h"

// Kernel initialization functions
void kernel_init();
void kernel_main();

// Basic output function
void kprint(const char* message, uint8_t color);

#endif // KERNEL_H