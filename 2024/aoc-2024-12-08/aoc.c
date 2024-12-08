#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char **array = NULL;
int width = 0;
int height = 0;


typedef struct {
	int x;
	int y;
} ant_t;


typedef struct {
	int letter;
	ant_t *ants;
	int nb_ants;
} freq_t;


freq_t *freqs = NULL;
int nb_freqs = 0;



void fill_frequencies_table(void)
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (array[y][x] == '.')
				continue;
			int f;
			for (f = 0; f < nb_freqs; f ++)
				if (freqs[f].letter == array[y][x])
					break;
			if (f == nb_freqs) {
				freqs = realloc(freqs, (nb_freqs + 1) * sizeof(freq_t));
				freqs[f].letter = array[y][x];
				freqs[f].ants = NULL;
				freqs[f].nb_ants = 0;
				nb_freqs++;
			}
			freqs[f].ants = realloc(freqs[f].ants, (freqs[f].nb_ants + 1) * sizeof(ant_t));
			freqs[f].ants[freqs[f].nb_ants].x = x;
			freqs[f].ants[freqs[f].nb_ants].y = y;
			freqs[f].nb_ants ++;
		}
	}
}



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



char **antinodes = NULL;

void fill_and_count_antinodes_1(void)
{
	antinodes = malloc(height * sizeof(char *));
	for (int i = 0; i < height; i++) {
		antinodes[i] = malloc(width + 1);
		memset(antinodes[i], '.', width);
		antinodes[i][width] = '\0';
	}

	for (int f = 0; f < nb_freqs; f++) {
		freq_t *fq = &(freqs[f]);
		for (int i = 0; i < fq->nb_ants - 1; i ++) {
			for (int j = i + 1; j < fq->nb_ants; j++) {
				int x1 = 2 * fq->ants[i].x - fq->ants[j].x;
				int y1 = 2 * fq->ants[i].y - fq->ants[j].y;

				int x2 = 2 * fq->ants[j].x - fq->ants[i].x;
				int y2 = 2 * fq->ants[j].y - fq->ants[i].y;

				if ((x1 >= 0) && (x1 < width) && (y1 >= 0) && (y1 < height))
					antinodes[y1][x1] = '#';

				if ((x2 >= 0) && (x2 < width) && (y2 >= 0) && (y2 < height))
					antinodes[y2][x2] = '#';

			}
		}
	}

	int count = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (antinodes[j][i] == '#')
				count ++;

	printf("Count = %d\n", count);
}



void fill_and_count_antinodes_2(void)
{
	antinodes = malloc(height * sizeof(char *));
	for (int i = 0; i < height; i++) {
		antinodes[i] = malloc(width + 1);
		memset(antinodes[i], '.', width);
		antinodes[i][width] = '\0';
	}

	for (int f = 0; f < nb_freqs; f++) {
		freq_t *fq = &(freqs[f]);
		for (int i = 0; i < fq->nb_ants - 1; i ++) {
			for (int j = i + 1; j < fq->nb_ants; j++) {
				int dx = fq->ants[j].x - fq->ants[i].x;
				int dy = fq->ants[j].y - fq->ants[i].y;

				int x = fq->ants[i].x;
				int y = fq->ants[i].y;

				while ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
					antinodes[y][x] = '#';
					x += dx;
					y += dy;
				}

				x = fq->ants[i].x;
				y = fq->ants[i].y;

				while ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
					antinodes[y][x] = '#';
					x -= dx;
					y -= dy;
				}


			}
		}
	}

	int count = 0;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			if (antinodes[j][i] == '#')
				count ++;

	printf("Count = %d\n", count);
}



void part_1(void)
{
	fill_frequencies_table();
	fill_and_count_antinodes_1();
}


void part_2(void)
{
	fill_frequencies_table();
	fill_and_count_antinodes_2();
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_array();

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
