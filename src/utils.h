#ifndef SAPPHIRE_UTILS_H
#define SAPPHIRE_UTILS_H

#include <stdio.h>
#include <stdlib.h>

char *my_strndup(const char *str, int chars);

typedef void* Pointer;

typedef struct{
    Pointer* data;
    size_t size;
    size_t capacity;
} PointerArray;

PointerArray* PointerArray_create();

void PointerArray_init(PointerArray *array);

void PointerArray_add(PointerArray *array, Pointer element);

void PointerArray_free(PointerArray *array);

#endif //SAPPHIRE_UTILS_H
