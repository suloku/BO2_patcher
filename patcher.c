#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "patcher.h"

//#define DEBUG_DIRECTORY

// Function to search for a binary pattern in a large file
// Will error depending on the size of ret_offset[] array
int search_binary_pattern(const char *filename, const unsigned char *pattern, size_t pattern_size, long *ret_offset) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE + pattern_size - 1);
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return -1;
    }

    long offset = 0;  // Track the current offset in the file
    size_t bytes_read;
    int occurrences = 0;

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        // Search for the pattern in the current chunk
        for (size_t i = 0; i <= bytes_read - pattern_size; i++) {
            if (memcmp(buffer + i, pattern, pattern_size) == 0) {

#ifdef DEBUG_DIRECTORY
                printf("Match at offset 0x%lX: First byte = 0x%X\n", offset + i, buffer[i]);
                printf("Pattern found at offset: 0x%lX (%ld bytes)\n", offset + i - pattern_size+1, offset + i - pattern_size+1);
#endif
                ret_offset[occurrences] = offset + i - pattern_size+1;
                occurrences++;
            }
        }

        // Move the file pointer back to avoid missing matches at chunk boundaries
        offset += bytes_read;
        fseek(file, offset - (pattern_size - 1), SEEK_SET);
    }
#ifdef DEBUG_DIRECTORY
    printf("Total occurrences found: %d\n", occurrences);
#endif

    free(buffer);
    fclose(file);

    return occurrences;
}

// Function to replace bytes in a binary file at a specific offset
int replace_data_in_file(const char *filename, long offset, const unsigned char *new_data, size_t data_size) {
    FILE *file = fopen(filename, "rb+"); // Open file for reading and writing in binary mode
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Move to the specified offset
    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(file);
        return -1;
    }

    // Write the new data at the specified location
    if (fwrite(new_data, 1, data_size, file) != data_size) {
        perror("Error writing to file");
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0; // Success
}

// Function to read 4 bytes from a binary file at a specific offset
int read_4_bytes_from_file(const char *filename, long offset, unsigned char *buffer) {
    FILE *file = fopen(filename, "rb"); // Open file for reading in binary mode
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    // Move to the specified offset
    if (fseek(file, offset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(file);
        return -1;
    }

    // Read 4 bytes into the buffer
    if (fread(buffer, 1, 4, file) != 4) {
        if (feof(file)) {
            fprintf(stderr, "End of file reached before reading 4 bytes\n");
        } else {
            perror("Error reading from file");
        }
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0; // Success
}
