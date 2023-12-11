#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
int width = 0;
int height = 0;

int   *empty_cols = NULL;
int nb_empty_cols = 0;

int   *empty_rows = NULL;
int nb_empty_rows = 0;



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



void find_empty_cols_and_lines(void)
{
	for (int row = 0; row < height; row++) {
		int col;
		for (col = 0; col < width; col ++) {
			if (array[row][col] != '.')
				break;
		}
		if (col == width) {
			empty_rows = realloc(empty_rows, (nb_empty_rows + 1) * sizeof(int));
			empty_rows[nb_empty_rows] = row;
			nb_empty_rows ++;
		}
	}

	for (int col = 0; col < width; col++) {
		int row;
		for (row = 0; row < height; row++) {
			if (array[row][col] != '.')
				break;
		}
		if (row == height) {
			empty_cols = realloc(empty_cols, (nb_empty_cols + 1) * sizeof(int));
			empty_cols[nb_empty_cols] = col;
			nb_empty_cols ++;
		}
	}
}



void display_empty(void)
{
	fprintf(stderr, "Empty cols: ");
	for (int i = 0; i < nb_empty_cols; i ++)
		fprintf(stderr, "%d ", empty_cols[i]);
	fprintf(stderr, "\n");

	fprintf(stderr, "Empty rows: ");
	for (int i = 0; i < nb_empty_rows; i ++)
		fprintf(stderr, "%d ", empty_rows[i]);
	fprintf(stderr, "\n");
}



void display_array(void)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			fprintf(stderr, "%c", array[y][x]);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}



void part(int mult)
{
	long long int sum = 0;

	load_array();
	find_empty_cols_and_lines();
//	display_empty();

	for (int x1 = 0; x1 < width; x1++)
		for (int y1 = 0; y1 < height; y1++) {
			if (array[y1][x1] == '.')
				continue;
			for (int x2 = 0; x2 < width; x2++)
				for (int y2 = 0; y2 < height; y2++) {
					if (array[y2][x2] == '.')
						continue;
					if ((x1 != x2) || (y1 != y2)) {
						long long int dist = abs(x2 - x1) + abs(y2 - y1);
						for (int i = 0; i < nb_empty_cols; i++)
							if (((x1 < empty_cols[i]) && (x2 > empty_cols[i]))
							 || ((x1 > empty_cols[i]) && (x2 < empty_cols[i])))
								dist += mult - 1;
						for (int i = 0; i < nb_empty_rows; i++)
							if (((y1 < empty_rows[i]) && (y2 > empty_rows[i]))
							 || ((y1 > empty_rows[i]) && (y2 < empty_rows[i])))
								dist += mult - 1;
						sum += dist;
					}
				}
		}
	printf("  Sum = %lld\n", sum / 2);
}



int main(int argc, char *argv[])
{
	int mult;

	if ((argc < 2) || (sscanf(argv[1], "%d", &mult) != 1)) {
		fprintf(stderr, "usage: %s <multiply> \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	part(mult);
	return EXIT_SUCCESS;
}
