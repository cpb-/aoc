#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
size_t width = 0;
size_t height = 0;



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



void display_array(void)
{
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++)
			fprintf(stderr, "%c", array[i][j]);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}



char **array_backup = NULL;


void backup_array(void)
{
	array_backup = malloc(height * sizeof(char *));
	if (array_backup == NULL) {
		perror("malloc 1");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < height; i++) {
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
	for (size_t i = 0; i < height; i++)
		memset(array_backup[i], 0, width + 1);
}



void restore_backup(void)
{
	for (size_t i = 0; i < height; i++) {
		strcpy(array[i], array_backup[i]);
		free(array_backup[i]);
	}
	free(array_backup);
	array_backup = NULL;
}



int array_equals_to_backup(void)
{
	for (size_t i = 0; i < height; i++)
		if (strcmp(array_backup[i], array[i]) != 0)
			return 0;
	return 1;
}



void part_1(void)
{
}


void part_2(void)
{
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
			part_1();
			break;
		case 2:
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
