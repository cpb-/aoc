#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
int **gear_count = NULL;
int **gear_power = NULL;

int width = 0;
int height = 0;

#define GEAR        '*'
#define EMPTY_CELL  '.'



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
		gear_count = realloc(gear_count, (height + 1) * sizeof(int *));
		gear_power = realloc(gear_power, (height + 1) * sizeof(int *));

		if ((array == NULL) || (gear_count == NULL) || (gear_power == NULL)) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		height ++;

		array[height - 1] = malloc(width + 1);
		gear_count[height - 1] = calloc(width + 1, sizeof(int));
		gear_power[height - 1] = calloc(width + 1, sizeof(int));

		if ((array[height - 1] == NULL) || (gear_count[height - 1] == NULL) || (gear_power[height - 1] == NULL)) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}

		strncpy(array[height - 1], line, width + 1);
		for (int i = 0; i < width; i ++) {
			gear_count[height - 1][i] = 0;
			gear_power[height - 1][i] = 1;
		}
	}
	return 0;
}



int is_symbol(int y, int x, int number)
{
	if (array[y][x] == GEAR) {
		gear_count[y][x] ++;
		gear_power[y][x] *= number;
	}

	return (array[y][x] != EMPTY_CELL);
}



int is_adjacent_to_symbol(int y, int x1, int x2, int number)
{
	if (y > 0) {
		if (x1 > 0)
			if (is_symbol(y - 1, x1 - 1, number))
				return 1;
		for (int i = x1; i <= x2; i++) {
			if (is_symbol(y - 1, i, number))
				return 1;
		}
		if (x2 < width - 1)
			if (is_symbol(y - 1, x2 + 1, number))
				return 1;
	}
	if (y < height - 1) {
		if (x1 > 0)
			if (is_symbol(y + 1, x1 - 1, number))
				return 1;
		for (int i = x1; i <= x2; i++) {
			if (is_symbol(y + 1, i, number))
				return 1;
		}
		if (x2 < width - 1)
			if (is_symbol(y + 1, x2 + 1, number))
				return 1;
	}
	if (x1 > 0) {
		if (is_symbol(y, x1 - 1, number))
			return 1;
	}
	if (x2 < width - 1) {
		if (is_symbol(y, x2 + 1, number))
			return 1;
	}

	return 0;
}



int extract_numbers_adjacents_to_symbol_in_line(int y)
{
	int x = 0;
	int start_x;
	int end_x;
	int number;
	int sum = 0;

	while (array[y][x] != '\0') {
		while ((! isdigit(array[y][x])) && (x < width))
			x++;
		if (x == width)
			break;
		start_x = x;
		number = 0;
		while (isdigit(array[y][x])) {
			end_x = x;
			number *= 10;
			number += (array[y][x] - '0');
			x++;
		}
		if (is_adjacent_to_symbol(y, start_x, end_x, number)) {
			sum += number;
		}
	}
	return sum;
}



void display_array(void)
{
	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < width; x ++)
			fprintf(stdout, "%c", array[y][x]);
		fprintf(stdout, "\n");
	}
}



void part_1(void)
{
	int sum = 0;

	load_array();
//	display_array();

	for (int y = 0; y < height; y++)
		sum += extract_numbers_adjacents_to_symbol_in_line(y);

	fprintf(stdout, "    Sum: %d\n", sum);
}



void part_2(void)
{
	int sum = 0;

	load_array();

	for (int y = 0; y < height; y++)
		extract_numbers_adjacents_to_symbol_in_line(y);

	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x ++)
			if (array[y][x] == '*')
				if (gear_count[y][x] == 2)
					sum += gear_power[y][x];

	fprintf(stdout, "    Sum: %d\n", sum);
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
