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
void display(void);
void display_row(int y, int xmin, int xmax);


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
			continue;
		}
		if (sscanf(buffer, "fold along x=%d", &n) == 1) {
			for (int i = 0; i < nb_points; i ++)
				if (points[i].x > n)
					points[i].x = 2 * n - points[i].x;
			continue;
		}
	}

	display();

	return 0;
}


void add_point(int x, int y)
{
	points = realloc(points, (nb_points + 1) * sizeof(point_t));
	points[nb_points].x = x;
	points[nb_points].y = y;
	nb_points ++;
}


void display(void)
{
	int xmin = points[0].x;
	int ymin = points[0].y;
	int xmax = points[0].x;
	int ymax = points[0].y;
	for (int i = 1; i < nb_points; i ++) {
		if (points[i].x < xmin)
			xmin = points[i].x;
		if (points[i].x > xmax)
			xmax = points[i].x;
		if (points[i].y < ymin)
			ymin = points[i].y;
		if (points[i].y > ymax)
			ymax = points[i].y;
	}
	for (int y = ymin; y < ymax+1; y++) {
		display_row(y, xmin, xmax);
	}
}


void display_row(int y, int xmin, int xmax)
{
	char buffer[4096];

	memset(buffer, ' ', 4095);
	buffer[xmax - xmin + 1] ='\0';
	for (int i = 0; i < nb_points; i++) {
		if (points[i].y == y)
			buffer[points[i].x - xmin] = '#';
	}
	fprintf(stdout, "%s\n", buffer);
}

