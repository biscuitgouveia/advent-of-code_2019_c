#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ethan_utilities.h>
#include <puzzles.h>

#define OPCODE_ADD 1
#define OPCODE_MULT 2
#define OPCODE_TERM 99

status_t opcode_add(int *buf, int *index) {
    if (buf[*index] != OPCODE_ADD) {
        printf("Incorrectly identified OPCODE_ADD!");
        return STATUS_BAD;
    }

    int operand_a = buf[buf[*index+1]];
    int operand_b = buf[buf[*index+2]];
    int target = buf[*index+3];

    int result = operand_a + operand_b;

    printf("OPCODE_ADD @%i: %i + %i = %i @ %i\n", *index, operand_a, operand_b, result, target);

    buf[target] = result;

    *index += 4;

    return STATUS_GOOD;
}

status_t opcode_mult(int *buf, int *index) {
    if (buf[*index] != OPCODE_MULT) {
        printf("Incorrectly identified OPCODE_MULT!");
        return STATUS_BAD;
    }

    int operand_a = buf[buf[*index+1]];
    int operand_b = buf[buf[*index+2]];
    int target = buf[*index+3];

    int result = operand_a * operand_b;

    printf("OPCODE_MULT @%i: %i * %i = %i @ %i\n", *index, operand_a, operand_b, result, target);

    buf[target] = result;

    *index += 4;

    return STATUS_GOOD;
}

int day_2() {

    size_t new_size = 0;

    int *data_buf = calloc(1, sizeof(int));

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    // open file stream and check for errors
    FILE *fp = fopen("./Day 2/input", "r");

    if (fp == NULL) {
        perror("fopen");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    char *lineptr = NULL;
    size_t line_buf_size = 0;

    while(getdelim(&lineptr, &line_buf_size, ',', fp) != -1) {
        int temp = atoi(lineptr);
        new_size++;

        if (new_size > 1) {
            // clean everything up if resizing array fails
            if (resize_dynamic_array(&data_buf, new_size) == STATUS_BAD) {
                free(data_buf);
                data_buf = NULL;
                free(lineptr);
                lineptr = NULL;
                fclose(fp);
                return -1;
            };
        }

        data_buf[new_size-1] = temp;
    }

    fclose(fp);
    free(lineptr);
    lineptr = NULL;

    // printf("#########################################\n");
    // printf("########## BEFORE RUNNING CODE ##########\n");
    // printf("#########################################\n");

    // for (int i = 0; i < new_size-1; i += 4) {
    //     printf("%i, %i, %i, %i\n", data_buf[i], data_buf[i+1], data_buf[i+2], data_buf[i+3]);
    // }

    int index = 0;
    int halt_sig = 0;

    data_buf[1] = 12;
    data_buf[2] = 2;

    while (!halt_sig) {
        if (index > new_size-1) {
            printf("Exceeded buffer!");
            return -1;
        }
        if (data_buf[index] == OPCODE_ADD) {
            opcode_add(data_buf, &index);
        } else if (data_buf[index] == OPCODE_MULT) {
            opcode_mult(data_buf, &index);
        } else if (data_buf[index] == OPCODE_TERM) {
            printf("TERMINATING @ %d!\n", index);
            halt_sig = 1;
            break;
        } else {
            printf("UNKNOWN OPCODE %i @ %i!\n", data_buf[index], index);
            halt_sig = 1;
            break;
        }
    }
    
    // printf("#########################################\n");
    // printf("########### AFTER RUNNING CODE ##########\n");
    // printf("#########################################\n");

    // for (int i = 0; i < new_size-1; i += 4) {
    //     printf("%i, %i, %i, %i\n", data_buf[i], data_buf[i+1], data_buf[i+2], data_buf[i+3]);
    // }
    
    printf("Part One Answer: %i\n", data_buf[0]);

    free(data_buf);
    data_buf = NULL;

    return 0;
}