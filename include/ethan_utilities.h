#ifndef ETHAN_UTILITIES_H

#include <stdlib.h>

typedef enum {
	STATUS_GOOD,
	STATUS_BAD,
} status_t;

status_t resize_dynamic_array(void **arr, size_t new_size, size_t element_size);
status_t util_split_lines(char ***buf, size_t *buf_size, char *filepath);
status_t ingest_comma_delim(int **buf, size_t *buf_size, char *filepath);
status_t ingest_comma_delim_char(char ***buf, size_t *buf_size, char *filepath);

#endif