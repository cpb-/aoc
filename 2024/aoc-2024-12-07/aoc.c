#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	long long int result;
	long long int *operands;
	int nb_operands;
} equation_t;

equation_t *equations = NULL;
int nb_equations = 0;

void read_equations(void)
{
	char line[4096];

	while (fgets(line, 4095, stdin) != NULL) {
		line[strlen(line) - 1] = '\0';

		if (line[0] == '\0')
			break;

		equations = realloc(equations, (nb_equations + 1) * sizeof(equation_t));
		equations[nb_equations].operands = NULL;
		equations[nb_equations].nb_operands = 0;
		int pos;
		sscanf(line, "%lld: %n", &(equations[nb_equations].result), &pos);

		long long int v;
		int n;

		while (sscanf(&(line[pos]), "%lld %n", &v, &n) == 1) {
			equations[nb_equations].operands = realloc(equations[nb_equations].operands, (equations[nb_equations].nb_operands + 1) * sizeof(long long int));
			equations[nb_equations].operands[equations[nb_equations].nb_operands] = v;
			equations[nb_equations].nb_operands ++;
			pos += n;
		}
		nb_equations ++;
	}
	fprintf(stderr, "Read: %d equations\n", nb_equations);
}


int check_equation_p1(equation_t *eq, long long int part, int rank)
{
	if (rank == eq-> nb_operands)
		return (eq->result == part);
	return (check_equation_p1(eq, part * eq->operands[rank], rank + 1))
	    || (check_equation_p1(eq, part + eq->operands[rank], rank + 1));
}


long long int concat(long long int a, long long int b)
{
	int nb = 1 + log10(b);
	return a * (pow(10, nb)) + b;
}


int check_equation_p2(equation_t *eq, long long int part, int rank)
{
	if (rank == eq-> nb_operands)
		return (eq->result == part);
	return (check_equation_p2(eq, part * eq->operands[rank], rank + 1))
	    || (check_equation_p2(eq, part + eq->operands[rank], rank + 1))
	    || (check_equation_p2(eq, concat(part, eq->operands[rank]), rank + 1));
}


void part_1(void)
{
	long long int sum = 0;

	for (int i = 0; i < nb_equations; i++)
		if (check_equation_p1(&(equations[i]), equations[i].operands[0], 1))
			sum += equations[i].result;
	printf("Sum = %lld\n", sum);
}


void part_2(void)
{
	long long int sum = 0;

	for (int i = 0; i < nb_equations; i++)
		if (check_equation_p2(&(equations[i]), equations[i].operands[0], 1))
			sum += equations[i].result;
	printf("Sum = %lld\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	read_equations();

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
