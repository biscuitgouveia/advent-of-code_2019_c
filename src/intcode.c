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
            printf("TERMINATING @ %d!\n", index);
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