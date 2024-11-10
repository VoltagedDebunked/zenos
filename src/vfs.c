#include "vfs.h"
#include "lib/memory.h"
#include "lib/string.h"
#include "lib/vga.h"

#define MAX_FILES 128
#define MAX_DIRS  128

static struct vfs_directory root_directory;

// Initialize the VFS
void vfs_init() {
    root_directory.name = "root";
    root_directory.files = (struct vfs_file**)malloc(sizeof(struct vfs_file*) * MAX_FILES);
    root_directory.directories = (struct vfs_directory**)malloc(sizeof(struct vfs_directory*) * MAX_DIRS);
    root_directory.file_count = 0;
    root_directory.dir_count = 0;
}

// Create a new file in the VFS
struct vfs_file* vfs_create_file(const char* name, size_t size) {
    if (root_directory.file_count >= MAX_FILES) {
        return NULL; // Maximum file limit reached
    }

    struct vfs_file* file = (struct vfs_file*)malloc(sizeof(struct vfs_file));
    file->name = (char*)malloc(strlen(name) + 1);
    strcpy(file->name, name);
    file->size = size;
    file->data = malloc(size); // Allocate memory for file data
    root_directory.files[root_directory.file_count++] = file;
    return file;
}

// Delete a file from the VFS
void vfs_delete_file(const char* name) {
    for (size_t i = 0; i < root_directory.file_count; i++) {
        if (strcmp(root_directory.files[i]->name, name) == 0) {
            free(root_directory.files[i]->name);
            free(root_directory.files[i]->data);
            free(root_directory.files[i]);
            // Shift remaining files
            for (size_t j = i; j < root_directory.file_count - 1; j++) {
                root_directory.files[j] = root_directory.files[j + 1];
            }
            root_directory.file_count--;
            return;
        }
    }
}

// Read data from a file
void* vfs_read_file(const char* name) {
    for (size_t i = 0; i < root_directory.file_count; i++) {
        if (strcmp(root_directory.files[i]->name, name) == 0) {
            return root_directory.files[i]->data; // Return pointer to file data
        }
    }
    return NULL; // File not found
}

// Write data to a file
void vfs_write_file(const char* name, const void* data, size_t size) {
    for (size_t i = 0; i < root_directory.file_count; i++) {
        if (strcmp(root_directory.files[i]->name, name) == 0) {
            // Resize the file if necessary
            if (size > root_directory.files[i]->size) {
                root_directory.files[i]->data = realloc(root_directory.files[i]->data, size);
                root_directory.files[i]->size = size;
            }
            memcpy(root_directory.files [i]->data, data, size); // Copy data to file
            return;
        }
    }
}

// Create a new directory in the VFS
struct vfs_directory* vfs_create_directory(const char* name) {
    if (root_directory.dir_count >= MAX_DIRS) {
        return NULL; // Maximum directory limit reached
    }

    struct vfs_directory* dir = (struct vfs_directory*)malloc(sizeof(struct vfs_directory));
    dir->name = (char*)malloc(strlen(name) + 1);
    strcpy(dir->name, name);
    dir->files = (struct vfs_file**)malloc(sizeof(struct vfs_file*) * MAX_FILES);
    dir->directories = (struct vfs_directory**)malloc(sizeof(struct vfs_directory*) * MAX_DIRS);
    dir->file_count = 0;
    dir->dir_count = 0;
    root_directory.directories[root_directory.dir_count++] = dir;
    return dir;
}

// Delete a directory from the VFS
void vfs_delete_directory(const char* name) {
    for (size_t i = 0; i < root_directory.dir_count; i++) {
        if (strcmp(root_directory.directories[i]->name, name) == 0) {
            free(root_directory.directories[i]->name);
            free(root_directory.directories[i]->files);
            free(root_directory.directories[i]->directories);
            free(root_directory.directories[i]);
            // Shift remaining directories
            for (size_t j = i; j < root_directory.dir_count - 1; j++) {
                root_directory.directories[j] = root_directory.directories[j + 1];
            }
            root_directory.dir_count--;
            return;
        }
    }
}

// List contents of a directory
void vfs_list_directory(const char* name) {
    struct vfs_directory* dir = vfs_get_directory(name);
    if (dir) {
        for (size_t i = 0; i < dir->file_count; i++) {
            // Print file names (you can replace this with your own output function)
            print(dir->files[i]->name);
        }
        for (size_t i = 0; i < dir->dir_count; i++) {
            // Print directory names
            print(dir->directories[i]->name);
        }
    }
}

// Get a directory by name
struct vfs_directory* vfs_get_directory(const char* name) {
    for (size_t i = 0; i < root_directory.dir_count; i++) {
        if (strcmp(root_directory.directories[i]->name, name) == 0) {
            return root_directory.directories[i];
        }
    }
    return NULL; // Directory not found
}