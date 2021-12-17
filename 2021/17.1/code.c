#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

int target_xmin, target_xmax, target_ymin, target_ymax;
int ymax;

void fire(int vx, int vy);

#define MIN_VX 1
#define MAX_VX 200
#define MIN_VY 1
#define MAX_VY 200

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	if (fgets(buffer, BUFFER_SIZE - 1, stdin) == NULL) {
		fprintf(stderr, "No input data\n");
		exit(EXIT_FAILURE);
	}

	if (sscanf(buffer, "target area: x=%d..%d, y=%d..%d", &target_xmin, &target_xmax, &target_ymin, &target_ymax) != 4) {
		fprintf(stderr, "unable to parse line: %s\n", buffer);
		exit(EXIT_FAILURE);
	}

	for(int vx = MIN_VX; vx < MAX_VX; vx++) {
		for (int vy = MIN_VY; vy < MAX_VY; vy++) {
			fire(vx, vy);
		}
	}

	fprintf(stdout, "Max reached Y = %d\n", ymax);

	return 0;
}

void fire(int vx, int vy)
{
	int fire_ymax=0;
	int x=0;
	int y=0;

	for (;;) {
		if ((x >= target_xmin) && (x <= target_xmax) && (y >= target_ymin) && (y <= target_ymax))
			break;
		if ((y < target_ymin))
			return;
		x += vx;
		y += vy;
		if (vx > 0) vx --;
		if (vx < 0) vx --;
		vy --;
		if (y > fire_ymax)
			fire_ymax = y;
	}
	if (fire_ymax > ymax) {
		ymax = fire_ymax;
	}
}


