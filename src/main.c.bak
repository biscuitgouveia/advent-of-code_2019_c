#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>

#include <ethan_utilities.h>
#include <puzzles.h>

int main(int argc, char *argv[]) { 
	char *dayarg = NULL;
	int d = 0;
	int c;

	while ((c = getopt(argc, argv, "d:")) != -1) {
		switch (c) {
			case 'd':
				dayarg = optarg;
				d = atoi(dayarg);
				break;
			case '?':
				printf("Unknown option -%c\n", c);
				break;
			default:
				return -1;
		}
	}

    switch(d) {
        case 1:
            day_1();
            break;
        case 2:
            day_2();
            break;
        default:
            return -1;
    }

    return 0;

}