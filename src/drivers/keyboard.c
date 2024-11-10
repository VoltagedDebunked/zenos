#include "keyboard.h"
#include "../lib/io.h"
#include "../lib/vga.h"
#include "../pic.h"

// Keyboard port
#define KEYBOARD_PORT 0x60

// Scancodes for keyboard keys
static const char scancode_to_ascii[128] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 
    0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, 
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Buffer for storing input characters
#define BUFFER_SIZE 128
static char input_buffer[BUFFER_SIZE];
static int buffer_index = 0;

// Initialize keyboard
void keyboard_init() {
    // Enable keyboard interrupts
    outb(PIC1_DATA, 0xFD); // Allow only keyboard interrupts
}

// Keyboard interrupt handler
void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_PORT);
    
    // Check if scancode is in the valid range
    if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c) {
            // Store the character in the input buffer
            if (buffer_index < BUFFER_SIZE - 1) {
                input_buffer[buffer_index++] = c;
                input_buffer[buffer_index] = '\0'; // Null-terminate the string
            }
            print(&c); // Optionally print the character to the screen
        }
    }
}

// Read a character from the input buffer
char read_char() {
    if (buffer_index > 0) {
        char c = input_buffer[0];
        // Shift the buffer
        for (int i = 1; i < buffer_index; i++) {
            input_buffer[i - 1] = input_buffer[i];
        }
        buffer_index--; // Decrease buffer index
        input_buffer[buffer_index] = '\0'; // Null-terminate the string
        return c;
    }
    return 0; // Return 0 if no characters are available
}