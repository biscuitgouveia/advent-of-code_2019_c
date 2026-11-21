#include <stdio.h>
#include <ethan_utilities.h>
#include <logger.h>
#include <puzzles.h>

#define INPUT_PATH "./Day 3/input"
#define UP "U"
#define DOWN "D"
#define LEFT "L"
#define RIGHT "R"

typedef struct {
    int x;
    int y;
} point_t;

 typedef struct {
    char *direction;
    int distance;
} movement_t;

typedef struct {
    char **movements;
    point_t *points;
    size_t num_movements;
} wire_t;

// status_t wire_init(wire_t *wire, char **movements, size_t num_movements) {

// }

int day_3() {

    char **data_buf = calloc(1, sizeof(char *));

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    size_t data_buf_size = 0;
    int num_of_wires = 1;

    if(ingest_comma_delim_char(&data_buf, &data_buf_size, &num_of_wires, INPUT_PATH) == STATUS_BAD) {
        LOG_ERROR("Failed to ingest data (ingest_comma_delim())");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    LOG_INFO("Number of wires: %d", num_of_wires);

    for (int i = 0; i < data_buf_size; i++) {
        LOG_DEBUG("%s", data_buf[i]);
    }

    LOG_DEBUG("Again: %s", data_buf[0]);

    free(data_buf);
    data_buf = NULL;

    return 0;
}