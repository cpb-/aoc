#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {

	char name[4];
	char left[4];
	char right[4];

} node;


node *Nodes = NULL;
int   Nb_nodes;

char Instructions[1024];

int Start = 0;

long long int *Elements = NULL;
int  Nb_elements;



void read_instructions(void)
{
	char buffer[1024];

	if (fgets(Instructions, 1024, stdin) == NULL)
		exit(EXIT_FAILURE);
	Instructions[strlen(Instructions) - 1] = '\0';

	if (fgets(buffer, 1024, stdin) == NULL)
		exit(EXIT_FAILURE);

	// fprintf(stderr, "Instructions read\n");
}



void read_nodes(int part)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {
		if (strlen(line) < 15)
			break;
		Nodes = realloc(Nodes, (Nb_nodes + 1) * sizeof(node));
		if (Nodes == NULL)
			exit(EXIT_FAILURE);
		strncpy(Nodes[Nb_nodes].name, &(line[0]), 3);
		Nodes[Nb_nodes].name[3] = '\0';
		strncpy(Nodes[Nb_nodes].left, &(line[7]), 3);
		Nodes[Nb_nodes].left[3] = '\0';
		strncpy(Nodes[Nb_nodes].right, &(line[12]), 3);
		Nodes[Nb_nodes].right[3] = '\0';

		if (strcmp(Nodes[Nb_nodes].name, "AAA") == 0)
			Start = Nb_nodes;

		if ((part == 2) && (Nodes[Nb_nodes].name[2] == 'A')) {
			Elements = realloc(Elements, (Nb_elements + 1) * sizeof(long long int));
			Elements[Nb_elements] = Nb_nodes;
			Nb_elements ++;
		}

		Nb_nodes ++;
	}
	// fprintf(stderr, "%d nodes read\n", Nb_nodes);
}



int find_path(int start, int part)
{
	int element;
	int step = 0;
	int i = 0;

	element = start;

	for(;;) {
		if (part == 1)
			if (strcmp(Nodes[element].name, "ZZZ") == 0)
				break;
		if (part == 2)
			if (Nodes[element].name[2] == 'Z')
				break;
		step ++;
		if (Instructions[i] == 'L') {
			for (int n = 0; n < Nb_nodes; n++) {
				if (strcmp(Nodes[n].name, Nodes[element].left) == 0) {
					element = n;
					break;
				}
			}
		} else {
			for (int n = 0; n < Nb_nodes; n++) {
				if (strcmp(Nodes[n].name, Nodes[element].right) == 0) {
					element = n;
					break;
				}
			}
		}
		i ++;
		if (Instructions[i] == '\0')
			i = 0;
	}
	return step;
}



void part_1(void)
{
	int step = 0;

	read_instructions();
	read_nodes(1);
	step = find_path(Start, 1);
	printf("   Nb steps = %d\n", step);
}



long long int *Primes = NULL;
int Nb_primes = 0;


int isprime(long long int value)
{
	for (int i = 2; i < value; i ++)
		if (value %i == 0)
			return 0;
	return 1;
}



void compute_primes_table(void)
{
	for (int i = 2; i < 20000; i++) {
		if (isprime(i)) {
			Primes = realloc(Primes, (Nb_primes + 1) * sizeof(long long int));
			Primes[Nb_primes] = i;
			Nb_primes ++;
		}
	}
}



long long int *compute_prime_factors(long long int num)
{
	long long int *list = NULL;
	int nb_primes = 0;
	int i = 0;

	if (Primes == NULL)
		compute_primes_table();

	while ((i < Nb_primes) && (Primes[i] <= num)) {
		if (num % Primes[i] == 0) {
			list = realloc(list, (nb_primes + 1) * sizeof(long long int));
			list[nb_primes] = Primes[i];
			nb_primes ++;
			num = num / Primes[i];
			continue;
		}
		i++;
	}
	list = realloc(list, (nb_primes + 1) * sizeof(long long int));
	list[nb_primes] = 0; // END
	nb_primes ++;

	return list;

}



long long int compute_gcd(long long int n1, long long int n2)
{
	long long int *primes = NULL;
	int nb_primes = 0;
	long long int gcd = 1;

	long long int *prime_factors_1 = compute_prime_factors(n1);
	long long int *prime_factors_2 = compute_prime_factors(n2);


	for (int i = 0; prime_factors_1[i] != 0; i++) {
		for (int j = 0; prime_factors_2[j] != 0; j ++) {
			if (prime_factors_1[i] == prime_factors_2[j]) {
				primes = realloc(primes, (nb_primes + 1) * sizeof(long long int));
				primes[nb_primes] = prime_factors_1[i];
				nb_primes ++;
				prime_factors_2[j] = -1; // already used
				break;
			}
		}
	}
	free(prime_factors_1);
	free(prime_factors_2);

	for (int i = 0; i < nb_primes; i ++)
		gcd *= primes[i];
	free(primes);

	return gcd;
}



long long int compute_lcm(long long int n1, long long int n2)
{
	return n1 * n2 / compute_gcd(n1, n2);
}



long long int compute_multiple_lcm(long long int *table, int nb)
{
	long long int lcm = 1;

	for (int i = 0; i < nb; i ++) {
		lcm = compute_lcm(table[i], lcm);
	}

	return lcm;
}



void part_2(void)
{
	long long int *steps;

	read_instructions();
	read_nodes(2);

	steps = calloc(Nb_elements, sizeof(long long int));

	for (int i = 0; i < Nb_elements; i++)
		steps[i] = find_path(Elements[i], 2);

	printf(" LCM = %lld\n", compute_multiple_lcm(steps, Nb_elements));
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
