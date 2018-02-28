#ifndef DYNAMIC_ARRAY_HEADER
#define DYNAMIC_ARRAY_HEADER

//TODO: Move some of the exposed variables into an internal object

#include <stddef.h>

typedef struct {
    void* ptr;      // Internal string buffer
    size_t el_size; // How big is each element
    size_t index;   // Where to insert the next character
    size_t len;     // Absolute string length (with null-terminator)
    size_t max;     // Biggest index used (DO NOT MESS WITH THIS!!!)
} Dynamic_Arr_t, *pDynamic_Arr_t;


pDynamic_Arr_t new_dynamic_array(size_t el_size);
void free_dynamic_array(pDynamic_Arr_t);
void add_array_element(pDynamic_Arr_t arr, const void* new);

void set_array_index(pDynamic_Arr_t arr, size_t index);
void* get_array_element(pDynamic_Arr_t arr, size_t index);

//Resize the pointer
void* flush_dynamic_array(pDynamic_Arr_t arr);

size_t get_array_count(pDynamic_Arr_t arr);

#endif // DYNAMIC_ARRAY_HEADER Included
