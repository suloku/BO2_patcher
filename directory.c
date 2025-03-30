#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "directory.h"

char get_path_separator() {
    #ifdef WIN32
        return '\\';
    #elif WIN64
        return '\\';
    #elif WINNT
        return '\\';
    #else
        return '/';
    #endif
}

void append_directory(char *base_path, const char *directory, char *result, size_t result_size) {
    char separator[2] = { get_path_separator(), '\0' };
    strncpy(result, base_path, result_size - 1);
    result[result_size - 1] = '\0';  // Ensure null termination
    strncat(result, separator, result_size - strlen(result) - 1);
    strncat(result, directory, result_size - strlen(result) - 1);
}

int file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to extract the filename from a given path
const char *get_filename(const char *path) {
    if (!path) return NULL;

    char separator = get_path_separator();
    const char *last_sep = strrchr(path, separator);

    // If no separator is found, return the original path (it is already a filename)
    return (last_sep) ? last_sep + 1 : path;
}
