#include <stdio.h>
#include <intcode.h>

#define INPUT_PATH "./Day 2/input"

#define MIN_NOUN 0
#define MAX_NOUN 99
#define MIN_VERB 0
#define MAX_VERB 99

#define PART_TWO_TARGET 19690720

int day_2() {

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

    if (run_code(&data_buf, data_buf_size, 12, 2) == STATUS_BAD) {
        printf("Something went wrong!\n");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    int part_one = data_buf[0];

    if (reload_program(&data_buf, &data_buf_size, INPUT_PATH) == STATUS_BAD) {
        printf("Error reloading program.\n");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    // Todo: Rewrite as binary search
    for (int noun = MIN_NOUN; noun <= MAX_NOUN; noun++) {
        for (int verb = MIN_VERB; verb <= MAX_VERB; verb++) {
            if (run_code(&data_buf, data_buf_size, noun, verb) == STATUS_BAD) {
                printf("Something went wrong!\n");
                free(data_buf);
                data_buf = NULL;
                return -1;
            }
            if (data_buf[0] == PART_TWO_TARGET) {
                printf("DONE!\n");
                printf("Part One Answer: %i\n", part_one);
                printf("Noun: %i\nVerb: %i\n", noun, verb);
                printf("Part Two Answer: %i\n", 100 * noun + verb);
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