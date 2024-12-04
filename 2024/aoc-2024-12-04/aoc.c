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


int check_pattern(char *pattern, int x, int y, int dx, int dy)
{
if ((x == 0) && (y == 0)) fprintf(stderr, "(%d, %d)", dx, dy);
	if (pattern[0] == '\0')
		return 1;
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return 0;
	if (array[y][x] != pattern[0])
		return 0;
	return check_pattern(&(pattern[1]), x + dx, y + dy, dx, dy);
}


void part_1(void)
{
	char *pattern = "XMAS";
	char deltas[8][2] = { { 1, 0}, { 1, 1}, {1, -1}, {0, 1}, {0, -1}, {-1, 1}, {-1, 0}, {-1, -1}};
	int sum = 0;

	load_array();

	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			for (int d = 0; d < 8; d ++)
				sum += check_pattern(pattern,x, y, deltas[d][0], deltas[d][1]);
	printf ("Sum = %d\n", sum);
}


void part_2(void)
{
	int sum = 0;

	load_array();

	for (int x = 1; x < width - 1; x++)
		for (int y = 1; y < height - 1; y++) {

			if (array[y][x] != 'A')
				continue;
			if ((array[y-1][x-1] != 'S') && (array[y-1][x-1] != 'M'))
				continue;
			if ((array[y-1][x-1] == 'M') && (array[y+1][x+1] != 'S'))
				continue;
			if ((array[y-1][x-1] == 'S') && (array[y+1][x+1] != 'M'))
				continue;

			if ((array[y-1][x+1] != 'S') && (array[y-1][x+1] != 'M'))
				continue;
			if ((array[y-1][x+1] == 'M') && (array[y+1][x-1] != 'S'))
				continue;
			if ((array[y-1][x+1] == 'S') && (array[y+1][x-1] != 'M'))
				continue;
			sum ++;
		}

	printf ("Sum = %d\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	switch(argv[1][0]) {
		case '1':
			part_1();
			break;
		case '2':
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
