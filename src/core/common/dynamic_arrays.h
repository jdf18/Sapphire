#ifndef SAPPHIRE_DYNAMIC_ARRAYS_H
#define SAPPHIRE_DYNAMIC_ARRAYS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DYN_ARRAY_INITIAL_CAPACITY 10

// Macro to define the array structure and its functions
#define DEFINE_DYNAMIC_ARRAY(type, name) \
typedef struct {                         \
    type* data;                              \
    size_t size;                             \
    size_t capacity;                          \
} name##_t;                              \
                                         \
void name##_init(name##_t *array) {      \
    array -> size = 0;                     \
    array -> capacity = DYN_ARRAY_INITIAL_CAPACITY; \
    array -> data = (type *)malloc(array->capacity * sizeof(type)); \
    if (array -> data == NULL) {        \
        fprintf(stderr, "Failed to allocate memory for " #name " array\n"); \
        exit(EXIT_FAILURE); \
    }\
}                                        \
                                         \
void name##_add(name##_t *array, type element) {    \
    if (array -> size >= array -> capacity) {\
        array -> capacity *= 2;          \
        array -> data = (type *)realloc(array->data, array->capacity * sizeof(type)); \
        if (array -> data == NULL) {        \
            fprintf(stderr, "Failed to reallocate memory for " #name " array\n"); \
            exit(EXIT_FAILURE); \
        }\
    }                                    \
    array->data[array->size++] = element;\
}                                        \
                                         \
void name##_free(name##_t *array) {      \
    free(array->data);                   \
}




#endif //SAPPHIRE_DYNAMIC_ARRAYS_H
