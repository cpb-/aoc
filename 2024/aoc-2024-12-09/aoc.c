#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char scheme[32000]; // the input file size is 20000 max.
int nb_blocks = 0;
int *blocks = NULL;
int nb_id;

typedef struct myfile {
	int id;
	int start;
	int len;
	struct myfile *next;
	struct myfile *prev;
} myfile_t;

myfile_t *myfile_head = NULL;
myfile_t *myfile_tail = NULL;

void read_scheme(void)
{
	fgets(scheme, 32000, stdin);
	scheme[strlen(scheme) - 1] = '\0';

	for (int i = 0; scheme[i] != '\0'; i++)
		nb_blocks += (scheme[i] - '0');

	fprintf(stderr, "Nb blocks = %d\n", nb_blocks);

	blocks = malloc((nb_blocks + 1) * sizeof(int));

	nb_id = 0;
	int b = 0;

	for (int i = 0; scheme[i] != '\0'; i++) {
		for (int j = 0; j < scheme[i] - '0'; j++) {
			blocks[b ++] = nb_id;
		}
		nb_id ++;
		i++;
		if (scheme[i] == '\0')
			break;
		for (int j = 0; j < scheme[i] - '0'; j++) {
			blocks[b ++] = -1;
		}
	}
	fprintf(stderr, "Written : %d blocks (index = %d)\n", b, nb_id);

	b = 0;
	for (int i = 0; i < nb_id; i ++) {
		if (blocks[b] == i) {
			myfile_t *f = malloc(sizeof(myfile_t));
			if (myfile_head == NULL) {
				myfile_head = f;
				myfile_tail = f;
				f->next = NULL;
				f->prev = NULL;
			} else {
				f->prev = myfile_tail;
				f->prev->next = f;
				myfile_tail = f;
				f->next = NULL;
			}
			f->id = i;
			f->start = b;
			while (blocks[b] == i)
				b ++;
			f->len = b - f->start;
		}
		while(blocks[b] == -1)
			b++;
	}


}


void sort_blocks_1()
{
	int a = 0;
	int b = nb_blocks - 1;

	for (;;) {
		while (blocks[a] != -1)
			a++;
		while (blocks[b] == -1)
			b--;
		if (a >= b)
			break;
		blocks[a] = blocks[b];
		blocks[b] = -1;
	}

}


void sort_blocks_2()
{
	for (int i = nb_id - 1; i >= 0; i--) {

		myfile_t *f;
		for (f = myfile_head; f != NULL; f = f->next)
			if (f->id == i)
				break;
		if (f == NULL)
			continue;


		for (myfile_t *a = myfile_head; a != myfile_tail; a = a->next) {

			if (a == f)
				break;

			myfile_t *b = a->next;

			if ((b->start - a->start - a->len) >= f->len) {

				if (b == f) {
					f->start = a->start+a->len;
					break;
				}

				if (f->next == NULL)
					myfile_tail = f->prev;
				else
					f->next->prev = f->prev;
				if (f->prev == NULL)
					myfile_head = f->next;
				else
					f->prev->next = f->next;
				f->prev = a;
				a->next = f;
				f->next = b;
				b->prev = f;
				f->start = a->start+a->len;
				break;
			}
		}
	}

}


long long int mult_and_sum(void)
{
	long long int sum = 0;
	long long int v;

	for (long long int i = 0; blocks[i] != -1; i++) {
		v = blocks[i];
		sum = sum + ( i * v);
	}

	return sum;

}

long long int mult_and_sum_2(void)
{
	long long int sum = 0;
	long long int v;

	for (myfile_t *f = myfile_head; f != NULL; f = f->next)
		for (int i = 0; i < f->len; i++) {
			v = f->id * (f->start + i);
			sum = sum + v;
		}

	return sum;

}

void part_1(void)
{
	read_scheme();
	sort_blocks_1();
	printf("Sum  = %lld\n", mult_and_sum());
}


void part_2(void)
{
	read_scheme();
	sort_blocks_2();
	printf("Sum  = %lld\n", mult_and_sum_2());
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
