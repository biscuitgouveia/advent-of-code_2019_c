#include <stdio.h>
#include <ethan_utilities.h>

#define INPUT_PATH "./Day 3/input"

typedef struct {
    int x;
    int y;
} point_t;

int day_3() {

    char *data_buf = calloc(1, sizeof(char));
    size_t data_buf_size = 0;

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    if(ingest_comma_delim(&data_buf, &data_buf_size, INPUT_PATH) == STATUS_BAD) {
        printf("Failed to ingest data (ingest_comma_delim())");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    for (int i = 0; i < data_buf_size; i++) {
        printf("%s", data_buf[i]);
    }

    free(data_buf);
    data_buf = NULL;

    return 0;
}