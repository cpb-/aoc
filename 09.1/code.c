#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int **cells = NULL;
int nb_rows;
int nb_cols;

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

	for (int i = 0; i < nb_rows; i++) {
		cells[i] = calloc(nb_cols, sizeof(int));
		if (cells[i] == NULL) {
			perror("calloc2");
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

	int sum = 0;
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
			sum  += cells[row][col] + 1;
		}

	fprintf(stdout, "Sum of risk points: %d\n", sum);

	return 0;
}


