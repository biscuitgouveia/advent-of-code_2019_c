#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <logger.h>
#include <puzzles.h>

#define INPUT_PATH "./Day 4/input"

void split_digits(int *digits, int input) {
    int counter = 0;
    while (input) {
        digits[counter] = input % 10;
        input /= 10;
        counter++;
    }
}

bool test_sorted(int *digits) {
    int temp = -1;
    for (int i = 0; i < 6; i++) {
        if (digits[i] > temp && temp != -1) {
            return false;
        }
        temp = digits[i];
    }
    return true;
}

bool test_recurring(int *digits) {
    int temp = -1;
    for (int i = 0; i < 6; i++) {
        if (digits[i] == temp) {
            // LOG_DEBUG("Matching digits found: %d%d%d%d%d%d", digits[0], digits[1], digits[2], digits[3], digits[4], digits[5]);
            return true;
        }
        temp = digits[i];
    }
    return false;
}

bool test_recurring_2(int *digits) {
   int temp = -1;
   int count_recurring = 1;
    for (int i = 1; i < 6; i++) {
        
        temp = digits[i-1];
        
        if (digits[i] == temp) {
            count_recurring++;
            
        } else if (digits[i] != temp && count_recurring == 2) {
            return true;
        } else {
            count_recurring = 1;
        }
    }
    if (count_recurring == 2) {
        return true;
    }
    return false;
}


int day_4() {
    
    int min = 367479;
    int max = 893698;

    int num_of_passwords = 0;

    int *digits = calloc(6, sizeof(int));

    for (int i = min; i <= max; i++) {
        split_digits(digits, i);
        if (test_recurring(digits) && test_sorted(digits)) {
            num_of_passwords++;
        }
    }

    LOG_INFO("Number of passwords (Part 1): %d", num_of_passwords);

    int num_of_passwords_2 = 0;

    for (int i = min; i <= max; i++) {
        split_digits(digits, i);
        if (test_recurring_2(digits) && test_sorted(digits)) {
            LOG_DEBUG("Candidate found: %d%d%d%d%d%d", digits[5], digits[4], digits[3], digits[2], digits[1], digits[0]);
            num_of_passwords_2++;
        }
    }
    
    LOG_INFO("Number of passwords (Part 2): %d", num_of_passwords_2);

    free(digits);
    digits = NULL;

    return 0;

}

