#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 16384


#define COORD_MAX 50

unsigned char cube[COORD_MAX * 2 + 1][COORD_MAX * 2 + 1][COORD_MAX * 2 + 1];

void set_cubes(int state, int x1, int x2, int y1, int y2, int z1, int z2);


int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	while(fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		int x1, x2, y1, y2, z1, z2;
		if (sscanf(buffer, "on x=%d..%d,y=%d..%d,z=%d..%d",
			&x1, &x2, &y1, &y2, &z1, &z2) == 6) {
			set_cubes(1, x1, x2, y1, y2, z1, z2);
			continue;
		} else if (sscanf(buffer, "off x=%d..%d,y=%d..%d,z=%d..%d",
			&x1, &x2, &y1, &y2, &z1, &z2) == 6) {
			set_cubes(0, x1, x2, y1, y2, z1, z2);
			continue;
		}
		fprintf(stderr, "Unknown command: %s\n", buffer);
	}

	long long int total = 0;

	for (int x = -COORD_MAX; x < COORD_MAX + 1; x ++) {
		for (int y = -COORD_MAX; y < COORD_MAX + 1; y ++) {
			for (int z = -COORD_MAX; z < COORD_MAX + 1; z ++) {
				total += cube[x + COORD_MAX][y + COORD_MAX][z+COORD_MAX];
			}
		}
	}
	fprintf(stderr, "nb_cubes on: %lld\n", total);

	return 0;
}


void set_cubes(int state, int x1, int x2, int y1, int y2, int z1, int z2)
{
	if (x1 < -COORD_MAX)
		x1 = -COORD_MAX;
	if (x2 > COORD_MAX)
		x2 = COORD_MAX;
	if (y1 < -COORD_MAX)
		y1 = -COORD_MAX;
	if (y2 > COORD_MAX)
		y2 = COORD_MAX;
	if (z1 < -COORD_MAX)
		z1 = -COORD_MAX;
	if (z2 > COORD_MAX)
		z2 = COORD_MAX;
	for (int x = x1; x <= x2; x++)
		for (int y = y1; y <= y2; y++)
			for (int z = z1; z <= z2; z++)
				cube [x + COORD_MAX][y + COORD_MAX][z+COORD_MAX] = state;
}



