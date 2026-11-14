#include <stdio.h>
#include <stdlib.h>
#include <intcode.h>

status_t opcode_add(int *buf, int *index, size_t max_size) {
    
    if (buf[*index] != OPCODE_ADD) {
        printf("Incorrectly identified OPCODE_ADD!");
        return STATUS_BAD;
    }

    int ptr_a = buf[*index+1];
    int ptr_b = buf[*index+2];
    int target = buf[*index+3];

    if (ptr_a > max_size || ptr_b > max_size || target > max_size) {
        printf("Pointer address is larger than the size of the program! (opcode_add())\n");
        printf("\tptr_a: %i\n\tptr_b: %i\n\ttarget: %i\n\tmax_size: %li\n", ptr_a, ptr_b, target, max_size);
        return STATUS_BAD;
    };

    int operand_a = buf[ptr_a];
    int operand_b = buf[ptr_b];

    int result = operand_a + operand_b;

    // printf("OPCODE_ADD @%i: %i + %i = %i @ %i\n", *index, operand_a, operand_b, result, target);

    buf[target] = result;

    *index += 4;

    return STATUS_GOOD;
}

status_t opcode_mult(int *buf, int *index, size_t max_size) {
    if (buf[*index] != OPCODE_MULT) {
        printf("Incorrectly identified OPCODE_MULT!");
        return STATUS_BAD;
    }

    int ptr_a = buf[*index+1];
    int ptr_b = buf[*index+2];
    int target = buf[*index+3];

    if (ptr_a > max_size || ptr_b > max_size || target > max_size) {
        printf("Pointer address is larger than the size of the program! (opcode_add())\n");
        printf("\tptr_a: %i\n\tptr_b: %i\n\ttarget: %i\n\tmax_size: %li\n", ptr_a, ptr_b, target, max_size);
        return STATUS_BAD;
    };

    int operand_a = buf[ptr_a];
    int operand_b = buf[ptr_b];

    int result = operand_a * operand_b;

    // printf("OPCODE_MULT @%i: %i * %i = %i @ %i\n", *index, operand_a, operand_b, result, target);

    buf[target] = result;

    *index += 4;

    return STATUS_GOOD;
}

status_t run_code(int **data_buf, size_t data_buf_size, int noun, int verb) {
    int index = 0;
    int halt_sig = 0;

    (*data_buf)[1] = noun;
    (*data_buf)[2] = verb;

    while (!halt_sig) {
        if (index > data_buf_size-1) {
            printf("Exceeded buffer!");
            return STATUS_BAD;
        }

        if ((*data_buf)[index] == OPCODE_TERM) {
            // printf("TERMINATING @ %d!\n", index);
            return STATUS_GOOD;
        }

        if ((*data_buf)[index] == OPCODE_ADD && data_buf_size-5 > index) {
            if (opcode_add(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        } else if ((*data_buf)[index] == OPCODE_MULT && data_buf_size-5 > index) {
            if (opcode_mult(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        } else if (data_buf_size-5 < index) {
            printf("TERMINATING @ %d! Danger of overflow!\n", index);
            printf("Data Buffer Size: %li\n", data_buf_size);
            return STATUS_BAD;
        } else {
            printf("UNKNOWN OPCODE %i @ %i!\n", (*data_buf)[index], index);
            return STATUS_BAD;
        }
    }
    printf("How did we get to the end of run_code()?\n");
    return STATUS_BAD;
}

status_t reset_buffer(int **data_buf, size_t *data_buf_size) {
    free(*data_buf);
    *data_buf = NULL;
    
    *data_buf = calloc(1, sizeof(int));
    *data_buf_size = 0;

    if (*data_buf == NULL) {
        free(*data_buf);
        *data_buf = NULL;
        return STATUS_BAD;
    }

    return STATUS_GOOD;
}

status_t reload_program(int **data_buf, size_t *data_buf_size, char *filepath) {
    
    if (reset_buffer(data_buf, data_buf_size) == STATUS_BAD) {
        return STATUS_BAD;
    }

    if(ingest_comma_delim(data_buf, data_buf_size, filepath) == STATUS_BAD) {
        printf("Failed to ingest data (ingest_comma_delim())");
        free(data_buf);
        data_buf = NULL;
        return STATUS_BAD;
    }

    return STATUS_GOOD;
}

int run_and_reset(int **data_buf, size_t *data_buf_size, int noun, int verb, char *filepath) {
    
    if(run_code(data_buf, *data_buf_size, noun, verb) == STATUS_BAD) {
            free(*data_buf);
            *data_buf = NULL;
            return -1;
        }

        int output = *data_buf[0];

        if (reload_program(data_buf, data_buf_size, filepath) == STATUS_BAD) {
            free(*data_buf);
            *data_buf = NULL;
            return -1;
        }

        return output;
}

status_t run_for_target(int **data_buf, size_t *data_buf_size, intcode_input *input, char *filepath) {
    int noun_left = input->min_noun;
    int noun_right = input->max_noun;
    
    int verb_left = input->min_verb;
    int verb_right = input->max_verb;

    // Find target noun
    while (noun_left <= noun_right) {
        int noun_mid = noun_left + (int)((noun_right - noun_left) / 2);

        int lower = run_and_reset(data_buf, data_buf_size, noun_mid, input->min_verb, filepath);

        if (lower < 0) {
            return STATUS_BAD;
        }

        if (lower > input->target) {
            noun_right = noun_mid - 1;
            continue;
        }

        int upper = run_and_reset(data_buf, data_buf_size, noun_mid, input->max_verb, filepath);

        if (upper < 0) {
            return STATUS_BAD;
        }

        if (upper < input->target) {
            noun_left = noun_mid + 1;
            continue;
        }

        if (lower < input->target && upper > input->target) {
            input->noun = noun_mid;
            break;
        }

    }

    // Find target verb
    while (verb_left <= verb_right) {
        int verb_mid = verb_left + (int)((verb_right - verb_left) / 2);

        int output = run_and_reset(data_buf, data_buf_size, input->noun, verb_mid, filepath);

        if (output < 0) {
            return STATUS_BAD;
        }

        if (output > input->target) {
            verb_right = verb_mid - 1;
            continue;
        }

        if (output < input->target) {
            verb_left = verb_mid + 1;
            continue;
        }

        if (output == input->target) {
            input->verb = verb_mid;
            return STATUS_GOOD;
        }

    }

    printf("End of run-for-code, something went wrong.\n");
    return STATUS_BAD;
}