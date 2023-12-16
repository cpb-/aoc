#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define UP      1
#define DOWN    2
#define LEFT    4
#define RIGHT   8



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



char **energy_array = NULL;


void allocate_energy_array(void)
{
	energy_array = malloc(height * sizeof(char *));
	for (int i = 0; i < height; i++) {
		energy_array[i] = malloc(width + 1);
		memset(energy_array[i], 0, width);
	}
}



void clear_energy_array(void)
{
	for (int i = 0; i < height; i++) {
		memset(energy_array[i], 0, width);
	}
}



void start_beam(int x, int y, int dir)
{
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height) )
		return;

	if ((energy_array[y][x] & dir) != 0)
		return;

	energy_array[y][x] |= dir;

	if (array[y][x] == '.') {
		start_beam(x + (dir == RIGHT) - (dir == LEFT), y + (dir == DOWN) - (dir == UP), dir);

	} else if (array[y][x] == '\\') {
		start_beam(x + (dir == DOWN) - (dir == UP), y - (dir == LEFT) + (dir == RIGHT),
		           dir == LEFT ? UP : dir == RIGHT ? DOWN : dir == DOWN ? RIGHT : LEFT);

	} else if (array[y][x] == '/') {
		start_beam(x - (dir == DOWN) + (dir == UP), y + (dir == LEFT) - (dir == RIGHT),
		           dir == LEFT ? DOWN : dir == RIGHT ? UP : dir == UP ? RIGHT : LEFT);
	} else if (array[y][x] == '-') {
		if ((dir == LEFT) || (dir == RIGHT)) {
			start_beam(x + (dir == RIGHT) - (dir == LEFT), y, dir);
		} else {
			start_beam(x - 1, y, LEFT);
			start_beam(x + 1, y, RIGHT);
		}
	} else if (array[y][x] == '|') {
		if ((dir == UP) || (dir == DOWN)) {
			start_beam(x, y + (dir == DOWN) - (dir == UP), dir);
		} else {
			start_beam(x, y - 1, UP);
			start_beam(x, y + 1, DOWN);
		}
	}
}



int compute_energy(void)
{
	int sum = 0;

	for (int y = 0; y < height; y ++)
		for (int x = 0; x < width; x++)
			sum += (energy_array[y][x] != 0);
	return sum;
}



void part_1(void)
{
	load_array();
	allocate_energy_array();
	start_beam(0, 0, RIGHT);


	printf("  Sum = %d\n", compute_energy());
}



void part_2(void)
{
	int max = 0;

	load_array();
	allocate_energy_array();

	for (int i = 0; i < width; i ++) {
		start_beam(i, 0, DOWN);
		int c = compute_energy();
		if (c > max)
			max = c;
		clear_energy_array();

		start_beam(i, height -1, UP);
		c = compute_energy();
		if (c > max)
			max = c;
		clear_energy_array();

		start_beam(0, i, RIGHT);
		c = compute_energy();
		if (c > max)
			max = c;
		clear_energy_array();

		start_beam(width - 1, i, LEFT);
		c = compute_energy();
		if (c > max)
			max = c;
		clear_energy_array();
	}

	printf("  Max = %d\n", max);
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
