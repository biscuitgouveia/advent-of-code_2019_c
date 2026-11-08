#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ethan_utilities.h>

int main() {

    // Create data buffer for part 2 which will automatically
    // expand with each line read from the file
    size_t new_size = 0;

    int *data_buf = calloc(1, sizeof(int));

    if (data_buf == NULL) {
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    // open file stream and check for errors
    FILE *fp = fopen("input", "r");

    if (fp == NULL) {
        perror("fopen");
        free(data_buf);
        data_buf = NULL;
        return -1;
    }

    int total_part_1 = 0;
    int total_part_2 = 0;

    // read line by line from file stream
    char *lineptr = NULL;
    size_t line_buf_size = 0;

    while(getline(&lineptr, &line_buf_size, fp) != -1) {
        // calculate each value for part 1
        int temp_base = atoi(lineptr);
        double temp_floor = floor(temp_base/3);
        temp_floor -= 2;

        // expand and add each individually to data_buf to handle
        // later in part 2
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

        data_buf[new_size-1] = temp_floor;
        total_part_1 += (int)temp_floor;

    }

    free(lineptr);
    fclose(fp);

    printf("Part 1: %i\n", total_part_1);

    // iterate over all values in data_buf and calculate
    // individual fuel need expansion
    for (int i = 0; i < new_size; i++) {

        int part_2_temp = floor(data_buf[i]/3) - 2;
        int part_2_seg = data_buf[i] + part_2_temp;

        while (part_2_temp > 0) {
            part_2_temp = floor(part_2_temp/3) - 2;

            // catch any sneaky values less than 1;
            if (part_2_temp > 0) {
                part_2_seg += part_2_temp;
            }
            
        }

        total_part_2 += part_2_seg;
    }

    free(data_buf);
    data_buf = NULL;
    
    printf("Part 2: %i\n", total_part_2);
    return 0;

}