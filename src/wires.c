#include <wires.h>

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

bool check_ccw(point_t A, point_t B, point_t C) {
    return (C.y - A.y) * (B.x - A.x) > (B.y - A.y) * (C.x - A.x);
}

bool check_intersecting(segment_t A, segment_t B) {
    if (A.polarity == B.polarity) {
        LOG_DEBUG("Matching Polarity between two segments: (%d, %d) to (%d, %d), (%d, %d) to (%d, %d)", A.start.x, A.start.y, A.start.x, A.end.y, B.start.x, B.start.y, B.end.x, B.end.y);
        return false;
    }
    return check_ccw(A.start, B.start, B.end) != check_ccw(A.end, B.start, B.end)
     && check_ccw(A.start, A.end, B.start) != check_ccw(A.start, A.end, B.end);
}

point_t find_intersection(segment_t A, segment_t B) {
    LOG_INFO("Testing Segments: (%d, %d) to (%d, %d), (%d, %d) to (%d, %d)", A.start.x, A.start.y, A.start.x, A.end.y, B.start.x, B.start.y, B.end.x, B.end.y);
    if (!check_intersecting(A, B)) {
        LOG_DEBUG("No intersection: (%d, %d) to (%d, %d), (%d, %d) to (%d, %d)", A.start.x, A.start.y, A.start.x, A.end.y, B.start.x, B.start.y, B.end.x, B.end.y);
        return (point_t) {.x = 0, .y=0};
    }

    int x1 = A.start.x;
    int y1 = A.start.y;
    int x2 = A.end.x;
    int y2 = A.end.y;

    int x3 = B.start.x;
    int y3 = B.start.y;
    int x4 = B.end.x;
    int y4 = B.end.y;

    LOG_DEBUG("Segments: (%d, %d) to (%d, %d) and (%d, %d) to (%d, %d)", x1, y1, x2, y2, x3, y3, x4, y4);

    point_t intersection = {0};

    intersection.x = ((((x1*y2)-(y1*x2))*(x3-x4)) - ((x1-x2)*((x3*y4)-(y3*x4)))) / (((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4)));
    intersection.y = ((((x1*y2)-(y1*x2))*(y3-y4)) - ((y1-y2)*((x3*y4)-(y3*x4)))) / (((x1-x2)*(y3-y4)) - ((y1-y2)*(x3-x4)));

    return intersection;

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
            wire->segments[wire->num_movements] = (segment_t) {
                .polarity = VERTICAL,
                .start = wire->points[wire->num_movements-1],
                .end = wire->points[wire->num_movements]
            };
            break;
        case DOWN:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x,
                .y = wire->points[wire->num_movements - 1].y - wire->movements[wire->num_movements - 1].distance
            };
            wire->segments[wire->num_movements] = (segment_t) {
                .polarity = VERTICAL,
                .start = wire->points[wire->num_movements-1],
                .end = wire->points[wire->num_movements]
            };
            break;
        case LEFT:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x - wire->movements[wire->num_movements - 1].distance,
                .y = wire->points[wire->num_movements - 1].y
            };
            wire->segments[wire->num_movements] = (segment_t) {
                .polarity = HORIZONTAL,
                .start = wire->points[wire->num_movements-1],
                .end = wire->points[wire->num_movements]
            };
            break;
        case RIGHT:
            wire->points[wire->num_movements] = (point_t) {
                .x = wire->points[wire->num_movements - 1].x + wire->movements[wire->num_movements - 1].distance,
                .y = wire->points[wire->num_movements - 1].y
            };
            wire->segments[wire->num_movements] = (segment_t) {
                .polarity = HORIZONTAL,
                .start = wire->points[wire->num_movements-1],
                .end = wire->points[wire->num_movements]
            };
            break;
        default:
            LOG_ERROR("Invalid direction: %c", wire->movements[wire->num_movements - 1].direction);
            return STATUS_BAD;

    }

    return STATUS_GOOD;
}

void wire_free(wire_t *wire) {
    free(wire->points);
    free(wire->segments);
    free(wire->movements);

    wire->points = NULL;
    wire->segments = NULL;
    wire->movements = NULL;

    free(wire);
    wire = NULL;
}

// Initialises a wire with all of its points, segments, and the number of movements.
//
//  params:
//      - wire: Pointer to a wire object.
//      - raw_movements: Double pointer to a string of movements formatted as {[DIRECTION][NUMBER],}.
//
//  returns STATUS_GOOD or STATUS_BAD.
status_t wire_init(wire_t *wire, char **raw_movements) {
    // Initialise wire with empty values
    wire->num_movements = 0;
    wire->points = calloc(2, sizeof(point_t));
    // wire->points[0] = (point_t) { .x = 0, .y = 0 };
    // wire->points[1] = (point_t) { .x = 0, .y = 0 };
    wire->segments = calloc(2, sizeof(segment_t));
    wire->movements = calloc(1, sizeof(movement_t));
    
    // Separate raw_movements by comma
    char *strptr = strtok(*raw_movements, ",");
    while (strptr != NULL) {
        wire->num_movements++;

        char *direction = strndup(strptr, 1);
        int distance = atoi(strptr + 1);

        LOG_DEBUG("Direction: %s, Distance: %i", direction, distance);
        
        if ((wire)->num_movements > 1) {
            // Expand movements array
            if(resize_dynamic_array((void **)(&(wire)->movements), (wire)->num_movements, sizeof(movement_t)) == STATUS_BAD) {
                free(direction);
                free(strptr);
                direction = NULL;
                strptr = NULL;
                wire_free(wire);
                LOG_ERROR("Failed to resize movements array");
                return STATUS_BAD;
            }
            // Expand points array
            if(resize_dynamic_array((void **)(&(wire)->points), (wire)->num_movements+1, sizeof(point_t)) == STATUS_BAD) {
                free(direction);
                free(strptr);
                direction = NULL;
                strptr = NULL;
                wire_free(wire);
                LOG_ERROR("Failed to resize movements array");
                return STATUS_BAD;
            }
            // Expand segments array
            if(resize_dynamic_array((void **)(&(wire)->segments), (wire)->num_movements+1, sizeof(segment_t)) == STATUS_BAD) {
                free(direction);
                free(strptr);
                direction = NULL;
                strptr = NULL;
                wire_free(wire);
                LOG_ERROR("Failed to resize movements array");
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