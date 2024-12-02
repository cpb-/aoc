#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int *list[2] = { NULL, NULL };
int  nb_items = 0;


void load_lists(void)
{
	char line[128];

	while (fgets(line, 127, stdin) != NULL) {
		int a, b;
		if (sscanf(line, "%d %d", &a, &b) != 2)
			break;
		nb_items ++;
		list[0] = realloc(list[0], nb_items * sizeof(int));
		list[1] = realloc(list[1], nb_items * sizeof(int));
		list[0][nb_items - 1] = a;
		list[1][nb_items - 1] = b;
	}
}



int comparint(const void *va, const void *vb)
{
	int *a = (int *)va;
	int *b = (int *)vb;

	return *a < *b ? -1 : *a == *b ? 0 : 1;
}



void part_1(void)
{
	load_lists();

	qsort(list[0], nb_items, sizeof(int), comparint);
	qsort(list[1], nb_items, sizeof(int), comparint);

	int dist = 0;

	for (int i = 0; i < nb_items; i++) {
		dist += abs(list[0][i] - list[1][i]);
	}
	printf("Total distance = %d\n", dist);
}


void part_2(void)
{
	load_lists();

	int sum = 0;

	for (int i = 0; i < nb_items; i ++)
		for (int j = 0; j < nb_items; j ++)
			if (list[0][i] == list[1][j])
				sum += list[0][i];
	printf("Similarity = %d\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

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
