#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
size_t width = 0;
size_t height = 0;


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


typedef struct s_state {
	size_t x;
	size_t y;
	size_t d; // direction 0N, 1E, 2S, 3W
	long long int s;
} state_t;


state_t * states = NULL;
int nb_states = 0;

long long int ***scores;

long long int best_score = -1;


void explore(size_t x, size_t y, size_t d, long long int s)
{
	states = realloc(states, (nb_states + 1) * sizeof(state_t));
	states[nb_states].x = x;
	states[nb_states].y = y;
	states[nb_states].d = d;
	states[nb_states].s = s;
	nb_states ++;

	while (nb_states > 0) {
		// find the best (shortest) distance node.
		int i_best = 0;
		for (int i = 1; i < nb_states; i++) {
			if ((states[i].s < states[i_best].s) && (states[i].s >= 0))
				i_best = i;
		}

		// extract it.
		state_t best;
		memcpy(&best, &states[i_best], sizeof(state_t));

		if (i_best < nb_states - 1)
			memcpy(&states[i_best], &states[nb_states - 1], sizeof(state_t));
		nb_states --;

		best_score = best.s;

		// End found?
		if (array[best.y][best.x] == 'E')
			break;

		if ((best.s >= scores[best.y][best.x][best.d]) && (scores[best.y][best.x][best.d] >= 0))
			continue;
		scores[best.y][best.x][best.d] = best.s;

		states = realloc(states, (nb_states + 1) * sizeof(state_t));
		states[nb_states].x = best.x;
		states[nb_states].y = best.y;
		states[nb_states].d = (best.d + 1) % 4;
		states[nb_states].s = best.s + 1000;
		nb_states ++;

		states = realloc(states, (nb_states + 1) * sizeof(state_t));
		states[nb_states].x = best.x;
		states[nb_states].y = best.y;
		states[nb_states].d = (best.d + 3) % 4;
		states[nb_states].s = best.s + 1000;
		nb_states ++;

		size_t x = best.x;
		size_t y = best.y;
		switch (best.d) {
			case 0: y--; break;
			case 1: x++; break;
			case 2: y++; break;
			case 3: x--; break;
		}
		if (array[y][x] != '#') {
			states = realloc(states, (nb_states + 1) * sizeof(state_t));
			states[nb_states].x = x;
			states[nb_states].y = y;
			states[nb_states].d = best.d;
			states[nb_states].s = best.s + 1;
			nb_states ++;
		}
	}
	printf("Score = %lld\n", best_score);
}



void find_start(size_t *xs, size_t *ys)
{
	scores = malloc(height * sizeof (long long int **));

	for (size_t y = 0; y < height; y++) {

		scores[y] = malloc(width * sizeof (long long int *));

		for (size_t x = 0; x < width; x++) {

			scores[y][x] = malloc(4 * sizeof(long long int));

			if (array[y][x] == 'S') {
				*xs = x;
				*ys = y;
			}
			scores[y][x][0] = -1;
			scores[y][x][1] = -1;
			scores[y][x][2] = -1;
			scores[y][x][3] = -1;
		}
	}
}


int ** seats = NULL;

int fill_seats(int x, int y, int d, int s)
{
	if ((array[y][x] == 'E') && (s == best_score)) {
		seats[y][x] ++;
		return 1;
	}
	if (best_score <= s)
		return 0;
	if ((scores[y][x][d] < s) && (scores[y][x][d] > 0))
		return 0;
	scores[y][x][d] = s;
	size_t xn = x;
	size_t yn = y;
	switch (d) {
		case 0: yn--; break;
		case 1: xn++; break;
		case 2: yn++; break;
		case 3: xn--; break;
	}
	int good_path = 0;
	if ((array[yn][xn] != '#') && (fill_seats(xn, yn, d, s + 1)))
			good_path = 1;
	if (fill_seats(x, y, (d + 1) % 4, s + 1000))
		good_path = 1;
	if (fill_seats(x, y, (d + 3) % 4, s + 1000))
		good_path = 1;
	if (good_path)
		seats[y][x] ++;
	return good_path;
}


void part_1(void)
{
	size_t xs, ys;

	find_start(&xs, &ys);
	explore(xs, ys, 1, 0);
}


void part_2(void)
{
	size_t xs, ys;

	find_start(&xs, &ys);
	explore(xs, ys, 1, 0);

	seats = malloc(height * sizeof(int *));
	for (size_t j = 0; j < height; j++) {
		seats[j] = malloc(width * sizeof(int));
		for (size_t i = 0; i < width; i++)
			seats[j][i] = 0;
	}

	fill_seats(xs, ys, 1, 0);

	int count = 0;
	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++)
			if (seats[j][i] != 0)
				count ++;
	}
	printf ("Nb seats: %d\n", count);
}


int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_array();

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
