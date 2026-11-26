#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <logger.h>
#include <puzzles.h>
#include <wires.h>

#define INPUT_PATH "./Day 3/input"

int day_3() {

    char **data_buf = calloc(1, sizeof(char *));

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    size_t data_buf_size = 0;

    if(util_split_lines(&data_buf, &data_buf_size, INPUT_PATH) == STATUS_BAD) {
        LOG_ERROR("Failed to ingest data");
        free(data_buf);
        data_buf = NULL;
        return -1;
    };

    for (int i = 0; i < data_buf_size; i++) {
        LOG_DEBUG("Line %i: %s", i, data_buf[i]);
    }

    wire_t *wires = calloc(data_buf_size, sizeof(wire_t));

    for (int i = 0; i < data_buf_size; i++) {
        wires[i] = (wire_t) { .movements = NULL, .points = NULL, .segments = NULL, .num_movements = 0 };
        if(wire_init(&wires[i], &data_buf[i]) == STATUS_BAD) {
            LOG_ERROR("Failed to initialise wire %i", i);
            free(data_buf);
            data_buf = NULL;
            return -1;
        }
    }

    int distance = 0;

    for (int i = 0; i < wires[0].num_movements+1; i++) {
        for (int j = 0; j < wires[1].num_movements+1; j++) {
            LOG_ERROR("i: %d, j: %d", i, j);
            point_t intersection = find_intersection(wires[0].segments[i], wires[1].segments[j]);
            if (intersection.x == 0 && intersection.y == 0) {
                LOG_INFO("No intersection!");
            } else {
                int temp_distance = wire_get_distance(&intersection);
                LOG_WARN("Temp Distance: %d", temp_distance);
                if (temp_distance < distance && temp_distance > 0) {
                    distance = temp_distance;
                }

                if (distance == 0) {
                    distance = temp_distance;
                }
            }
            LOG_INFO("Intersection Found: (%d, %d)", intersection.x, intersection.y);
        }
    }

    free(data_buf);
    data_buf = NULL;
    free(wires);
    wires = NULL;

    LOG_INFO("DISTANCE: %d", distance);

    return 0;
}