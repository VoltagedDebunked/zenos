#include "shell.h"
#include "kernel.h"
#include "lib/types.h"
#include "lib/string.h"

#define MAX_COMMAND_LENGTH 256
#define MAX_ARGUMENTS 10

// Function prototypes
void execute_command(const char* command);
void echo_command(const char* args);
void help_command();

// List of commands
const char* commands[] = {
    "echo",
    "help",
    // Add more commands here
};

void init_shell() {
    kprint("Shell initialized.\n", vga_make_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
}

void run_shell() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        kprint("zenos> ", vga_make_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));  // Shell prompt
        read_line(command, sizeof(command));  // Read command from user
        execute_command(command);
    }
}

void execute_command(const char* command) {
    // Split command into command and arguments
    char* args[MAX_ARGUMENTS];
    int arg_count = 0;
    char* token = strtok(command, " ");
    
    while (token != NULL && arg_count < MAX_ARGUMENTS) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
    }

    // Handle commands
    if (arg_count > 0) {
        if (strcmp(args[0], "echo") == 0) {
            echo_command(args[1]);  // Pass the argument to echo
        } else if (strcmp(args[0], "help") == 0) {
            help_command();
        } else {
            kprint("Unknown command: ", vga_make_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
            kprint(args[0], vga_make_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
            kprint("\n", vga_make_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
        }
    }
}

void echo_command(const char* args) {
    if (args) {
        kprint(args, vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
        kprint("\n", vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    } else {
        kprint("\n", vga_make_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    }
}

void help_command() {
    kprint("Available commands:\n", vga_make_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
    kprint("  echo [text] - Display the text\n", vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    kprint("  help        - Show this help message\n", vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    // List other commands here
}