#include "kernel.h"
#include "gdt.h"

void kernel_main() {
    gdt_init();
    while(1) {
    }
}