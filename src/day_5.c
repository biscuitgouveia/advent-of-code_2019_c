#include <stdio.h>
#include <intcode.h>
#include <puzzles.h>
#include <logger.h>

#define INPUT_PATH "./Day 5/input"


int day_5() {

    int *data_buf = calloc(1, sizeof(int));
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

    if (ic_program_run(&data_buf, data_buf_size, data_buf[1], data_buf[2]) == STATUS_BAD) {
        LOG_ERROR("Something went wrong!");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    // int part_one = data_buf[0];

    // if (ic_program_reload(&data_buf, &data_buf_size, INPUT_PATH) == STATUS_BAD) {
    //     printf("Error reloading program.\n");
    //     free(data_buf);
    //     data_buf = NULL;
    //     return -1;
    // }


    free(data_buf);
    data_buf = NULL;

    return 0;

}