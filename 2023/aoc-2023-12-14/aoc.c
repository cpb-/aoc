#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



char **array = NULL;
int width = 0;
int height = 0;



int load_array(void)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		if (width == 0) {
			width = strlen(line);
		}
		array = realloc(array, (height + 1) * sizeof(char *));
		if (array == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		height ++;

		array[height - 1] = malloc(width + 1);
		if (array[height - 1] == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strncpy(array[height - 1], line, width + 1);
	}
	return 0;
}



void display_array(void)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++)
			fprintf(stderr, "%c", array[i][j]);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}



char **array_backup = NULL;


void backup_array(void)
{
	array_backup = malloc(height * sizeof(char *));
	for (int i = 0; i < height; i++) {
		array_backup[i] = malloc(width + 1);
		strcpy(array_backup[i], array[i]);
	}
}



int array_equals_to_backup(void)
{
	for (int i = 0; i < height; i++)
		if (strcmp(array_backup[i], array[i]) != 0)
			return 0;
	return 1;
}



void tilt_north(void)
{
	for (int row = 0; row < height; row++) {
		for (int col = 0; col < width; col++) {
			if (array[row][col] == 'O') {
				for (int r = row - 1;(r >= 0) && (array[r][col] == '.'); r--) {
					array[r][col] = 'O';
					array[r + 1][col] = '.';
				}
			}
		}
	}
}



long long int compute_load_on_north(void)
{
	long long int sum = 0;
	for (int row = 0; row < height; row ++) {
		for (int col = 0; col < width; col++) {
			if (array[row][col] == 'O') {
				sum += height - row;
			}
		}
	}
	return sum;
}



void rotate_array_90_clockwise(void)
{
	char **new_array = malloc(height * sizeof(char *));
	for (int i = 0; i < height; i++) {
		new_array[i] = malloc(width);
		for (int j = 0; j < width; j++)
			new_array[i][j] = array[width - j - 1][i];
	}
	for (int i = 0; i < height; i++)
		free(array[i]);
	free(array);
	array = new_array;

}



void full_cycle(void)
{
	tilt_north();
	rotate_array_90_clockwise();
	tilt_north();
	rotate_array_90_clockwise();
	tilt_north();
	rotate_array_90_clockwise();
	tilt_north();
	rotate_array_90_clockwise();
}



void part_1(void)
{
	load_array();
	tilt_north();
	printf("  Total load = %lld\n", compute_load_on_north());
}



void part_2(void)
{
	int repetitions = 0;
	int cycles = 0;
	int start = 1000;

	load_array();

	for (cycles = 0; cycles < 1000000000; cycles++) {
		if (cycles == start) {
			backup_array();
		}
		if (cycles > start) {
			if (array_equals_to_backup()) {
				repetitions = cycles - start;
				fprintf(stdout, "\n\nFound a repetition every %d cycles!\n", repetitions);
				break;
			}
		}
		full_cycle();
	}
	int remaining = (1000000000 - cycles) % repetitions;
	for (int i = 0; i < remaining; i ++)
		full_cycle();

	printf("  Total load = %lld\n", compute_load_on_north());
}



int main(int argc, char *argv[])
{
	int part;

	if ((argc < 2) || (sscanf(argv[1], "%d", &part) != 1)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	switch(part) {
		case 1:
			part_1();
			break;
		case 2:
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
