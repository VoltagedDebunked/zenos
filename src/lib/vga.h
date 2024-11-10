#ifndef VGA_H
#define VGA_H

#include "types.h"

// VGA text buffer starts at this memory address
#define VGA_BUFFER (0xB8000)
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// VGA color codes
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// VGA text buffer pointer
static volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_BUFFER;

// Function to initialize VGA
static void vga_init() {
    // Clear the screen
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (VGA_COLOR_BLACK << 12) | ' ';
        }
    }
}

// Function to print a character to the screen
static void vga_put_char(char c, int x, int y, uint8_t color) {
    vga_buffer[y * VGA_WIDTH + x] = (color << 8) | c;
}

// Function to print a string to the screen
void print(const char* str, uint8_t color) {
    static int x = 0;
    static int y = 0;

    while (*str) {
        if (*str == '\n') {
            x = 0;
            y++;
        } else {
            vga_put_char(*str, x, y, color);
            x++;
            if (x >= VGA_WIDTH) {
                x = 0;
                y++;
            }
        }
        if (y >= VGA_HEIGHT) {
            y = 0; // Scroll up if we reach the bottom
        }
        str++;
    }
}

// Function to convert a number to hex string
static void hex_to_string(uint32_t value, char* buffer) {
    static const char hex_digits[] = "0123456789ABCDEF";
    buffer[0] = '0';
    buffer[1] = 'x';
    
    for (int i = 7; i >= 0; i--) {
        buffer[2 + (7 - i)] = hex_digits[(value >> (i * 4)) & 0xF];
    }
    buffer[10] = '\0';
}

// Function to print hex value
void print_hex(uint32_t value) {
    char buffer[11];  // "0x" + 8 hex digits + null terminator
    hex_to_string(value, buffer);
    print(buffer, VGA_COLOR_WHITE);
}

void vga_clear_screen() {
    // Clear the screen by filling the VGA buffer with spaces
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (VGA_COLOR_BLACK << 12) | ' '; // Set background color and clear character
        }
    }
}

#endif // VGA_H