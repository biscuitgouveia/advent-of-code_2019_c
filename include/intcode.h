#ifndef INTCODE_H

#include <ethan_utilities.h>

#define OPCODE_ADD 1
#define OPCODE_MULT 2
#define OPCODE_INP 3
#define OPCODE_OUTP 4
#define OPCODE_JUMP_TRUE 5
#define OPCODE_JUMP_FALSE 6
#define OPCODE_LESS_THAN 7
#define OPCODE_EQUAL 8
#define OPCODE_TERM 99

#define POS_MODE 0
#define IMM_MODE 1

typedef struct {
    int noun;
    int verb;
    int min_noun;
    int max_noun;
    int min_verb;
    int max_verb;
    int target;
} intcode_input;

typedef struct {
    int opcode;
    int first_param;
    int second_param;
    int third_param;
}  opcode_mode;

status_t ic_params_decode(opcode_mode *opcode_param_mode, int opcode);
status_t ic_opcode_add(int *buf, int *index, size_t max_size);
status_t ic_opcode_mult(int *buf, int *index, size_t max_size);
status_t ic_program_run(int **data_buf, size_t data_buf_size, int noun, int verb);
status_t ic_buffer_reset(int **data_buf, size_t *data_buf_size);
status_t ic_program_reload(int **data_buf, size_t *data_buf_size, char *filepath);
status_t ic_target_run(int **data_buf, size_t *data_buf_size, intcode_input *input, char *filepath);

#endif