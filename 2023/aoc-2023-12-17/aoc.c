#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3



char **array = NULL;
int width = 0;
int height = 0;


typedef struct s_cell {
	int x;
	int y;
	int w;
	int sum;
	int done;
	int consec;
	int dir;
	struct s_cell *prev;
} cell_t;



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



cell_t ****Cells = NULL;


cell_t **Queue = NULL;
int Queue_size = 0;



int is_in_queue(cell_t *c)
{
	for (int i = 0; i < Queue_size; i++)
		if (Queue[i] == c)
			return 1;
	return 0;
}



void insert_in_queue(cell_t *c)
{
	int i;


	if (is_in_queue(c))
		return;

	Queue = realloc(Queue, sizeof(cell_t *) * (Queue_size + 1));

	i = 0;
	for (i = 0; i < Queue_size; i++) {
		if (Queue[i]->sum > c->sum)
			break;
	}
	if (i < Queue_size)
		memmove(&Queue[i + 1], &Queue[i], (Queue_size - i) * sizeof(cell_t *));
	Queue[i] = c;
	Queue_size ++;
}



cell_t *extract_first_cell_from_queue(void)
{
	cell_t *r = NULL;

	if (Queue_size >= 1) {
		r = Queue[0];
		if (Queue_size > 1)
			memmove(&Queue[0], &Queue[1], (Queue_size - 1) * sizeof(cell_t *));
		Queue = realloc(Queue, (Queue_size - 1) * sizeof(cell_t *));
		Queue_size --;
	}
	return r;
}



void try_to_move(cell_t *cell, int dir)
{
	int nx = cell->x + (dir == RIGHT) - (dir == LEFT);
	int ny = cell->y + (dir == DOWN)  - (dir == UP);

	if ((nx < 0) || (nx >= width) || (ny < 0) || (ny >= height))
		return;

	cell_t *neighbour;

	if (dir == cell->dir) {
		neighbour = &Cells[ny][nx][dir][cell->consec];
	} else {
		neighbour = &Cells[ny][nx][dir][0];
	}

	if (neighbour->done)
		return;

	int s = cell->sum + neighbour->w;

	if ((neighbour->sum >= 0) && (neighbour->sum <= s))
		return;

	neighbour->sum = s;
	neighbour->prev = cell;

	insert_in_queue(neighbour);

}



void part_func(int min_consec, int max_consec)
{
	load_array();

	Cells = malloc(sizeof(cell_t) * height);
	for (int y = 0; y < height; y ++) {
		Cells[y] = malloc(width * sizeof(cell_t *));
		for (int x = 0; x < width; x++) {
			Cells[y][x] = malloc(4 * sizeof(cell_t *));
			for (int d = 0; d < 4; d ++) {
				Cells[y][x][d] = malloc(max_consec * sizeof(cell_t));
				for (int c = 0; c < max_consec; c++) {
					Cells[y][x][d][c].x = x;
					Cells[y][x][d][c].y = y;
					Cells[y][x][d][c].dir = d;
					Cells[y][x][d][c].w = array[y][x] - '0';
					Cells[y][x][d][c].sum = -1;
					Cells[y][x][d][c].consec = c + 1;
					Cells[y][x][d][c].done = 0;
					Cells[y][x][d][c].prev = NULL;
				}
			}
		}
	}
	for (int d = 0; d < 4; d++) {
		Cells[0][0][d][0].sum = 0;
		Cells[0][0][d][0].done = 1;
	}

	Cells[0][1][RIGHT][0].sum = Cells[0][1][RIGHT][0].w;
	Cells[0][1][RIGHT][0].prev = &Cells[0][0][RIGHT][0];
	insert_in_queue(&(Cells[0][1][RIGHT][0]));

	Cells[1][0][DOWN][0].sum = Cells[1][0][DOWN][0].w;
	Cells[0][1][RIGHT][0].prev = &Cells[0][0][DOWN][0];
	insert_in_queue(&(Cells[1][0][DOWN][0]));

	cell_t *cell;

	while ((cell = extract_first_cell_from_queue()) != NULL) {

		cell->done = 1;

		if ((cell->x == width - 1) && (cell->y == height - 1) && (cell->consec >= min_consec)) {

			printf("   Sum = %d\n", cell->sum);

			break;
		}

		if (cell->consec < max_consec)
			try_to_move(cell, cell->dir);

		if (cell->consec >= min_consec) {
			if ((cell->dir == LEFT) || (cell->dir == RIGHT)) {
				try_to_move(cell, UP);
				try_to_move(cell, DOWN);
			} else {
				try_to_move(cell, LEFT);
				try_to_move(cell, RIGHT);
			}
		}

	}
}



void part_1(void)
{
	part_func(0, 3);
}



void part_2(void)
{
	part_func(4, 10);
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
