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

    intcode_input part_two_params = {
        0,
        0,
        MIN_NOUN,
        MAX_NOUN,
        MIN_VERB,
        MAX_VERB,
        PART_TWO_TARGET,
    };

    if (run_for_target(&data_buf, &data_buf_size, &part_two_params, INPUT_PATH) == STATUS_BAD) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    printf("Part One Answer: %i\n", part_one);
    printf("Noun: %i\nVerb: %i\n", part_two_params.noun, part_two_params.verb);
    printf("Part Two Answer: %i\n", 100 * part_two_params.noun + part_two_params.verb);

    free(data_buf);
    data_buf = NULL;

    return 0;

}