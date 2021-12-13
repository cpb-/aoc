#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int **cells = NULL;
int **size = NULL;
int **used = NULL;
int nb_rows;
int nb_cols;

int compute_basin_size(int row, int col);

int main(int argc, char *argv[])
{
	char buffer[1024];

	if ((argc != 3)
	 || (sscanf(argv[1], "%d", &nb_cols) != 1)
	 || (sscanf(argv[2], "%d", &nb_rows) != 1)) {
		fprintf(stderr, "usage: %s <nb_cols> <nb_rows>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	cells = calloc(nb_rows, sizeof(int *));
	if (cells == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	size = calloc(nb_rows, sizeof(int *));
	if (size == NULL) {
		perror("calloc1");
		exit(EXIT_FAILURE);
	}
	used = calloc(nb_rows, sizeof(int *));
	if (used == NULL) {
		perror("calloc2");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < nb_rows; i++) {
		cells[i] = calloc(nb_cols, sizeof(int));
		if (cells[i] == NULL) {
			perror("calloc3");
			exit(EXIT_FAILURE);
		}
		size[i] = calloc(nb_cols, sizeof(int));
		if (size[i] == NULL) {
			perror("calloc4");
			exit(EXIT_FAILURE);
		}
		used[i] = calloc(nb_cols, sizeof(int));
		if (used[i] == NULL) {
			perror("calloc5");
			exit(EXIT_FAILURE);
		}
	}

	int row = 0;
	int col = 0;
	while ((fgets(buffer, 1023, stdin) != NULL) && (row < nb_rows)) {
		for (col = 0; col < nb_cols; col++)
			cells[row][col] = buffer[col] - '0';
		row++;
	}

	for (row = 0; row < nb_rows; row++)
		for (col = 0; col < nb_cols; col++) {
			if ((row > 0) && (cells[row][col] >= cells[row - 1][col]))
				continue;
			if ((row < nb_rows - 1) && (cells[row][col] >= cells[row + 1][col]))
				continue;
			if ((col > 0) && (cells[row][col] >= cells[row][col - 1]))
				continue;
			if ((col < nb_cols - 1) && (cells[row][col] >= cells[row][col + 1]))
				continue;
			for (int i=0; i< nb_rows; i ++)
				for (int j = 0; j < nb_cols; j++)
					used[i][j] = 0;
			size[row][col] = compute_basin_size(row, col);
			fprintf(stderr, "cell (%d, %d) = %d, size(%d, %d) = %d\n",
				row, col, cells[row][col],
				row, col, size[row][col]);
		}

	int first = -1, second = -1, third = -1;
	for (row = 0; row < nb_rows; row++) {
		for (col = 0; col < nb_cols; col++) {
			if (size[row][col] > first) {
				third = second;
				second = first;
				first = size[row][col];
				continue;
			}
			if (size[row][col] > second) {
				third = second;
				second = size[row][col];
				continue;
			}
			if (size[row][col] > third) {
				third = size[row][col];
				continue;
			}
		}
	}

	fprintf(stdout, "Basin sizes: %d, %d, %d\n", first, second, third);
	fprintf(stdout, "Product of basin size: %d\n", first * second * third);

	return 0;
}

int compute_basin_size(int row, int col)
{
	int size = 1;
	used[row][col] = 1;
	if ((row > 0) && (! used[row - 1][col]) && (cells[row - 1][col] != 9))
		size += compute_basin_size(row - 1, col);
	if ((row < nb_rows - 1) && (! used[row + 1][col]) && (cells[row + 1][col] != 9))
		size += compute_basin_size(row + 1, col);
	if ((col > 0) && (! used[row][col - 1]) && (cells[row][col - 1] != 9))
		size += compute_basin_size(row, col - 1);
	if ((col < nb_cols - 1) && (! used[row][col + 1]) && (cells[row][col + 1] != 9))
		size += compute_basin_size(row, col + 1);

	return size;
}
