#include <stdio.h>
#include <stdlib.h>
#include <intcode.h>
#include <logger.h>

status_t ic_params_decode(opcode_mode *opcode_mode, int opcode) {
    if (opcode <= 0) {
        printf("Invalid opcode: %i\n", opcode);
        return STATUS_BAD;
    }
    opcode_mode->opcode = opcode % 20;
    opcode /= 10;
    opcode /= 10;

    if (opcode > 0) {
        opcode_mode->first_param = opcode % 10;
        opcode /= 10;
    }

    if (opcode > 0) {
        opcode_mode->second_param = opcode % 10;
        opcode /= 10;
    }

    if (opcode > 0) {
        opcode_mode->third_param = opcode % 10;
        opcode /= 10;
    }

    LOG_DEBUG("Opcode parameters: Opcode: %d Param 1: %d Param 2: %d Param 3: %d",
        opcode_mode->opcode,
        opcode_mode->first_param,
        opcode_mode->second_param,
        opcode_mode->third_param);

    return STATUS_GOOD;
}

status_t ic_opcode_add(int *buf, int *int_pointer, size_t max_size) {
    
    // Decode opcode and parameter modes into opcode_mode
    opcode_mode opcode_mode = {0};

    if (ic_params_decode(&opcode_mode, buf[*int_pointer]) == STATUS_BAD) {
        return STATUS_BAD;
    }

    LOG_DEBUG("opcode_mode: Opcode: %d Param 1: %d Param 2: %d Param 3: %d",
        opcode_mode.opcode,
        opcode_mode.first_param,
        opcode_mode.second_param,
        opcode_mode.third_param);

    // Test that the opcode is actually an add opcode
    if (opcode_mode.opcode % 10 != OPCODE_ADD) {
        LOG_ERROR("Incorrectly identified OPCODE_ADD!");
        return STATUS_BAD;
    }

    int operand_a = 0;
    int operand_b = 0;

    if (opcode_mode.first_param == POS_MODE) {
        int ptr_a = buf[*int_pointer+1];
        if (ptr_a > max_size) {
            LOG_ERROR("Pointer address is larger than the size of the program!");
            LOG_ERROR("\tptr_a: %i\n\tmax_size: %li\n", ptr_a, max_size);
            return STATUS_BAD;
        }
        operand_a = buf[ptr_a];
    } else {
        operand_a = buf[*int_pointer+1]; // Immediate mode
    }

    if (opcode_mode.second_param == POS_MODE) {
        int ptr_b = buf[*int_pointer+2];
        if (ptr_b > max_size) {
            LOG_ERROR("Pointer address is larger than the size of the program!");
            LOG_ERROR("\tptr_b: %i\n\tmax_size: %li\n", ptr_b, max_size);
            return STATUS_BAD;
        }
        operand_b = buf[ptr_b];
    } else {
        operand_b = buf[*int_pointer+2]; // Immediate mode
    }

    int target_ptr = buf[*int_pointer+3];

    if (target_ptr > max_size) {
        LOG_ERROR("Pointer address is larger than the size of the program!");
        LOG_ERROR("\ttarget_ptr: %i\n\tmax_size: %li\n", target_ptr, max_size);
        return STATUS_BAD;
    }

    int result = operand_a + operand_b;

    LOG_DEBUG("IC_OPCODE_ADD @%i: %i + %i = %i @ %i", *int_pointer, operand_a, operand_b, result, target_ptr);

    buf[target_ptr] = result;

    *int_pointer += 4;

    return STATUS_GOOD;
}

status_t ic_opcode_mult(int *buf, int *int_pointer, size_t max_size) {
    
    // Decode opcode and parameter modes into opcode_mode
    opcode_mode opcode_mode = {0};

    if (ic_params_decode(&opcode_mode, buf[*int_pointer]) == STATUS_BAD) {
        return STATUS_BAD;
    }

    LOG_DEBUG("opcode_mode: Opcode: %d Param 1: %d Param 2: %d Param 3: %d",
        opcode_mode.opcode,
        opcode_mode.first_param,
        opcode_mode.second_param,
        opcode_mode.third_param);

    // Test that the opcode is actually a mult opcode
    if (opcode_mode.opcode % 10 != OPCODE_MULT) {
        LOG_ERROR("Incorrectly identified OPCODE_MULT!");
        return STATUS_BAD;
    }

    int operand_a = 0;
    int operand_b = 0;

    if (opcode_mode.first_param == POS_MODE) {
        int ptr_a = buf[*int_pointer+1];
        if (ptr_a > max_size) {
            LOG_ERROR("Pointer address is larger than the size of the program!");
            LOG_ERROR("\tptr_a: %i\n\tmax_size: %li\n", ptr_a, max_size);
            return STATUS_BAD;
        }
        operand_a = buf[ptr_a];
    } else {
        operand_a = buf[*int_pointer+1]; // Immediate mode
    }

    if (opcode_mode.second_param == POS_MODE) {
        int ptr_b = buf[*int_pointer+2];
        if (ptr_b > max_size) {
            LOG_ERROR("Pointer address is larger than the size of the program!");
            LOG_ERROR("\tptr_b: %i\n\tmax_size: %li\n", ptr_b, max_size);
            return STATUS_BAD;
        }
        operand_b = buf[ptr_b];
    } else {
        operand_b = buf[*int_pointer+2]; // Immediate mode
    }

    int target_ptr = buf[*int_pointer+3];

    if (target_ptr > max_size) {
        LOG_ERROR("Pointer address is larger than the size of the program!");
        LOG_ERROR("\ttarget_ptr: %i\n\tmax_size: %li\n", target_ptr, max_size);
        return STATUS_BAD;
    }

    int result = operand_a * operand_b;

    LOG_DEBUG("IC_OPCODE_MULT @%i: %i * %i = %i @ %i", *int_pointer, operand_a, operand_b, result, target_ptr);

    buf[target_ptr] = result;

    *int_pointer += 4;

    return STATUS_GOOD;
}

status_t ic_opcode_inp(int *buf, int *int_pointer, size_t max_size) {
    // Decode opcode and parameter modes into opcode_mode
    opcode_mode opcode_mode = {0};

    if (ic_params_decode(&opcode_mode, buf[*int_pointer]) == STATUS_BAD) {
        return STATUS_BAD;
    }

    LOG_DEBUG("opcode_mode: Opcode: %d Param 1: %d Param 2: %d Param 3: %d",
        opcode_mode.opcode,
        opcode_mode.first_param,
        opcode_mode.second_param,
        opcode_mode.third_param);

    // Test that the opcode is actually an inp opcode
    if (opcode_mode.opcode % 10 != OPCODE_INP) {
        LOG_ERROR("Incorrectly identified OPCODE_INP!");
        return STATUS_BAD;
    }

    int target_ptr = buf[*int_pointer+1];
    
    if (target_ptr > max_size) {
        LOG_ERROR("Pointer address is larger than the size of the program!");
        LOG_ERROR("\ttarget_ptr: %i\n\tmax_size: %li\n", target_ptr, max_size);
        return STATUS_BAD;
    }

    printf("Input an integer: ");
    scanf("%d", &buf[target_ptr]);
    *int_pointer += 2;
    return STATUS_GOOD;
}

status_t ic_opcode_outp(int *buf, int *int_pointer, size_t max_size) {
    // Decode opcode and parameter modes into opcode_mode
    opcode_mode opcode_mode = {0};

    if (ic_params_decode(&opcode_mode, buf[*int_pointer]) == STATUS_BAD) {
        return STATUS_BAD;
    }

    LOG_DEBUG("opcode_mode: Opcode: %d Param 1: %d Param 2: %d Param 3: %d",
        opcode_mode.opcode,
        opcode_mode.first_param,
        opcode_mode.second_param,
        opcode_mode.third_param);

    // Test that the opcode is actually an outp opcode
    if (opcode_mode.opcode % 10 != OPCODE_OUTP) {
        LOG_ERROR("Incorrectly identified OPCODE_OUTP!");
        return STATUS_BAD;
    }

    int target_ptr = buf[*int_pointer+1];

    if (target_ptr > max_size) {
        LOG_ERROR("Pointer address is larger than the size of the program!");
        LOG_ERROR("\ttarget_ptr: %i\n\tmax_size: %li\n", target_ptr, max_size);
        return STATUS_BAD;
    }

    LOG_INFO("Output: %i", buf[target_ptr]);
    
    *int_pointer += 2;
    return STATUS_GOOD;
}

// Runs a full intcode program.
//
// params:
//  - data_buf: Double pointer to a data buffer initialised with an intcode program.
//  - data_buf_size: The size of the data buffer.
//  - noun: Noun supplied for the first register of the program.
//  - verb: Verb supplied for the second register of the program.
//
// returns: STATUS_GOOD on success, or STATUS_BAD on error.
status_t ic_program_run(int **data_buf, size_t data_buf_size, int noun, int verb) {
    int index = 0;
    int halt_sig = 0;

    (*data_buf)[1] = noun;
    (*data_buf)[2] = verb;

    while (!halt_sig) {
        if (index > data_buf_size-1) {
            LOG_ERROR("Exceeded buffer!");
            return STATUS_BAD;
        }

        if ((*data_buf)[index] == OPCODE_TERM || (*data_buf)[index] % 20 == OPCODE_TERM) {
            // printf("TERMINATING @ %d!\n", index);
            return STATUS_GOOD;
        }

        if (((*data_buf)[index] == OPCODE_INP || (*data_buf)[index] % 10 == OPCODE_INP) && data_buf_size-2 > index) {
            if (ic_opcode_inp(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        }

        if (((*data_buf)[index] == OPCODE_OUTP || (*data_buf)[index] % 10 == OPCODE_OUTP)  && data_buf_size-2 > index) {
            if (ic_opcode_outp(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        }

        if (((*data_buf)[index] == OPCODE_ADD || (*data_buf)[index] % 10 == OPCODE_ADD) && data_buf_size-5 > index) {
            if (ic_opcode_add(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        } else if (((*data_buf)[index] == OPCODE_MULT || (*data_buf)[index] % 10 == OPCODE_MULT) && data_buf_size-5 > index) {
            if (ic_opcode_mult(*data_buf, &index, data_buf_size-1) == STATUS_BAD) {
                return STATUS_BAD;
            }
        } else if (data_buf_size-5 < index) {
            LOG_ERROR("TERMINATING @ %d! Danger of overflow!", index);
            LOG_ERROR("Data Buffer Size: %li", data_buf_size);
            return STATUS_BAD;
        } else {
            LOG_ERROR("UNKNOWN OPCODE %i @ %i!", (*data_buf)[index], index);
            return STATUS_BAD;
        }
    }
    LOG_ERROR("How did we get to the end of ic_program_run()?\n");
    return STATUS_BAD;
}

// Empties the data buffer.
//
// params:
//  - data_buf: Double pointer to a data buffer initialised with a previous intcode program.
//  - data_buf_size: A pointer to a variable counting the size of the buffer.
//
// returns: STATUS_GOOD on success, or STATUS_BAD on error.
status_t ic_buffer_reset(int **data_buf, size_t *data_buf_size) {
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

// Empties the data buffer and reloads an intcode program from a file.
//
// params:
//  - data_buf: Double pointer to a data buffer initialised with a previous intcode program.
//  - data_buf_size: A pointer to a variable counting the size of the buffer.
//  - filepath: Path to the file containing the intcode program.
//
// returns: STATUS_GOOD on success, or STATUS_BAD on error.
status_t ic_program_reload(int **data_buf, size_t *data_buf_size, char *filepath) {
    
    if (ic_buffer_reset(data_buf, data_buf_size) == STATUS_BAD) {
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
    
    if(ic_program_run(data_buf, *data_buf_size, noun, verb) == STATUS_BAD) {
            free(*data_buf);
            *data_buf = NULL;
            return -1;
        }

        int output = *data_buf[0];

        if (ic_program_reload(data_buf, data_buf_size, filepath) == STATUS_BAD) {
            free(*data_buf);
            *data_buf = NULL;
            return -1;
        }

        return output;
}

// Runs an intcode program repeatedly until a target value is found, using a binary search to find the correct noun and verb.
//
// params:
//  - data_buf: Double pointer to a data buffer initialised with an intcode program.
//  - data_buf_size: A pointer to a variable counting the size of the buffer.
//  - intcode_input: Pointer to a struct containing the target value, min and max noun and verb values, and the noun and verb values themselves.
//  - filepath: Path to the file containing the intcode program.
//
// returns: STATUS_GOOD on success, or STATUS_BAD on error.
status_t ic_target_run(int **data_buf, size_t *data_buf_size, intcode_input *input, char *filepath) {
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