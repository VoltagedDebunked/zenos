#include "vfs.h"     // For filesystem operations
#include "memory.h"  // For memory management
#include "elf.h" // For process management

// Function to execute an ELF file
int exec(const char* path) {
    // Open the ELF file
    struct vfs_file* elf_file = vfs_open(path);
    if (!elf_file) {
        print("Error: Could not open ELF file.\n");
        return -1;
    }

    // Read the ELF header
    struct elf_header elf;
    vfs_read(elf_file, &elf, sizeof(elf));

    // Check ELF magic number
    if (elf.magic != 0x464C457F) { // Check for ELF magic
        print("Error: Not a valid ELF file.\n");
        vfs_close(elf_file);
        return -1;
    }

    // Allocate memory for the process
    void* process_memory = allocate_process_memory(elf.mem_size);
    if (!process_memory) {
        print("Error: Could not allocate memory for process.\n");
        vfs_close(elf_file);
        return -1;
    }

    // Load program headers
    for (int i = 0; i < elf.ph_count; i++) {
        struct elf_program_header ph;
        vfs_read(elf_file, &ph, sizeof(ph));

        if (ph.type == 1) { // PT_LOAD
            // Load the segment into memory
            vfs_seek(elf_file, ph.offset);
            vfs_read(elf_file, process_memory + ph.vaddr, ph.file_size);
            // Zero-fill the remaining memory
            memset(process_memory + ph.vaddr + ph.file_size, 0, ph.mem_size - ph.file_size);
        }
    }

    // Create a new process
    create_process(elf.entry , process_memory); // Pass the entry point and allocated memory to create a new process

    vfs_close(elf_file); // Close the ELF file
    return 0; // Success
}

// Example usage in the execute_command function
void execute_command(char* command) {
    char* args[MAX_ARGS];
    int argc = parse_command(command, args);

    if (argc == 0) {
        return; // No command entered
    }

    // Construct the path to the executable
    char path[MAX_COMMAND_LENGTH];
    strcpy(path, "/bin/");
    strcat(path, args[0]);

    // Attempt to execute the command
    if (vfs_file_exists(path)) {
        // Call the exec function to execute the ELF binary
        if (exec(path) == 0) {
            print("Executing: ");
            print(path);
            print("\n");
        } else {
            print("Failed to execute: ");
            print(args[0]);
            print("\n");
        }
    } else {
        print("Command not found: ");
        print(args[0]);
        print("\n");
    }
}