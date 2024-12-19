#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **towels = NULL;
int nb_towels = 0;

char **designs = NULL;
int nb_designs = 0;

void load_towels_and_designs(void)
{
	char line[4096];

	if (fgets(line, 4096, stdin) == NULL)
		exit(1);

	int pos = 0;

	while (isalpha(line[pos])) {
		int n = 0;
		while(isalpha(line[pos + n]))
			n++;
		towels = realloc(towels, (nb_towels + 1) * sizeof(char *));
		towels[nb_towels] = malloc(n + 1);
		memcpy(towels[nb_towels], &(line[pos]), n);
		towels[nb_towels][n] = '\0';
		nb_towels++;

		pos = pos + n;
		while ((line[pos] == ',') || (line[pos] == ' '))
			pos++;
	}

	printf("Nb towels: %d\n", nb_towels);

	if (fgets(line, 4096, stdin) == NULL)
		exit(1);

	while (fgets(line, 4096, stdin) != NULL) {
		line[strlen(line) - 1] = '\0';
		designs = realloc(designs, (nb_designs + 1) * sizeof(char *));
		designs[nb_designs] = malloc(strlen(line) + 1);
		strcpy(designs[nb_designs], line);
		nb_designs++;
	}
	printf("Nb designs: %d\n", nb_designs);
}



typedef struct {
	int rank;
	long long int ret;
} memo;


memo *Memo = NULL;
int Nb_memo = 0;


long long int check_memoization(int rank)
{
	for (int n = 0; n < Nb_memo; n++)
		if (rank == Memo[n].rank)
			return Memo[n].ret;
	return -1;
}



void memoize(int rank, long long int ret)
{
	memo *m;
	m = realloc(Memo, (Nb_memo + 1) * sizeof(memo));
	if (m == NULL)
		return;
	Memo = m;
	Memo[Nb_memo].rank = rank;
	Memo[Nb_memo].ret = ret;
	Nb_memo ++;
}



void clear_memoization(void)
{
	free(Memo);
	Memo = NULL;
	Nb_memo = 0;
}



long long int try_letter(char *design, int rank, int part)
{
	long long int ret = 0;

	long long int r = check_memoization(rank);
	if (r >= 0)
		return r;

	if (design[rank] == '\0') {
		ret = 1;
	} else {
		for (int i = 0; i < nb_towels; i++) {
			int j;
			for (j = 0; towels[i][j] != '\0'; j++) {
				if (design[rank + j] == '\0')
					break;
				if (design[rank + j] != towels[i][j])
					break;
			}
			if (towels[i][j] == '\0') {
				if (part == 1) {
					if (try_letter(design, rank + j, part))
						return 1;
				} else {
					ret += try_letter(design, rank + j, part);
				}
			}
		}
	}
	memoize(rank, ret);
	return ret;
}



void part_1(void)
{
	int count = 0;

	for (int i = 0; i < nb_designs; i++) {
		clear_memoization();
		count += try_letter(designs[i], 0, 1);
	}
	printf("Count = %d\n", count);
}



void part_2(void)
{
	long long int count = 0;

	for (int i = 0; i < nb_designs; i++) {
		clear_memoization();
		long long int n = try_letter(designs[i], 0, 2);
		count += n;
	}
	printf("Count = %lld\n", count);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_towels_and_designs();

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
