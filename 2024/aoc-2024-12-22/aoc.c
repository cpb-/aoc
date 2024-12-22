#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

unsigned long long compute_secret_numbers(unsigned long long int seed, int rank)
{
	unsigned long long int secret = seed;
	unsigned long long int result;

	for (int i = 0; i < rank; i ++) {

		result = secret *  64;
		secret = secret ^ result;
		secret = secret % 16777216;

		result = secret / 32;
		secret = secret ^ result;
		secret = secret % 16777216;

		result = secret * 2048;
		secret = secret ^ result;
		secret = secret % 16777216;
	}
	return secret;
}


long long int *values = NULL;
int nb_values = 0;

void read_values(void)
{
	char line[128];
	while (fgets(line, 128, stdin) != NULL) {
		values = realloc(values, (nb_values + 1) * sizeof(long long int));
		if (sscanf(line, "%lld", &(values[nb_values])) == 1)
			nb_values ++;
	}
}


void part_1(void)
{
	unsigned long long sum = 0;

	for (int n = 0; n < nb_values; n++)
		sum += compute_secret_numbers(values[n], 2000);
	printf("Sum = %lld\n", sum);
}


long long int ** secrets = NULL;

int **diffs = NULL;

void compute_diffs(void)
{
	diffs = realloc(diffs, nb_values * sizeof(int *));
	secrets = realloc(secrets, nb_values * sizeof(long long int *));

	for (int i = 0; i < nb_values; i++) {
		secrets[i] = calloc(2000, sizeof(long long int));
		if (secrets[i] == NULL) {
			perror("malloc");
			exit(1);
		}

		secrets[i][0] = values[i];

		for (int k = 1; k < 2000;k ++) {
			long long int secret = secrets[i][k - 1];
			long long result = secret *  64;
			secret = secret ^ result;
			secret = secret % 16777216;

			result = secret / 32;
			secret = secret ^ result;
			secret = secret % 16777216;

			result = secret * 2048;
			secret = secret ^ result;
			secret = secret % 16777216;

			secrets[i][k] = secret;
		}


		diffs[i] = calloc(1999, sizeof(int));
		if (diffs[i] == NULL) {
			perror("malloc");
			exit(1);
		}

		for (int k = 0; k < 1999;k ++) {
			diffs[i][k] = (secrets[i][k + 1] % 10) - (secrets[i][k] % 10);
		}
	}

}

int sell_after_sequence(int v, int a, int b, int c, int d)
{
	for (int i = 0; i < 1996; i ++)
		if ((diffs[v][i] == a)
		 && (diffs[v][i + 1] == b)
		 && (diffs[v][i + 2] == c)
		 && (diffs[v][i + 3] == d))
		return secrets[v][i + 4]%10;
	return 0;
}

void part_2(void)
{
	compute_diffs();

	long int best = 0;

	for (int a = -9; a < 10; a ++)
		for (int b = -9; b < 10; b ++)
			for (int c = -9; c < 10; c ++)
				for (int d = -9; d < 10; d ++) {
					long int sum = 0;
					for (int v = 0; v < nb_values; v++)
						sum += sell_after_sequence(v, a, b, c, d);
					if (sum > best) {
						best = sum;
						printf("Best = %d, (%d, %d, %d, %d)\n",
							best, a, b, c, d);
					}
				}
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	read_values();

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
