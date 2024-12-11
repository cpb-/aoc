#include <ctype.h>
#include <math.h>
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


typedef struct {
	long long int in;
	long long int out;
	int nb;
} memo;


memo *Memo = NULL;

int Nb_memo = 0;


long long int check_memoization(long long int in, int nb)
{
	for (int n = 0; n < Nb_memo; n++)
		if ((in == Memo[n].in) && (nb == Memo[n].nb))
			return Memo[n].out;
	return -1;
}



void memoize(long long int in, long long int out, int nb)
{
	memo *m;
	m = realloc(Memo, (Nb_memo + 1) * sizeof(memo));
	if (m == NULL)
		return;
	Memo = m;
	Memo[Nb_memo].in = in;
	Memo[Nb_memo].out = out;
	Memo[Nb_memo].nb = nb;
	Nb_memo ++;
}



void clear_memoization(void)
{
	free(Memo);
	Memo = NULL;
	Nb_memo = 0;
}








int nb_digits(long long int v)
{
	return 1 + (int)(log10l(v));
}

long long int count_stones(long long int v, int nb)
{

	long long int ret = check_memoization(v, nb);
	if (ret >= 0)
		return ret;

	if (nb == 0)
		return 1;

	if (v == 0)
		return count_stones(1, nb - 1);

	int n = nb_digits(v);
	if (n % 2 == 0) {
		long long int v1 = v / pow(10, n / 2);
		long long int m = pow(10, n / 2);
		long long int v2 = v % m;
		ret = count_stones(v1, nb - 1) + count_stones(v2, nb - 1);
	} else {
		ret = count_stones(v * 2024, nb - 1);
	}
	memoize(v, ret, nb);
	return ret;
}


void part_1(void)
{
	long long int count = 0;
	long long int v;

	while (fscanf(stdin, "%lld", &v) == 1)
		count += count_stones(v, 25);

	printf("Count = %lld\n", count);
}


void part_2(void)
{
	long long int count = 0;
	long long int v;

	while (fscanf(stdin, "%lld", &v) == 1)
		count += count_stones(v, 75);

	printf("Count = %lld\n", count);
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
