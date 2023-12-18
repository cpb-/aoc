#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define UP    0
#define RIGHT 1
#define DOWN  2
#define LEFT  3


typedef struct {
	long long int x1;
	long long int y1;
} vertex_t;



vertex_t *Vertices = NULL;
int Nb_vertices = 0;



void add_vertex(long long int x1, long long int y1)
{
	//fprintf(stderr, "add_vertex %lld %lld %lld %lld\n", x1, y1, x2, y2);
	Vertices = realloc(Vertices, (Nb_vertices + 1) * sizeof(vertex_t));
	Vertices[Nb_vertices].x1 = x1;
	Vertices[Nb_vertices].y1 = y1;
	Nb_vertices ++;
}



long long int Perimeter = 0;



void read_inputs(int part)
{
	char line[1024];

	long long int x = 0;
	long long int y = 0;

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == 0)
			break;

		if (part == 1) {
			int len;

			if (sscanf(&(line[2]), "%d", &len) != 1)
				break;

			Perimeter += len;

			add_vertex(x, y);

			switch(line[0]) {
				case 'U': y -= len; break;
				case 'R': x += len; break;
				case 'D': y += len; break;
				case 'L': x -= len; break;
			}
		} else {
			int unused;
			long long int len;
			int dir = line[strlen(line) - 2];
			line[strlen(line) - 2] = '\0';

			if (sscanf(&(line[2]), "%d (#%llx", &unused, &len) != 2)
				break;
			Perimeter += len;

			add_vertex(x, y);

			switch(dir) {
				case '3': y -= len; break;
				case '0': x += len; break;
				case '1': y += len; break;
				case '2': x -= len; break;
			}
		}
	}
	fprintf(stderr, "  Loaded: %d vertices (perimeter = %lld)\n", Nb_vertices, Perimeter);
}



void part_func(int part)
{
	read_inputs(part);

	long long int sum = 0;

	// Shoelace algorithm.

	for (int i = 1; i < Nb_vertices; i++)
		sum += Vertices[i].x1 * Vertices[i - 1].y1;
	sum += Vertices[Nb_vertices - 1].x1 * Vertices[0].y1;

	for (int i = 0; i < Nb_vertices; i++)
		sum -= Vertices[i].x1 * Vertices[i + 1].y1;
	sum += Vertices[Nb_vertices - 1].y1 * Vertices[0].x1;

	long long int area = llabs(sum) / 2;

	printf("  Area = %lld\n", Perimeter + area - Perimeter / 2 + 1);
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
			part_func(1);
			break;
		case 2:
			part_func(2);
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
