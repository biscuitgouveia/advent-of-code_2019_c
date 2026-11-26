#ifndef ETHAN_UTILITIES_H

#include <string.h>
#include <stdbool.h>
#include <ethan_utilities.h>
#include <logger.h>

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

status_t segment_calc_polarity(segment_t *segment);
int wire_get_distance(point_t *point);
status_t util_find_duplicates(point_t *arr1, int arr1_size, point_t *arr2, int arr2_size, point_t **duplicates, int *num_duplicates);
int compare_points(point_t *p1, point_t *p2);
status_t wire_compute_movement(wire_t *wire);
status_t wire_init(wire_t *wire, char **raw_movements);
point_t find_intersection(segment_t A, segment_t B);


#endif