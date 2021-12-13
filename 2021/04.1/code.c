#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int *drawn_numbers;
int nb_drawn_numbers;


typedef struct {
	int number [25];
	int selected [25];
} grid_t;

grid_t *grids = NULL;
int nb_grids;


void read_drawn_numbers(void);
void add_drawn_number(int value);
int read_new_grid(void);
void draw_numbers(void);
void add_new_grid(grid_t *grid);
int read_blank_line(void);
int read_grid_row(grid_t *grid, int row);
int draw_number(int number);
int grid_contains_number(int number, int grid);
int one_row_selected(int grid);
int one_column_selected(int grid);
void winner_is(int grid, int last_number);

int main(int argc, char *argv[])
{
	read_drawn_numbers();

	while (read_new_grid() == 0)
		;

	fprintf(stderr, "Found %d grids\n", nb_grids);
	draw_numbers();

	return EXIT_SUCCESS;
}


void read_drawn_numbers(void)
{
	char buffer[4096];

	if (fgets(buffer, 4095, stdin) == NULL) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}

	int start = 0;
	int value;
	int consummed;
	while (sscanf(&(buffer[start]), "%d%n", &value, &consummed) == 1) {
		add_drawn_number(value);
		start += consummed;
		if (buffer[start] == ',')
			start ++;
		if (! isdigit(buffer[start]))
			break;
	}
	fprintf(stderr, "Found %d numbers\n", nb_drawn_numbers);
}


void add_drawn_number(int value)
{
	drawn_numbers = realloc(drawn_numbers, (nb_drawn_numbers + 1) * sizeof(int));
	if (drawn_numbers == NULL) {
		perror("calloc drawn number");
		exit(EXIT_FAILURE);
	}
	drawn_numbers[nb_drawn_numbers] = value;
	nb_drawn_numbers ++;
}


int read_new_grid(void)
{
	grid_t grid;
	if (read_blank_line() != 0) {
		fprintf(stderr, "No more blank line after %d grids\n", nb_grids);
		return -1;
	}
	for (int i = 0; i < 5; i ++)
		if (read_grid_row(&grid, i) != 0)
			return -1;
	for (int i = 0; i < 25; i ++)
		grid.selected[i] = 0;
	add_new_grid(&grid);
	return 0;
}


void add_new_grid(grid_t *grid)
{
	grids = realloc(grids, (nb_grids + 1) * sizeof(grid_t));
	if (grids == NULL) {
		perror("calloc grid");
		exit(EXIT_FAILURE);
	}
	memcpy(&(grids[nb_grids]), grid, sizeof(grid_t));
	nb_grids ++;
}


int read_blank_line(void)
{
	char buffer[128];

	if (fgets(buffer, 127, stdin) == NULL)
		return -1;
	for (int i = 0; buffer[i] != '\0'; i++) {
		if (! isspace(buffer[i]))
			return -1;
	}
	return 0;
}


int read_grid_row(grid_t *grid, int row)
{
	char buffer[256];

	if (fgets(buffer, 255, stdin) == NULL)
		return -1;
	int start = 0;
	int consummed;
	int value;
	for (int i = 0; i < 5; i++) {
		if (sscanf(&(buffer[start]), "%d%n", &value, &consummed) != 1)
			return -1;
		start += consummed;
		grid->number[row * 5 + i] = value;
	}
	return 0;
}


void draw_numbers(void)
{
	for (int i = 0; i < nb_drawn_numbers; i ++)
		if (draw_number(drawn_numbers[i]) != 0)
			return;
}



int draw_number(int number)
{
	for (int i = 0; i < nb_grids; i++) {
		if (grid_contains_number(number, i)) {
			if ((one_row_selected(i)) || (one_column_selected(i))) {
				winner_is(i, number);
				return -1;
			}
		}
	}
	return 0;
}


int grid_contains_number(int number, int grid)
{
	int found = 0;
	for (int i = 0; i < 5; i ++) {
		for (int j = 0; j < 5; j ++) {
			if (grids[grid].number[i * 5 + j] == number) {
				grids[grid].selected[i * 5 + j] = 1;
				found = 1;
			}

		}
	}
	return found;
}


int one_row_selected(int grid)
{
	for (int i = 0; i < 5; i ++) {
		int j;
		for (j = 0; j < 5; j ++) {
			if (grids[grid].selected[i*5+j] == 0)
				break;
		}
		if (j == 5)
			return 1;
	}
	return 0;
}


int one_column_selected(int grid)
{
	for (int i = 0; i < 5; i ++) {
		int j;
		for (j = 0; j < 5; j ++) {
			if (grids[grid].selected[i+j*5] == 0)
				break;
		}
		if (j == 5)
			return 1;
	}
	return 0;
}


void winner_is(int grid, int last_number)
{
	int sum = 0;
	for (int i = 0; i < 5; i ++) {
		for (int j = 0; j < 5; j ++) {
			if (grids[grid].selected[i*5+j] == 0)
				sum += grids[grid].number[i*5+j];
		}
	}
	fprintf(stdout, "The winner is %d, the sum is %d, the last number is %d, result is %d\n",
		grid, sum, last_number, sum * last_number);
}

