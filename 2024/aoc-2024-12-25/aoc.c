#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int key;
	int col[5];
} item_t;

item_t *items = NULL;
int nb_items = 0;

void load_keys_and_holes(void)
{
	char buffer[128];
	int col[5];

	for (;;) {

		int key = 0;

		for (int i = 0; i < 5; i ++)
			col[i] = 0;

		for (int i = 0; i < 7; i++) {

			if (fgets(&(buffer[0]), 128, stdin) == NULL)
				return;

			if ((i == 0) && (buffer[0] == '#'))
				key = 1;

			for (int j = 0; j < 5; j++)
				if (buffer[j] == '#')
					col[j] ++;
		}

		items = realloc(items, (nb_items + 1) * sizeof(item_t));
		items[nb_items].key = key;
		for (int i = 0; i < 5; i++)
			items[nb_items].col[i] = col[i];
		nb_items ++;
		if (fgets(buffer, 128, stdin) == NULL)
			return;
	}
}

int match(item_t *key, item_t *hole)
{
	for (int i = 0; i < 5; i++)
		if (key->col[i] + hole->col[i] > 7)
			return 0;
	return 1;
}

void part_1(void)
{
	int count = 0;
	for (int i = 0; i < nb_items; i++) {
		if (! items[i].key)
			continue;
		for (int j = 0; j < nb_items; j++) {
			if (items[j].key)
				continue;
			if (match(&(items[j]), &(items[i])))
				count ++;
		}
	}
	printf("Count = %d\n", count);
}


void part_2(void)
{
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_keys_and_holes();

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
