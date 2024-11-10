#ifndef VFS_H
#define VFS_H

#include "lib/types.h"

// Define the file structure
struct vfs_file {
    char* name;           // Name of the file
    size_t size;         // Size of the file
    void* data;          // Pointer to file data
};

// Define the directory structure
struct vfs_directory {
    char* name;          // Name of the directory
    struct vfs_file** files; // Pointer to an array of file pointers
    size_t file_count;   // Number of files in the directory
    struct vfs_directory** directories; // Pointer to an array of subdirectory pointers
    size_t dir_count;    // Number of subdirectories
};

// Function prototypes
void vfs_init();
struct vfs_file* vfs_create_file(const char* name, size_t size);
void vfs_delete_file(const char* name);
void* vfs_read_file(const char* name);
void vfs_write_file(const char* name, const void* data, size_t size);
struct vfs_directory* vfs_create_directory(const char* name);
void vfs_delete_directory(const char* name);
struct vfs_directory* vfs_get_directory(const char* name);
void vfs_list_directory(const char* name);

#endif // VFS_H