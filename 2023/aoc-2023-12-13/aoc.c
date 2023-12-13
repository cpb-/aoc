#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char ***Patterns = NULL;
int *Width  = NULL;
int *Height = NULL;
int  Nb_patterns = 0;



int load_patterns(void)
{
	char line[1024];

	for (;;) {
		if (fgets(line, 1024, stdin) == NULL)
			break;
		line[strlen(line) - 1] = '\0';
		if ((line[0] == '\0') || (Patterns == NULL)) {
			Patterns = realloc(Patterns, (Nb_patterns + 1) * sizeof(char **));
			Width = realloc(Width, (Nb_patterns + 1) * sizeof(int));
			Width = realloc(Width, (Nb_patterns + 1) * sizeof(int));
			Height = realloc(Height, (Nb_patterns + 1) * sizeof(int));
			Nb_patterns ++;
			Patterns[Nb_patterns - 1] = NULL;
			Width[Nb_patterns - 1] = 0;
			Height[Nb_patterns - 1] = 0;
			if (line[0] == '\0')
				continue;
		}

		if (Width[Nb_patterns - 1] == 0) {
			Width[Nb_patterns - 1] = strlen(line);
		}
		Patterns[Nb_patterns - 1] = realloc(Patterns[Nb_patterns - 1], (Height[Nb_patterns - 1] + 1) * sizeof(char *));
		if (Patterns[Nb_patterns - 1] == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		Height[Nb_patterns - 1]++;

		Patterns[Nb_patterns - 1][Height[Nb_patterns -1] - 1] = malloc(Width[Nb_patterns - 1] + 1);
		strncpy(Patterns[Nb_patterns - 1][Height[Nb_patterns -1] - 1], line, Width[Nb_patterns - 1] + 1);
	}
	fprintf(stderr, "   Loaded %d patterns\n", Nb_patterns);
	return 0;
}



int horizontal_mirror(char **pattern, int width, int height, int row)
{
	int errors = 0;

	for (int j = 0; j < height - 1; j++) {
		if (row - j < 0)
			break;
		if (row + 1 + j >= height)
			break;
		for (int i = 0; i < width; i ++)
			if (pattern[row - j][i] != pattern[row + 1 + j][i])
				errors ++;
	}
	return errors;
}



int vertical_mirror(char **pattern, int width, int height, int column)
{
	int errors = 0;

	for (int i = 0; i < height; i ++) {
		for (int j = 0; j < width - 1; j++) {
			if (column - j < 0)
				break;
			if (column + 1 + j >= width)
				break;
			if (pattern[i][column - j] != pattern[i][column + 1 + j])
				errors ++;
		}
	}
	return errors;
}



int find_horizontal_mirror_errors(char **pattern, int width, int height, int errors)
{
	for (int i = 0; i < height - 1; i ++) {
		if (horizontal_mirror(pattern, width, height, i) == errors) {
			return i + 1;
		}
	}
	return 0;
}



int find_vertical_mirror_errors(char **pattern, int width, int height, int errors)
{
	for (int i = 0; i < width - 1; i ++) {
		if (vertical_mirror(pattern, width, height, i) == errors) {
			return i + 1;
		}
	}

	return 0;
}



void part_function(int nb_errors)
{
	long long int sum = 0;

	load_patterns();
	for (int i = 0; i < Nb_patterns; i ++) {
		sum += 100 * find_horizontal_mirror_errors(Patterns[i], Width[i], Height[i], nb_errors);
		sum += find_vertical_mirror_errors(Patterns[i], Width[i], Height[i], nb_errors);
	}

	printf("   Sum = %lld\n", sum);
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
			part_function(0);
			break;
		case 2:
			part_function(1);
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
