#include <ethan_utilities.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <logger.h>

status_t resize_dynamic_array(void **arr, size_t new_size, size_t element_size) {
    int *temp = reallocarray(*arr, new_size, element_size);

    if (temp == NULL) {
        // Handle realloc failure
        printf("Memory allocation failed (resize_dynamic_array())\n");
        return STATUS_BAD;
    }

    *arr = temp;
    
    return STATUS_GOOD;
}

status_t ingest_comma_delim(int **buf, size_t *buf_size, char *filepath) {
    // open file stream and check for errors
    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        perror("fopen");
        free(*buf);
        *buf = NULL;
        return STATUS_BAD;
    }

    char *lineptr = NULL;
    size_t line_buf_size = 0;
    
    size_t new_size = 0;

    while(getdelim(&lineptr, &line_buf_size, ',', fp) != -1) {

        int temp = atoi(lineptr);
        new_size += 1;

        if (new_size > 1) {
            // clean everything up if resizing array fails
            if (resize_dynamic_array((void **)buf, new_size, sizeof(int)) == STATUS_BAD) {
                printf("\tMemory allocation failed (ingest_comma_delim())\n");
                printf("\t\tCurrent new_size: %li\n", new_size);
                free(*buf);
                *buf = NULL;
                free(lineptr);
                lineptr = NULL;
                fclose(fp);
                return STATUS_BAD;
            }
        }
        
        (*buf)[new_size-1] = temp;
    }

    fclose(fp);
    free(lineptr);
    lineptr = NULL;
    *buf_size = new_size;

    return STATUS_GOOD;
}

status_t ingest_comma_delim_char(char ***buf, size_t *buf_size, int *num_of_lines, char *filepath) {
    // open file stream and check for errors
    FILE *fp = fopen(filepath, "r");

    if (fp == NULL) {
        perror("fopen");
        free(*buf);
        *buf = NULL;
        return STATUS_BAD;
    }

    char *lineptr = NULL;
    size_t line_buf_size = 0;
    
    size_t new_size = 0;

    while(getdelim(&lineptr, &line_buf_size, ',', fp) != -1) {

        char *temp = lineptr;
        printf("temp: %s\n", temp);
        new_size += 1;

        if (new_size > 1) {
            // clean everything up if resizing array fails
            if (resize_dynamic_array((void **)buf, new_size, sizeof(char *)) == STATUS_BAD) {
                printf("\tMemory allocation failed (ingest_comma_delim())\n");
                printf("\t\tCurrent new_size: %li\n", new_size);
                free(*buf);
                *buf = NULL;
                free(lineptr);
                lineptr = NULL;
                fclose(fp);
                return STATUS_BAD;
            }
        }

        // Remove trailing comma if present
        char *comma = strchr(temp, ',');
        if (comma != NULL) {
            *comma = '\0';
        }

        char *newline = strchr(temp, '\n');
        if (newline) {
            *num_of_lines += 1;
            LOG_INFO("New line detected. Total lines: %d", *num_of_lines);
        }

        (*buf)[new_size-1] = strdup(temp);

        printf("%s\n", (*buf)[new_size-1]);
    }

    fclose(fp);
    free(lineptr);
    lineptr = NULL;
    *buf_size = new_size;

    return STATUS_GOOD;
}