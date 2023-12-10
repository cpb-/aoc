#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
int width = 0;
int height = 0;

char **array_2 = NULL;



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
	fprintf(stderr, "width = %d, height = %d\n", width, height);

	array_2 = calloc(height, sizeof(char *));
	for (int j = 0; j < height; j++)
		array_2[j] = malloc(width);

	return 0;
}



void clear_array_2(void)
{
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i ++)
			array_2[j][i] = '.';
}



void display_array_2(void)
{
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i ++) {
			fprintf(stderr, "%c", array_2[j][i]);
		}
		fprintf(stderr, "\n");
	}
}



#define UP     0
#define DOWN   1
#define LEFT   2
#define RIGHT  3


int count_steps_from_dir(int x, int y, int dir)
{
	int s;

	array_2[y][x] = array[y][x];

	if ((dir == UP) && (y > 0)) {
		if (array[y - 1][x] == 'S')
			return 1;
		if (array[y - 1][x] == '|') {
			s = count_steps_from_dir(x, y - 1, UP);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y - 1][x] == 'F') {
			s = count_steps_from_dir(x, y - 1, RIGHT);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y - 1][x] == '7')  {
			s = count_steps_from_dir(x, y - 1, LEFT);
			if (s < 0)
				return -1;
			return s + 1;
		}
	}
	if ((dir == DOWN) && (y < height - 1)) {
		if (array[y + 1][x] == 'S')
			return 1;
		if (array[y + 1][x] == '|') {
			s = count_steps_from_dir(x, y + 1, DOWN);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y + 1][x] == 'L') {
			s = count_steps_from_dir(x, y + 1, RIGHT);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y + 1][x] == 'J') {
			s = count_steps_from_dir(x, y + 1, LEFT);
			if (s < 0)
				return -1;
			return s + 1;
		}
	}
	if ((dir == RIGHT) && (x < width - 1)) {
		if (array[y][x + 1] == 'S')
			return 1;
		if (array[y][x + 1] == '-') {
			s = count_steps_from_dir(x + 1, y, RIGHT);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y][x + 1] == 'J') {
			s = count_steps_from_dir(x + 1, y, UP);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y][x + 1] == '7') {
			s = count_steps_from_dir(x + 1, y, DOWN);
			if (s < 0)
				return -1;
			return s + 1;
		}
	}
	if ((dir == LEFT) && (x > 0)) {
		if (array[y][x - 1] == 'S')
			return 1;
		if (array[y][x - 1] == '-') {
			s = count_steps_from_dir(x - 1, y, LEFT);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y][x - 1] == 'L') {
			s = count_steps_from_dir(x - 1, y, UP);
			if (s < 0)
				return -1;
			return s + 1;
		}
		if (array[y][x - 1] == 'F') {
			s = count_steps_from_dir(x - 1, y, DOWN);
			if (s < 0)
				return -1;
			return s + 1;
		}
	}
	return -1;
}



int count_steps_from(int xs, int ys)
{
	int s;

	if (xs > 0) {
		if (array[ys][xs - 1] == '-') {
			clear_array_2();
			s = count_steps_from_dir(xs - 1, ys, LEFT);
			if (s > 0)
				return s + 1;
		}
		if (array[ys][xs - 1] == 'L') {
			clear_array_2();
			s = count_steps_from_dir(xs - 1, ys, UP);
			if (s > 0)
				return s + 1;
		}
		if (array[ys][xs - 1] == 'F') {
			clear_array_2();
			s = count_steps_from_dir(xs - 1, ys, DOWN);
			if (s > 0)
				return s + 1;
		}
	}
	if (xs < width - 1) {
		if (array[ys][xs + 1] == '-') {
			clear_array_2();
			s = count_steps_from_dir(xs + 1, ys, RIGHT);
			if (s > 0)
				return s + 1;
		}
		if (array[ys][xs + 1] == 'J') {
			clear_array_2();
			s = count_steps_from_dir(xs + 1, ys, UP);
			if (s > 0)
				return s + 1;
		}
		if (array[ys][xs + 1] == '7') {
			clear_array_2();
			s = count_steps_from_dir(xs + 1, ys, DOWN);
			if (s > 0)
				return s + 1;
		}
	}
	if (ys > 0) {
		if (array[ys - 1][xs] == '|') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys - 1, UP);
			if (s > 0)
				return s + 1;
		}
		if (array[ys - 1][xs] == 'F') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys - 1, RIGHT);
			if (s > 0)
				return s + 1;
		}
		if (array[ys - 1][xs] == '7') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys - 1, LEFT);
			if (s > 0)
				return s + 1;
		}
	}
	if (ys < height - 1) {
		if (array[ys + 1][xs] == '|') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys + 1, DOWN);
			if (s > 0)
				return s + 1;
		}
		if (array[ys + 1][xs] == 'L') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys + 1, RIGHT);
			if (s > 0)
				return s + 1;
		}
		if (array[ys + 1][xs] == 'J') {
			clear_array_2();
			s = count_steps_from_dir(xs, ys + 1, LEFT);
			if (s > 0)
				return s + 1;
		}
	}

	exit(EXIT_FAILURE);
}



void part_1(void)
{
	int xs;
	int ys;
	int length;

	load_array();

	for (xs = 0; xs < width; xs ++) {
		for (ys = 0; ys < height; ys ++) {
			if (array[ys][xs] == 'S')
				break;
		}
		if (ys < height)
			break;
	}
	fprintf(stderr, "   Starting at Xs=%d, Ys=%d\n", xs, ys);

	length = count_steps_from(xs, ys);

	printf("   Length = %d\n", length);
	printf("   Max distance = %d\n", length / 2);
}



void part_2(void)
{
	int xs;
	int ys;

	load_array();

	for (xs = 0; xs < width; xs ++) {
		for (ys = 0; ys < height; ys ++) {
			if (array[ys][xs] == 'S')
				break;
		}
		if (ys < height)
			break;
	}

	clear_array_2();
	count_steps_from(xs, ys);
	array_2[ys][xs] = 'S';
//	display_array_2();
//	fprintf(stderr, "\n\n");

	for (int y = 0; y < height; y ++) {
		int c = ' ';
		for (int x = 0; x < width; x ++) {
			if (array_2[y][x] == '.') {
				array_2[y][x] = c;
			} else if (array_2[y][x] == 'L') {
				while (x < width) {
					if (array_2[y][x] == 'J')
						break;
					if (array_2[y][x] == '7') {
						c = (c == ' ' ? '*' : ' ');
						break;
					}
					x ++;
				}
			} else if (array_2[y][x] == 'F') {
				while (x < width) {
					if (array_2[y][x] == '7')
						break;
					if (array_2[y][x] == 'J') {
						c = (c == ' ' ? '*' : ' ');
						break;
					}
					x ++;
				}
			} else if (array_2[y][x] == '|') {
				c = (c == ' ' ? '*' : ' ');
			}
		}
	}

//	display_array_2();

	int sum = 0;
	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < width; x ++) {
			if (array_2[y][x] == '*')
				sum ++;
		}
	}
	printf ("  Sum = %d\n", sum);

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
