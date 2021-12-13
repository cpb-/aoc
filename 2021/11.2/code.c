#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NB_COLS 10
#define NB_ROWS 10
#define NB_STEPS 100

int cells[NB_ROWS][NB_COLS];
int flashed[NB_ROWS][NB_COLS];

int flashes = 0;

void compute_step(void);
void reset_flashed(void);
void increase_energy_levels(void);
int check_for_flash(void);


int main(int argc, char *argv[])
{
	char buffer[4096];

	int row = 0;
	int col = 0;
	while ((fgets(buffer, 4095, stdin) != NULL) && (row < NB_ROWS)) {
		for (col = 0; col < NB_COLS; col ++)
			cells[row][col] = buffer[col] - '0';
		row ++;
	}

	int step = 0;

	do {
		step ++;
		flashes = 0;
		compute_step();
		fprintf(stderr, "step = %d, flashes = %d\n", step, flashes);

	} while (flashes < NB_COLS * NB_ROWS);


	return 0;
}



void compute_step(void)
{
	reset_flashed();
	increase_energy_levels();
	while (check_for_flash())
		;
}



void reset_flashed(void)
{
	for (int row = 0; row < NB_ROWS; row ++)
		for (int col = 0; col < NB_COLS; col++)
			flashed[row][col] = 0;
}



void increase_energy_levels(void)
{
	for (int row = 0; row < NB_ROWS; row ++)
		for (int col = 0; col < NB_COLS; col++)
			cells[row][col] ++;
}



int check_for_flash(void)
{
	int ret = 0;

	for (int row = 0; row < NB_ROWS; row ++) {
		for (int col = 0; col < NB_COLS; col++) {
			if ((cells[row][col] <= 9) || (flashed[row][col]))
				continue;
			cells[row][col] = 0;
			flashed[row][col] = 1;
			flashes ++;
			if (row > 0) {
				if (col > 0) {
					if (! flashed[row-1][col-1])
						cells[row-1][col-1] ++;
				}
				if (! flashed[row-1][col])
					cells[row-1][col] ++;
				if (col < NB_COLS - 1) {
					if (! flashed[row-1][col+1])
						cells[row-1][col+1] ++;
				}
			}
			if (col > 0) {
				if (! flashed[row][col-1])
					cells[row][col-1] ++;
			}
			if (col < NB_COLS - 1) {
				if (! flashed[row][col+1])
					cells[row][col+1] ++;
			}
			if (row < NB_ROWS - 1) {
				if (col > 0) {
					if (! flashed[row+1][col-1])
						cells[row+1][col-1] ++;
				}
				if (! flashed[row+1][col])
					cells[row+1][col] ++;
				if (col < NB_COLS - 1) {
					if (! flashed[row+1][col+1])
						cells[row+1][col+1] ++;
				}
			}
			ret = 1;
		}
	}
	return ret;
}

