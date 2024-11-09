#include "kernel.h"
#include "drivers/keyboard.h"

void kernel_main() {
    kernel_init();
    keyboard_init();

    // Kernel loop
    while(1) {
    }
}