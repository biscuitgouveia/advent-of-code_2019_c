#include <stdio.h>
#include <string.h>
#include <ethan_utilities.h>
#include <logger.h>
#include <puzzles.h>

#define INPUT_PATH "./Day 3/example_input_1"
#define UP 'U'
#define DOWN 'D'
#define LEFT 'L'
#define RIGHT 'R'

typedef enum {
    HORIZONTAL,
    VERTICAL
} polarity_t;

typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    point_t start;
    point_t end;
    polarity_t polarity;
} segment_t;

 typedef struct {
    char direction;
    int distance;
} movement_t;

typedef struct {
    movement_t *movements;
    point_t *points;
    segment_t *segments;
    size_t num_movements;
} wire_t;

status_t segment_calc_polarity(segment_t *segment) {
    if (segment->start.x == segment->end.x) {
        segment->polarity = VERTICAL;
    } else if (segment->start.y == segment->end.y) {
        segment->polarity = HORIZONTAL;
    } else {
        LOG_ERROR("Invalid segment: start (%i, %i), end (%i, %i)", segment->start.x, segment->start.y, segment->end.x, segment->end.y);
        return STATUS_BAD;
    }
    return STATUS_GOOD;
}

int wire_get_distance(point_t *point) {
    return abs(point->x) + abs(point->y);
}

status_t util_find_duplicates(point_t *arr1, int arr1_size, point_t *arr2, int arr2_size, point_t **duplicates, int *num_duplicates) {
    int i = 0;
    int j = 0;

    while (i < arr1_size && j < arr2_size) {
        int dist1 = wire_get_distance(&arr1[i]);
        int dist2 = wire_get_distance(&arr2[j]);
        LOG_INFO("Comparing points: (%i, %i) and (%i, %i) with distances %i and %i", arr1[i].x, arr1[i].y, arr2[j].x, arr2[j].y, dist1, dist2);
        if (dist1 == dist2) {
            (*num_duplicates)++;
            if (*num_duplicates > 1) {
                if (resize_dynamic_array((void **)duplicates, *num_duplicates, sizeof(point_t)) == STATUS_BAD) {
                    LOG_ERROR("Failed to resize duplicates array");
                    free(*duplicates);
                    *duplicates = NULL;
                    return STATUS_BAD;
                }
            }
            (*duplicates)[*num_duplicates - 1] = arr1[i];
            i++;
            j++;
        } else if (dist1 < dist2) {
            i++;
        } else {
            j++;
        }
    }
    return STATUS_GOOD;
}

int compare_points(point_t *p1, point_t *p2) {
    int d1 = wire_get_distance(p1);
    int d2 = wire_get_distance(p2);

    if (d1 < d2) {
        return -1;
    } else if (d1 > d2) {
        return 1;
    } else {
        return 1;
    }
}

status_t wire_compute_movement(wire_t *wire) {
    switch(wire->movements[wire->num_movements - 1].direction) {
        case UP:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x,
                .y = wire->points[wire->num_movements - 1].y + wire->movements[wire->num_movements - 1].distance
            };
            break;
        case DOWN:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x,
                .y = wire->points[wire->num_movements - 1].y - wire->movements[wire->num_movements - 1].distance
            };
            break;
        case LEFT:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x - wire->movements[wire->num_movements - 1].distance,
                .y = wire->points[wire->num_movements - 1].y
            };
            break;
        case RIGHT:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x + wire->movements[wire->num_movements - 1].distance,
                .y = wire->points[wire->num_movements - 1].y
            };
            break;
        default:
            LOG_ERROR("Invalid direction: %c", wire->movements[wire->num_movements - 1].direction);
            return STATUS_BAD;

    }
    return STATUS_GOOD;
}

status_t wire_init(wire_t *wire, char **raw_movements) {
    wire->num_movements = 0;
    wire->points = calloc(2, sizeof(point_t));
    wire->points[0] = (point_t) { .x = 0, .y = 0 };
    wire->points[1] = (point_t) { .x = 0, .y = 0 };
    wire->segments = calloc(1, sizeof(segment_t));
    wire->movements = calloc(1, sizeof(movement_t));
    
    char *strptr = strtok(*raw_movements, ",");
    while (strptr != NULL) {
        wire->num_movements++;

        char *direction = strndup(strptr, 1);
        int distance = atoi(strptr + 1);

        LOG_DEBUG("Direction: %s, Distance: %i", direction, distance);
        
        if ((wire)->num_movements > 1) {
            if(resize_dynamic_array((void **)(&(wire)->movements), (wire)->num_movements, sizeof(movement_t)) == STATUS_BAD) {
                free(direction);
                LOG_ERROR("Failed to resize movements array");
                return STATUS_BAD;
            }
            if(resize_dynamic_array((void **)(&(wire)->points), (wire)->num_movements+1, sizeof(point_t)) == STATUS_BAD) {
                free(direction);
                free(wire);
                wire = NULL;
                return STATUS_BAD;
            }
        }
        
        (wire)->movements[(wire)->num_movements - 1] = (movement_t) { .direction = direction[0], .distance = distance };
        if (wire->num_movements > 0) {
            if(wire_compute_movement(wire) == STATUS_BAD) {
                LOG_ERROR("Failed to compute movement");
                return STATUS_BAD;
            }
        }
        LOG_DEBUG("Point: (%i, %i)", wire->points[wire->num_movements].x, wire->points[wire->num_movements].y);
        strptr = strtok(NULL, ",");
        free(direction);
    }
    free(strptr);
    strptr = NULL;
    return STATUS_GOOD;
}

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
        qsort(wires[i].points, wires[i].num_movements + 1, sizeof(point_t), (int (*)(const void *, const void *))compare_points);
    }

    int num_duplicates = 0;
    point_t *duplicates = calloc(1, sizeof(point_t));
    if (util_find_duplicates(wires[0].points, wires[0].num_movements + 1, wires[1].points, wires[1].num_movements + 1, &duplicates, &num_duplicates) == STATUS_BAD) {
        LOG_ERROR("Failed to find duplicates");
        free(data_buf);
        data_buf = NULL;
        free(wires);
        wires = NULL;
        return -1;
    }

    for (int i = 0; i < num_duplicates; i++) {
        LOG_INFO("Duplicate %i: (%i, %i), Distance: %i", i, duplicates[i].x, duplicates[i].y, wire_get_distance(&duplicates[i]));
    }

    free(data_buf);
    data_buf = NULL;
    free(wires);
    wires = NULL;

    return 0;
}