#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
int width = 0;
int height = 0;

char *moves = NULL;
int nb_moves = 0;


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
	for (int j = 0; j < height; j ++) {
		for (int i = 0; i < width; i ++)
			printf("%c", array[j][i]);
		printf("\n");
	}
	printf("\n");
}

void load_moves(void)
{
	char buffer[4096];

	while (fgets(buffer, 4096, stdin) != NULL) {
		int l = strlen(buffer);
		buffer[l--] = '\0';
		moves = realloc(moves, (nb_moves + l) + 1);
		strcpy(&moves[nb_moves], buffer);
		nb_moves += l;
	}

	printf("Read %d moves\n", nb_moves);
}



void widen_array(void)
{
	for (int j = 0; j < height; j++) {
		array[j] = realloc(array[j], width * 2);
		for (int i = width - 1; i >= 0; i --) {
			if ((array[j][i] == '.') || (array[j][i] == '#')) {
				array[j][2 * i] = array[j][i];
				array[j][2 * i + 1] = array[j][i];
			} else if (array[j][i] == 'O') {
				array[j][2 * i] = '[';
				array[j][2 * i + 1] = ']';
			} else if (array[j][i] == '@') {
				array[j][2 * i] = '@';
				array[j][2 * i + 1] = '.';
			}
		}
	}
	width = width * 2;
}


void find_robot(int *x, int *y)
{
	for (int j = 0; j < height; j++)
		for (int i = 0; i < width; i++)
			if (array[j][i] == '@') {
				*x = i;
				*y = j;
				return;
			}
}


int move_box(int x, int y, int dx, int dy)
{
	if (array[y + dy][x + dx] == '.') {
		array[y + dy][x + dx] = 'O';
		array[y][x] = '.';
		return 1;
	}
	if (array[y + dy][x + dx] == '#')
		return 0;
	if (move_box(x + dx, y + dy, dx, dy)) {
		array[y + dy][x + dx] = 'O';
		array[y][x] = '.';
		return 1;
	}
}


int move_robot(int x, int y, int dx, int dy)
{
	if (array[y + dy][x + dx] == '.') {
		array[y + dy][x + dx] = '@';
		array[y][x] = '.';
		return 1;
	}
	if (array[y + dy][x + dx] == '#')
		return 0;
	if (move_box(x + dx, y + dy, dx, dy)) {
		array[y + dy][x + dx] = '@';
		array[y][x] = '.';
		return 1;
	}
	return 0;
}


void apply_moves(int x, int y)
{
	for (int i = 0; i < nb_moves; i++) {
		if (moves[i] == '^') {
			if (move_robot(x, y, 0, -1))
				y--;
		} else if (moves[i] == 'v') {
			if (move_robot(x, y, 0, 1))
				y++;
		} else if (moves[i] == '<') {
			if (move_robot(x, y, -1, 0))
				x--;
		} else if (moves[i] == '>') {
			if (move_robot(x, y, 1, 0))
				x++;
		}
	}
}


int can_move_box(int x, int y, int dx, int dy)
{
	if (array[y + dy][x + dx] == '#')
		return 0;
	if (array[y + dy][x + dx] == '.')
		return 1;
	if (array[y + dy][x + dx] == ']')
		return (can_move_box(x + dx, y + dy, dx, dy) && ((dx == 1) || can_move_box(x - 1 + dx, y + dy, dx, dy)));
	if (array[y + dy][x + dx] == '[')
		return (can_move_box(x + dx, y + dy, dx, dy) && ((dx == -1) || can_move_box(x + 1 + dx, y + dy, dx, dy)));
}


void move_box_2(int x, int y, int dx, int dy)
{
	if (array[y + dy][x + dx] == '[') {
		if (dx != -1)
			move_box_2(x + 1 + dx, y + dy, dx, dy);
		move_box_2(x + dx, y + dy, dx, dy);
	} else if (array[y + dy][x + dx] == ']') {
		if (dx != 1)
			move_box_2(x - 1 + dx, y + dy, dx, dy);
		move_box_2(x + dx, y + dy, dx, dy);
	} else if (array[y + dy][x + dx] == '#') {
		move_box_2(x + dx, y + dy, dx, dy);
	}
	array[y + dy][x + dx] = array[y][x];
	array[y][x] = '.';
}



void apply_moves_2(int x, int y)
{
	for (int i = 0; i < nb_moves; i++) {
		switch (moves[i]) {
			case '^':
				switch (array[y - 1][x]) {
					case '.': array[y - 1][x] = '@'; array[y][x] = '.'; y--; break;
					case '[':
						if ((can_move_box(x, y - 1, 0, -1)) && (can_move_box(x + 1, y - 1, 0, -1))) {
							move_box_2(x, y - 1, 0, -1);
							move_box_2(x + 1, y - 1, 0, -1);
							array[y - 1][x] = '@';
							array[y][x] = '.';
							y --;
						}
						break;
					case ']':
						if ((can_move_box(x, y - 1, 0, -1)) && (can_move_box(x - 1, y - 1, 0, -1))) {
							move_box_2(x, y - 1, 0, -1);
							move_box_2(x - 1, y - 1, 0, -1);
							array[y - 1][x] = '@';
							array[y][x] = '.';
							y--;
						}
						break;
					default: break;
				}
				break;
			case 'v':
				switch (array[y + 1][x]) {
					case '.': array[y + 1][x] = '@'; array[y][x] = '.'; y++; break;
					case '[':
						if ((can_move_box(x, y + 1, 0, 1)) && (can_move_box(x + 1, y + 1, 0, 1))) {
							move_box_2(x, y + 1, 0, 1);
							move_box_2(x + 1, y + 1, 0, 1);
							array[y + 1][x] = '@';
							array[y][x] = '.';
							y ++;
						}
						break;
					case ']':
						if ((can_move_box(x, y + 1, 0, 1)) && (can_move_box(x - 1, y + 1, 0, 1))) {
							move_box_2(x, y + 1, 0, 1);
							move_box_2(x - 1, y + 1, 0, 1);
							array[y + 1][x] = '@';
							array[y][x] = '.';
							y++;
						}
						break;
					default: break;
				}
				break;
			case '<':
				switch (array[y][x - 1]) {
					case '.': array[y][x - 1] = '@'; array[y][x] = '.'; x--; break;
					case ']':
						if (can_move_box(x - 2, y, -1, 0)) {
							move_box_2(x - 2, y, -1, 0);
							move_box_2(x - 1, y, -1, 0);
							array[y][x - 1] = '@';
							array[y][x] = '.';
							x--;
						}
						break;
					default: break;
				}
				break;
			case '>':
				switch (array[y][x + 1]) {
					case '.': array[y][x + 1] = '@'; array[y][x] = '.'; x++; break;
					case '[':
						if (can_move_box(x + 2, y, 1, 0)) {
							move_box_2(x + 2, y, 1, 0);
							move_box_2(x + 1, y, 1, 0);
							array[y][x + 1] = '@';
							array[y][x] = '.';
							x++;
						}
						break;
					default: break;
				}
				break;
		}
	}
}


void boxes_coord(void)
{
	long long int sum = 0;

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			if ((array[j][i] == 'O') || (array[j][i] == '['))
				sum += j * 100 + i;

	printf("Sum = %lld\n", sum);

}

void part_1(void)
{
	int x, y;
	find_robot(&x, &y);
	apply_moves(x, y);
	boxes_coord();
}


void part_2(void)
{
	int x, y;

	widen_array();
	find_robot(&x, &y);
	apply_moves_2(x, y);
	display_array();
	boxes_coord();
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_array();
	load_moves();

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
