#include <stdio.h>
#include <intcode.h>

int day_2() {

    int *data_buf = calloc(1, sizeof(int));
    size_t data_buf_size = 0;

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    if(ingest_comma_delim(&data_buf, &data_buf_size, "./Day 2/input") == STATUS_BAD) {
        printf("Failed to ingest data (ingest_comma_delim())");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    if (run_code(&data_buf, data_buf_size, 12, 2) == STATUS_BAD) {
        printf("Something went wrong!\n");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    int part_one = data_buf[0];

    printf("Part One Answer: %i\n", part_one);

    int part_two_target = 19690720;

    if (reload_program(&data_buf, &data_buf_size, "./Day 2/input") == STATUS_BAD) {
        printf("Error reloading program.\n");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            printf("Noun: %i\nVerb: %i\n", noun, verb);
            if (run_code(&data_buf, data_buf_size, noun, verb) == STATUS_BAD) {
                printf("Something went wrong!\n");
                free(data_buf);
                data_buf = NULL;
                return -1;
            }
            if (data_buf[0] == part_two_target) {
                printf("DONE!\n");
                printf("Noun: %i\nVerb: %i\n", noun, verb);
                free(data_buf);
                data_buf = NULL;
                return 0;
            } else {
                if (reload_program(&data_buf, &data_buf_size, "./Day 2/input") == STATUS_BAD) {
                    printf("Error reloading program.\n");
                    free(data_buf);
                    data_buf = NULL;
                    return -1;
                }
            }
        }
    }

    free(data_buf);
    data_buf = NULL;

    return 0;

}