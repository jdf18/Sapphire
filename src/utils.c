#include "utils.h"
#include "dynamic_arrays.h"

#include <stdlib.h>

char *my_strndup(const char *str, int chars) {
    char *buffer;
    int n;

    buffer = (char *) malloc(chars +1);
    if (buffer)
    {
        for (n = 0; ((n < chars) && (str[n] != 0)) ; n++) buffer[n] = str[n];
        buffer[n] = 0;
    }

    return buffer;
}

// Dynamic pointer array
PointerArray* PointerArray_create() {
    PointerArray* pa = malloc(sizeof(PointerArray));
    if (pa == NULL) {
        fprintf(stderr, "Failed to allocate memory for PointerArray\n");
        exit(EXIT_FAILURE);
    }

    PointerArray_init(pa);
    return pa;
}

void PointerArray_init(PointerArray *array) {
    array -> size = 0;
    array -> capacity = DYN_ARRAY_INITIAL_CAPACITY;
    array -> data = (Pointer *)malloc(array->capacity * sizeof(Pointer));
    if (array -> data == NULL) {
        fprintf(stderr, "Failed to allocate memory for PointerArray array\n");
        exit(EXIT_FAILURE);
    }
}

void PointerArray_add(PointerArray *array, Pointer element) {
    if (array -> size >= array -> capacity) {
        array -> capacity *= 2;
        array -> data = (Pointer *)realloc(array->data, array->capacity * sizeof(Pointer));
        if (array -> data == NULL) {
            fprintf(stderr, "Failed to reallocate memory for PointerArray array\n");
            exit(EXIT_FAILURE);
        }
    }
    array->data[array->size++] = element;
}

void PointerArray_free(PointerArray *array) {
    free(array->data);
}