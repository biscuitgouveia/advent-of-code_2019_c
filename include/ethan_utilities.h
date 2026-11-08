#ifndef ETHAN_UTILITIES_H

#include <stdlib.h>

typedef enum {
	STATUS_GOOD,
	STATUS_BAD,
} status_t;

status_t resize_dynamic_array(int **arr, size_t new_size);

#endif