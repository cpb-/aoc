#include <assert.h>
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


char **array_backup = NULL;


void backup_array(void)
{
	array_backup = malloc(height * sizeof(char *));
	if (array_backup == NULL) {
		perror("malloc 1");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < height; i++) {
		array_backup[i] = malloc(width + 1);
		if (array_backup[i] == NULL) {
			perror("malloc 2");
			exit(EXIT_FAILURE);
		}
		strcpy(array_backup[i], array[i]);
	}
}


void restore_backup(void)
{
	for (int i = 0; i < height; i++) {
		strcpy(array[i], array_backup[i]);
		free(array_backup[i]);
	}
	free(array_backup);
	array_backup = NULL;
}



int xg, yg;
int dir = 0;

void find_guard(void)
{
	for (yg = 0; yg < height; yg++) {
		for (xg = 0; xg < width; xg++) {
			if ((array[yg][xg] == '<') || (array[yg][xg] == '>') || (array[yg][xg] == 'v') || (array[yg][xg] == '^'))
				break;
		}
		if (xg < width)
			break;
	}
	assert((yg < height) && (xg < width));
}


int iter_guard(void)
{
	if (array[yg][xg] == '^') {
		if (yg == 0) {
			array[yg][xg] = '|';
			return 0;
		}
		if (array[yg - 1][xg] == '#') {
			array[yg][xg] = '>';
		} else {
			array[yg--][xg] = 'X';
			array[yg][xg] = '^';
		}
		return 1;
	}
	if (array[yg][xg] == 'v') {
		if (yg == height - 1) {
			array[yg][xg] = 'X';
			return 0;
		}
		if (array[yg + 1][xg] == '#') {
			array[yg][xg] = '<';
		} else {
			array[yg++][xg] = 'X';
			array[yg][xg] = 'v';
		}
		return 1;
	}
	if (array[yg][xg] == '<') {
		if (xg == 0) {
			array[yg][xg] = 'X';
			return 0;
		}
		if (array[yg][xg - 1] == '#') {
			array[yg][xg] = '^';
		} else {
			array[yg][xg--] = 'X';
			array[yg][xg] = '<';
		}
		return 1;
	}
	if (array[yg][xg] == '>') {
		if (xg == width - 1) {
			array[yg][xg] = 'X';
			return 0;
		}
		if (array[yg][xg + 1] == '#') {
			array[yg][xg] = 'v';
		} else {
			array[yg][xg++] = 'X';
			array[yg][xg] = '>';
		}
		return 1;
	}
	assert(0);
}


int count_X(void)
{
	int sum = 0;
	for (int y = 0; y < height; y++)
		for (int x = 0; x < width; x++)
			if (array[y][x] == 'X')
				sum ++;
	return sum;
}


void part_1(void)
{
	load_array();
	find_guard();

	while (iter_guard())
		;

	printf("Sum = %d\n", count_X());
}



int obstructions = 0;



int contains(int d, int cell)
{
	return (((cell - '0') | (1<<(d - 1))) == (cell - '0'));
}



int add(int d, int cell)
{
	return '0' + ((cell - '0') | (1 << (d - 1)));
}



int next_dir(void)
{
	return 1 + (dir % 4);
}

int iter_with_obstr(void)
{
	if (array[yg][xg] == '.') {
		array[yg][xg] = '0' + (1 << (dir - 1));
	} else if (contains(dir, array[yg][xg])) {
		obstructions ++;
		return 0;
	} else {
		array[yg][xg] = add(dir, array[yg][xg]);
	}

	if (((dir == 1) && (yg == 0))
	 || ((dir == 2) && (xg == width - 1))
	 || ((dir == 3) && (yg == height - 1))
	 || ((dir == 4) && (xg == 0)))
		return 0;

	if (((dir == 1) && (array[yg - 1][xg] == '#'))
	 || ((dir == 2) && (array[yg][xg + 1] == '#'))
	 || ((dir == 3) && (array[yg + 1][xg] == '#'))
	 || ((dir == 4) && (array[yg][xg - 1] == '#'))) {
		dir = next_dir();
		return 1;
	}

	if (dir == 1)
		yg --;
	if (dir == 2)
		xg ++;
	if (dir == 3)
		yg ++;
	if (dir == 4)
		xg --;

	return 1;
}


void part_2(void)
{
	load_array();
	backup_array();

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y ++) {
			restore_backup();
			backup_array();
			if (array[y][x] == '.')
				array[y][x] = '#';
			else
				continue;

			find_guard();

			if (array[yg][xg] == '^')
				dir = 1;
			else if (array[yg][xg] == '>')
				dir = 2;
			else if (array[yg][xg] == 'v')
				dir = 3;
			else
				dir = 4;

			array[yg][xg] = '.';

			while (iter_with_obstr())
				;
		}
	}

	printf("Sum = %d\n", obstructions);

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
