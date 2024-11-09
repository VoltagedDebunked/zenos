#include "vga.h"

// VGA text buffer location
static const uint16_t VGA_WIDTH = 80;
static const uint16_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

// Current cursor position
static uint16_t cursor_row = 0;
static uint16_t cursor_col = 0;

uint8_t vga_make_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

void vga_initialize() {
    cursor_row = 0;
    cursor_col = 0;
}

void vga_clear_screen() {
    uint8_t default_color = vga_make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    for (uint16_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            const uint16_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = ' ' | default_color << 8;
        }
    }
    
    cursor_row = 0;
    cursor_col = 0;
}

void vga_write_char(char c, uint8_t color) {
    // Handle newline
    if (c == '\n') {
        cursor_row++;
        cursor_col = 0;
        return;
    }
    
    // If we've reached the end of the screen, scroll
    if (cursor_row >= VGA_HEIGHT) {
        // Simple scroll: move everything up one line
        for (uint16_t y = 1; y < VGA_HEIGHT; y++) {
            for (uint16_t x = 0; x < VGA_WIDTH; x++) {
                VGA_MEMORY[(y-1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];
            }
        }
        
        // Clear the last line
        uint8_t default_color = vga_make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
        for (uint16_t x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(VGA_HEIGHT-1) * VGA_WIDTH + x] = ' ' | default_color << 8;
        }
        
        cursor_row = VGA_HEIGHT - 1;
        cursor_col = 0;
    }
    
    // Write character
    const uint16_t index = cursor_row * VGA_WIDTH + cursor_col;
    VGA_MEMORY[index] = c | color << 8;
    
    // Move cursor
    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        cursor_row++;
        cursor_col = 0;
    }
}

void vga_write_string(const char* str, uint8_t color) {
    while (*str) {
        vga_write_char(*str, color);
        str++;
    }
}