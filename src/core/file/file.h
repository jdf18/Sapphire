#ifndef SAPPHIRE_FILE_H
#define SAPPHIRE_FILE_H

#include <stdio.h>

FILE* open_file(const char* filename);
void close_file(FILE* file);

long get_file_length(FILE* file);

void* read_file_into_memory(FILE* file, void* destination, size_t size, long offset);
void memory_write_to_file(FILE* file, const void* source, size_t size, long offset);

#endif //SAPPHIRE_FILE_H