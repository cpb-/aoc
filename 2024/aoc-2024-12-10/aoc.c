#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
int width = 0;
int height = 0;


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



char **array_backup = NULL;


void backup_array(void)
{
	array_backup = malloc(height * sizeof(char *));
	if (array_backup == NULL) {
		perror("malloc 1");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < height; i++) {
		array_backup[i] = malloc(width + 1);
		if (array_backup[i] == NULL) {
			perror("malloc 2");
			exit(EXIT_FAILURE);
		}
		strcpy(array_backup[i], array[i]);
	}
}


void clear_backup_array(void)
{
	for (int i = 0; i < height; i++)
		memset(array_backup[i], 0, width + 1);
}



long long int count_score(int y, int x)
{
	long long int score = 0;

	if (array[y][x] == '9') {
		if (array_backup[y][x] != 'X') {
			array_backup[y][x] = 'X';
			score ++;
		}
		return score;
	}
	if ((y > 0) && (array[y - 1][x] == array[y][x] + 1)) {
		score += count_score(y - 1, x);
	}
	if ((y < height - 1) && (array[y + 1][x] == array[y][x] + 1)) {
		score += count_score(y  + 1, x);
	}
	if ((x > 0) && (array[y][x - 1] == array[y][x] + 1)) {
		score += count_score(y, x - 1);
	}
	if ((x < width - 1) && (array[y][x + 1] == array[y][x] + 1)) {
		score += count_score(y, x + 1);
	}
	return score;
}


long long int count_score_2(int y, int x)
{
	long long int score = 0;

	if (array[y][x] == '9') {
		score ++;
		return score;
	}
	if ((y > 0) && (array[y - 1][x] == array[y][x] + 1)) {
		score += count_score_2(y - 1, x);
	}
	if ((y < height - 1) && (array[y + 1][x] == array[y][x] + 1)) {
		score += count_score_2(y  + 1, x);
	}
	if ((x > 0) && (array[y][x - 1] == array[y][x] + 1)) {
		score += count_score_2(y, x - 1);
	}
	if ((x < width - 1) && (array[y][x + 1] == array[y][x] + 1)) {
		score += count_score_2(y, x + 1);
	}
	return score;
}


void part_1(void)
{
	long long int sum = 0;

	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < width; x++) {
			if (array[y][x] != '0')
				continue;
			clear_backup_array();
			sum += count_score(y, x);
		}
	}
	printf("Sum = %lld\n", sum);
}


void part_2(void)
{
	long long int sum = 0;

	for (int y = 0; y < height; y ++) {
		for (int x = 0; x < width; x++) {
			if (array[y][x] != '0')
				continue;
			sum += count_score_2(y, x);
		}
	}
	printf("Sum = %lld\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_array();
	backup_array();

	switch(argv[1][0]) {
		case '1':
			part_1();
			break;
		case '2':
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
