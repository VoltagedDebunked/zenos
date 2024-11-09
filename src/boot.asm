extern kernel_main

; Multiboot header
section .multiboot
multiboot_header:
    dd 0x1BADB002            ; magic number
    dd 0x00000003            ; flags
    dd -(0x1BADB002 + 0x00000003)  ; checksum

global _start
section .text
_start:
    cli                      ; Clear interrupts
    mov esp, stack_top       ; Set up stack
    call kernel_main         ; Call kernel main function
    
    ; Halt the CPU if kernel returns
    cli
    hlt

section .bss
stack_bottom:
    resb 16384               ; 16 KB stack
stack_top: