#include "file.h"

#include <stdio.h>
#include <stdlib.h>

FILE* open_file(const char* filename) {
    FILE* file = fopen(filename, "r+b");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(EXIT_FAILURE);
    }
    return file;
}

void close_file(FILE* file) {
    if (fclose(file) != 0) {
        printf("Error closing the file.\n");
        exit(EXIT_FAILURE);
    }
}


long get_file_length(FILE* file) {
    // Move the file pointer to the end of the file
    if (fseek(file, 0, SEEK_END) != 0) {
        printf("Failed to seek the file.\n");
        exit(EXIT_FAILURE);
    }

    // Get the position of the pointer
    long length = ftell(file);
    if (length == -1) {
        printf("Failed to determine the file size.\n");
        exit(EXIT_FAILURE);
    }

    // Return the pointer back to the start of the file
    if (fseek(file, 0, SEEK_SET) != 0) {
        printf("Failed to seek the file.\n");
        exit(EXIT_FAILURE);
    }

    return length;
}


void* read_file_into_memory(FILE* file, void* destination, size_t size, long offset) {
    // Moves the cursor offset bytes from the start of the file
    if (fseek(file, offset, SEEK_SET) != 0) {
        printf("Failed to seek the file.\n");
        exit(EXIT_FAILURE);
    }

    size_t bytes_read = fread(destination, 1, size, file);

    if (bytes_read != size) {
        printf("Failed to read the whole file.\n");
        exit(EXIT_FAILURE);
    }

    return destination;
}

void memory_write_to_file(FILE* file, const void* source, size_t size, long offset) {
    // Moves the cursor offset bytes from the start of the file
    if (fseek(file, offset, SEEK_SET) != 0) {
        printf("Failed to seek the file.\n");
        exit(EXIT_FAILURE);
    }

    size_t bytes_written = fwrite(source, 1, size, file);

    if (bytes_written != size) {
        printf("Failed to write data to the file.\n");
        exit(EXIT_FAILURE);
    }
}