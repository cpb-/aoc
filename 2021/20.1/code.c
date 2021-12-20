#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 16384

#define NB_ALGO_VALUES 512
int algorithm_values[NB_ALGO_VALUES];

#define XMAX 128
#define YMAX 128
#define OFFSET 64
int image[XMAX + OFFSET*2][YMAX + OFFSET*2];
int newimage[XMAX + OFFSET * 2][YMAX + OFFSET*2];

void display();
void one_pass(void);
int count(void);

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	if (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		for (int i = 0; i < NB_ALGO_VALUES; i++) {
			if (buffer[i] == '#')
				algorithm_values[i] = 1;
			else if (buffer[i] == '.')
				algorithm_values[i] = 0;
			else {
				fprintf(stderr, "unexpected end of line in algorithm values\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	fgets(buffer, BUFFER_SIZE - 1, stdin);

	for (int i = 0; i < XMAX + OFFSET*2; i++) {
		for (int j = 0; j < YMAX + OFFSET*2; j++) {
			image[i][j] = 0;
		}
	}

	int y = OFFSET;

	while (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		for (int i = 0; i < XMAX; i++) {
			if (buffer[i] == '#')
				image[OFFSET + i][y] = 1;
			else if (buffer[i] == '.')
				image[OFFSET + i][y] = 0;
			else
				break;
		}
		y ++;
	}
	display();
	one_pass();
	display();
	one_pass();
	display();
	fprintf(stderr, "Nb pixel lit = %d\n", count());

	return 0;
}


void display(void)
{
	for (int i = 0; i < XMAX; i++)
		fprintf(stderr, "=");
	fprintf(stderr, "\n");

	for (int j = 0; j < YMAX  + OFFSET*2; j++) {
		for (int i = 0; i < XMAX + OFFSET*2; i++) {
			fprintf(stderr, "%d", image[i][j]);
		}
		fprintf(stderr, "\n");
	}

	for (int i = 0; i < XMAX; i++)
		fprintf(stderr, "=");
	fprintf(stderr, "\n");

}


int compute(int x, int y);

void one_pass(void)
{

	for (int i = 0; i < XMAX + OFFSET*2; i++)
		for (int j = 0; j < XMAX + OFFSET*2; j++)
			newimage[i][j] = 0;

	for (int j = 0; j < YMAX + OFFSET*2; j++)
		for (int i = 0; i < XMAX + OFFSET*2; i++) {
			newimage[i][j] = algorithm_values[compute(i, j)];
		}

	for (int i = 0; i < XMAX + OFFSET*2; i++)
		for (int j = 0; j < XMAX + OFFSET*2; j++)
			image[i][j] = newimage[i][j];


}



int compute(int x, int y)
{
	int result = 0;

	if (y > 0) {
		result <<= 1;  if (x > 0) result |= image[x-1][y-1]; else result |= image[x][y-1];
		result <<= 1;  result |= image[x][y-1];
		result <<= 1;  if (x < XMAX  + OFFSET*2- 1) result |= image[x+1][y-1]; else result |= image[x][y-1];
	} else {
		result <<= 1;  if (x > 0) result |= image[x-1][y]; else result |= image[x][y];
		result <<= 1;  result |= image[x][y];
		result <<= 1;  if (x < XMAX  + OFFSET*2- 1) result |= image[x+1][y]; else result |= image[x][y];
	}

	result <<= 1;  if (x > 0) result |= image[x-1][y]; else result |= image[x][y];
	result <<= 1;  result |= image[x][y];
	result <<= 1;  if (x < XMAX  + OFFSET*2 - 1) result |= image[x+1][y]; else result |= image[x][y];

	if (y < YMAX  + OFFSET*2 - 1) {
		result <<= 1;  if (x > 0) result |= image[x-1][y+1]; else result |= image[x][y+1];
		result <<= 1;  result |= image[x][y+1];
		result <<= 1;  if (x < XMAX  + OFFSET*2 - 1) result |= image[x+1][y+1]; else  result |= image[x][y+1];
	} else {
		result <<= 1;  if (x > 0) result |= image[x-1][y]; else result |= image[x][y];
		result <<= 1;  result |= image[x][y];
		result <<= 1;  if (x < XMAX  + OFFSET*2 - 1) result |= image[x+1][y]; else  result |= image[x][y];
	}

	return result;
}


int count(void)
{
	int result = 0;
	for (int i = 0; i < XMAX + OFFSET*2; i++)
		for (int j = 0; j < XMAX + OFFSET*2; j++)
			if (image[i][j])
				result ++;
	return result;
}
