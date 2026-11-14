#ifndef INTCODE_H

#include <ethan_utilities.h>

#define OPCODE_ADD 1
#define OPCODE_MULT 2
#define OPCODE_TERM 99

typedef struct {
    int noun;
    int verb;
    int min_noun;
    int max_noun;
    int min_verb;
    int max_verb;
    int target;
} intcode_input;

status_t opcode_add(int *buf, int *index, size_t max_size);
status_t opcode_mult(int *buf, int *index, size_t max_size);
status_t run_code(int **data_buf, size_t data_buf_size, int noun, int verb);
status_t reset_buffer(int **data_buf, size_t *data_buf_size);
status_t reload_program(int **data_buf, size_t *data_buf_size, char *filepath);
status_t run_for_target(int **data_buf, size_t *data_buf_size, intcode_input *input, char *filepath);

#endif