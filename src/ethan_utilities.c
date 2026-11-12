#include <ethan_utilities.h>
#include <stdlib.h>
#include <stdio.h>

status_t resize_dynamic_array(int **arr, size_t new_size) {
    // printf("Arr: %p\n", arr);
    // printf("*Arr: %p\n", *arr);
    // printf("New_Size: %li\n", new_size);
    int *temp = reallocarray(*arr, new_size, sizeof(int));
    // printf("Size Arr: %li\n", sizeof(*arr)/sizeof(int));
    if (temp == NULL) {
        // Handle realloc failure
        printf("Memory allocation failed (resize_dynamic_array())\n");
        return STATUS_BAD;
    }
    // printf("Temp: %p\n", temp);
    *arr = temp;
    
    // printf("Arr: %p\n", *arr);
    return STATUS_GOOD;
}
