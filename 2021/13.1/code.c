#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct s_point {
	int x;
	int y;
} point_t;

point_t *points = NULL;
int nb_points;


void add_point(int x, int y);
void count_visible_points(void);


int main(int argc, char *argv[])
{
	char buffer[4096];

	while (fgets(buffer, 4095, stdin) != NULL) {
		int x, y;
		if (sscanf(buffer, "%d,%d", &x, &y) != 2)
			break;
		add_point(x, y);
	}

	while(fgets(buffer, 4095, stdin) != NULL) {
		int n;
		if (sscanf(buffer, "fold along y=%d", &n) == 1) {
			for (int i = 0; i < nb_points; i ++)
				if (points[i].y > n)
					points[i].y = 2 * n - points[i].y;
			count_visible_points();
			break;
		}
		if (sscanf(buffer, "fold along x=%d", &n) == 1) {
			for (int i = 0; i < nb_points; i ++)
				if (points[i].x > n)
					points[i].x = 2 * n - points[i].x;
			count_visible_points();
			break;
		}
	}

	return 0;
}


void add_point(int x, int y)
{
	points = realloc(points, (nb_points + 1) * sizeof(point_t));
	points[nb_points].x = x;
	points[nb_points].y = y;
	nb_points ++;
}


void count_visible_points(void)
{
	int nb_visible_points = 0;

	for (int i = 0; i < nb_points; i++) {
		int j;
		for (j = 0; j < i; j ++)
			if ((points[i].x == points[j].x) && (points[i].y == points[j].y))
				break;
		if (j == i)
			nb_visible_points++;
	}

	fprintf(stdout, "Nb visible points = %d\n", nb_visible_points);

}

