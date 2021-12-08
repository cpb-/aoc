#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void read_initial_values(void);
void add_value(int val);
int compute_fuel(int val);

int *values = NULL;
int nb_values = 0;
int min_value = -1;
int max_value = -1;

int main(int argc, char *argv[])
{
	int min_fuel = -1;
	int min_value = -1;

	read_initial_values();


	for (int i = min_value; i <= max_value; i++) {
		int f = compute_fuel(i);
		if ((f < min_fuel) || (min_fuel < 0)) {
			min_fuel = f;
			min_value = i;
		}
	}

	fprintf(stdout, "min_value = %d, min_fuel = %d\n", min_value, min_fuel);

	return EXIT_SUCCESS;
}


void read_initial_values(void)
{
	char buffer[4096];

	if (fgets(buffer, 4095, stdin) == NULL) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}

	int start = 0;
	int val;
	int consummed;
	while (sscanf(&(buffer[start]), "%d%n", &val, &consummed) == 1) {
		add_value(val);
		start += consummed;
		if (buffer[start] == ',')
			start ++;
		if (! isdigit(buffer[start]))
			break;
	}
	fprintf(stderr, "Found %d values\n", nb_values);
}


void add_value(int val)
{
	values = realloc(values, (nb_values + 1) * sizeof(int));
	if (values == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	if ((val < min_value) || (min_value == -1))
		min_value = val;
	if ((val > max_value) || (max_value == -1))
		max_value = val;
	values[nb_values] = val;
	nb_values ++;
}


int sum(int n)
{
	int d = n * (n + 1) / 2;
	return d;
}

int compute_fuel(int val)
{
	int ret = 0;
	for (int i = 0; i < nb_values; i ++)
		ret += sum(abs(val - values[i]));
	return ret;
}
