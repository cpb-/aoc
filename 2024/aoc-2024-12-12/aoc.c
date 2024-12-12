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



typedef struct {
	int c;
	int x;
	int y;
	int area;
	int perim;
	int fences;
	int price_1;
	int price_2;
} region_t;


region_t *regs = NULL;
int nb_regs = 0;


void find_neighbours(int j, int i, region_t *r)
{
	array_backup[j][i] = '.';
	r->area += 1;
	r->perim += 4;

	if ((j > 0) && (array[j - 1][i] == r->c))
		r->perim --;
	if ((j < height - 1) && (array[j + 1][i] == r->c))
		r->perim --;
	if ((i > 0) && (array[j][i - 1] == r->c))
		r->perim --;
	if ((i < width - 1) && (array[j][i + 1] == r->c))
		r->perim --;

	if ((j == 0) || (array[j -1][i] != r->c)) {
		if ((i == 0) || (array[j][i - 1] != r->c))
			r->fences ++;
		if ((i == width - 1) || (array[j][i + 1] != r->c))
			r->fences ++;
	}

	if ((j == height - 1) || (array[j + 1][i] != r->c)) {
		if ((i == 0) || (array[j][i - 1] != r->c))
			r->fences ++;
		if ((i == width - 1) || (array[j][i + 1] != r->c))
			r->fences ++;
	}

	if ((j > 0) && (array[j - 1][i] == r->c)) {
		if ((i > 0) && (array[j][i - 1] == r->c))
			if (array[j - 1][i - 1] != r->c)
				r->fences ++;
		if ((i < width - 1 ) && (array[j][i + 1] == r->c))
			if (array[j - 1][i + 1] != r->c)
				r->fences ++;
	}

	if ((j < height - 1) && (array[j + 1][i] == r->c)) {
		if ((i > 0) && (array[j][i - 1] == r->c))
			if (array[j + 1][i - 1] != r->c)
				r->fences ++;
		if ((i < width - 1 ) && (array[j][i + 1] == r->c))
			if (array[j + 1][i + 1] != r->c)
				r->fences ++;
	}

	r->price_1 = r->perim * r->area;
	r->price_2 = r->fences * r->area;

	if ((j > 0) && (array_backup[j - 1][i] == r->c))
		find_neighbours(j - 1, i, r);
	if ((j < height - 1) && (array_backup[j + 1][i] == r->c))
		find_neighbours(j + 1, i, r);
	if ((i > 0) && (array_backup[j][i - 1] == r->c))
		find_neighbours(j, i - 1, r);
	if ((i < width - 1) && (array_backup[j][i + 1] == r->c))
		find_neighbours(j, i + 1, r);

}

void search_regions(void)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (array_backup[j][i] == '.')
				continue;
			regs = realloc(regs, (nb_regs + 1) * sizeof(region_t));
			regs[nb_regs].c = array[j][i];
			regs[nb_regs].x = i;
			regs[nb_regs].y = j;
			regs[nb_regs].area = regs[nb_regs].perim = regs[nb_regs].fences = 0;
			find_neighbours(j, i, &(regs[nb_regs]));
			nb_regs ++;
		}
	}
}


void part_1(void)
{
	int sum = 0;

	search_regions();
	printf("Nb regions = %d\n", nb_regs);
	for (int i = 0; i < nb_regs; i++)
		sum += regs[i].price_1;
	printf("Sum = %d\n", sum);
}


void part_2(void)
{
	search_regions();
	int sum = 0;

	search_regions();
	printf("Nb regions = %d\n", nb_regs);
	for (int i = 0; i < nb_regs; i++) {
		sum += regs[i].price_2;
	}
	printf("Sum = %d\n", sum);

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
