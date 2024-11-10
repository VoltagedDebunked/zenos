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
volatile uint16_t* vga_buffer = (volatile uint16_t*)VGA_BUFFER;

// Function to initialize VGA
void vga_init() {
    // Clear the screen
    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = (VGA_COLOR_BLACK << 12) | ' ';
        }
    }
}

// Function to print a character to the screen
void vga_put_char(char c, int x, int y, uint8_t color) {
    vga_buffer[y * VGA_WIDTH + x] = (color << 8) | c;
}

// Function to print a string to the screen
void print(const char* str) {
    static int x = 0;
    static int y = 0;
    uint8_t color = VGA_COLOR_LIGHT_GREY; // Set default color

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

#endif // VGA_H