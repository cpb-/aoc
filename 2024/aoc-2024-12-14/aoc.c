#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	long long int px;
	long long int py;
	long long int vx;
	long long int vy;
} robot_t;

robot_t *robs = NULL;
int nb_robs = 0;

long long int width = 11;
long long int height = 7;


void load_robots(void)
{
	char buffer[128];

	while (fgets(buffer, 128, stdin) != NULL) {
		robs = realloc(robs, (nb_robs + 1) * sizeof(robot_t));
		robot_t *r = &(robs[nb_robs]);

		if (sscanf(buffer, "p=%lld,%lld v=%lld,%lld", &(r->px), &(r->py), &(r->vx), &(r->vy)) != 4)
			break;
		nb_robs ++;
		if ((width == 11) && (r->px > 11)) {
			width = 101;
			height = 103;
		}
	}
	printf("Nb robots: %d\n", nb_robs);
}



void run_robots(long long int steps)
{
	for (int i = 0; i < nb_robs; i++) {
		robs[i].px += robs[i].vx * steps;
		robs[i].py += robs[i].vy * steps;
		while(robs[i].px >= width)
			robs[i].px -= width;
		while(robs[i].py >= height)
			robs[i].py -= height;
		while(robs[i].px < 0)
			robs[i].px += width;
		while(robs[i].py < 0)
			robs[i].py += height;
	}
}



int count_robots(long long int xmin, long long int xmax, long long int ymin, long long int ymax)
{
	int nb = 0;
	for (int i = 0; i < nb_robs; i++)
		if ((robs[i].px >= xmin) && (robs[i].px <= xmax) && (robs[i].py >= ymin) && (robs[i].py <= ymax))
			nb ++;
	return nb;
}



void part_1(void)
{
	run_robots(100);

	int factor =
		  count_robots(0,            width / 2 - 1,    0,                 height / 2 - 1)
		* count_robots(0,            width / 2 - 1,    height / 2 + 1,    height - 1)
		* count_robots(width / 2 + 1, width - 1,       0,                 height / 2 - 1)
		* count_robots(width / 2 + 1, width - 1,       height / 2 + 1,    height - 1);
	printf("Safety factor = %d\n", factor);
}


char frame[103][101];

int fill_frame(void)
{
	int overlap = 0;

	for (int y = 0; y < 103; y++)
		for (int x = 0; x < 101; x++)
			frame[y][x] = '.';
	for (int r = 0; r < nb_robs; r++) {
		if (frame[robs[r].py][robs[r].px] == '.')
			frame[robs[r].py][robs[r].px] = '1';
		else {
			overlap ++;
			frame[robs[r].py][robs[r].px]++;
		}
	}
	return overlap;
}

void display_frame(void)
{
	for (int y = 0; y < 103; y++) {
		for (int x = 0; x < 101; x++)
			printf("%c", frame[y][x]);
		printf("\n");
	}
	printf("\n");
}

void part_2(void)
{
	int steps = 0;

	for (;;) {
		while (fill_frame() != 0) {
			run_robots(1);
			steps ++;
		}
		printf ("%d:\n", steps);
		display_frame();
		sleep(1);
	}
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_robots();

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
