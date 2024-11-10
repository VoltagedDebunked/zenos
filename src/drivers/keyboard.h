#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../lib/types.h"

// Function prototypes
void keyboard_init();
void keyboard_handler();
char read_char(); // Function to read a character from the buffer

#endif // KEYBOARD_H