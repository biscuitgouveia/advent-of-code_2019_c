#include <ethan_utilities.h>
#include <stdlib.h>
#include <stdio.h>

status_t resize_dynamic_array(int **arr, size_t new_size) {
    int *temp = realloc(*arr, new_size * sizeof(int));
    if (temp == NULL) {
        // Handle realloc failure
        printf("Memory allocation failed\n");
        return STATUS_BAD;
    }
    *arr = temp;
    return STATUS_GOOD;
}
