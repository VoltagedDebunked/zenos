#include "kernel.h"
#include "shell.h"

void kernel_init() {
    // Initialize VGA text mode
    vga_initialize();
    
    // Clear screen
    vga_clear_screen();

    // Shell Functions
    init_shell();
    run_shell();
}

void kernel_main() {
    // Perform kernel initialization
    kernel_init();
    
    // Kernel loop
    while(1) {
    }
}

void kprint(const char* message, uint8_t color) {
    vga_write_string(message, color);
}