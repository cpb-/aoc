#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int most_common_bit(long int *values, int nb_values, int bit, int nb_bits);
void compute_gamma_and_epsilon(long int *values, int nb_values, long int *gamma_rate, long int *epsilon_rate, int nb_bits);
void compute_oxygen_and_co2_rating(long int *values, int nb_values, int nb_bits, long int *oxygen_generator_rating, long int *co2_scrubber_rating);


int main(int argc, char *argv[])
{
	int nb_bits;

	char *buffer;
	int nb_values = 0;

	long int *values = NULL;

	long int gamma_rate = 0;
	long int epsilon_rate = 0;
	long int oxygen_generator_rating = 0;
	long int co2_scrubber_rating = 0;

	if ((argc != 2)
	 || (sscanf(argv[1], "%d", &nb_bits) != 1)) {
		fprintf(stderr, "usage: %s <nb-bits>\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	buffer = malloc(nb_bits + 3); // +3 because of eventual \r\n then \0
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	while (fgets(buffer, nb_bits + 2, stdin) != NULL) {
		values = realloc(values, (nb_values + 1) * sizeof(long int));
		if (values == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		values[nb_values] = strtol(buffer, NULL, 2);
		nb_values ++;
	}
	free(buffer);

	compute_gamma_and_epsilon(values, nb_values, &gamma_rate, &epsilon_rate, nb_bits);
	fprintf(stderr, "gamma = %ld, epsilon = %ld, power consumption = %ld\n",
		gamma_rate, epsilon_rate, (gamma_rate * epsilon_rate));

	compute_oxygen_and_co2_rating(values, nb_values, nb_bits, &oxygen_generator_rating, &co2_scrubber_rating);
	fprintf(stderr, "oxygen_generator_rating = %ld, co2_scrubber_rating = %ld, life support rating = %ld\n",
		oxygen_generator_rating, co2_scrubber_rating, oxygen_generator_rating * co2_scrubber_rating);

	return EXIT_SUCCESS;
}


void compute_gamma_and_epsilon(long int *values, int nb_values, long int *gamma_rate, long int *epsilon_rate, int nb_bits)
{
	int *counter;

	counter = calloc(nb_values, sizeof(int));
	if (counter == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < nb_values; i ++) {
		for (int j = 0; j < nb_bits; j ++) {
			if (values[i] & (1 << (nb_bits - 1 - j)))
				counter[j] ++;
			else
				counter[j] --;
		}
	}

	*gamma_rate = 0;
	*epsilon_rate = 0;

	for (int i = 0; i < nb_bits; i ++) {
		int value = 1 << (nb_bits - 1 - i);
		if (counter[i] >= 0)
			(*gamma_rate) |= value;
		else
			(*epsilon_rate) |= value;
	}
	free(counter);
}



void compute_oxygen_and_co2_rating(long int *values, int nb_values, int nb_bits, long int *oxygen_generator_rating, long int *co2_scrubber_rating)
{
	long int *remaining = NULL;
	int nb_remaining;

	remaining = calloc(nb_values, sizeof(long int));
	if (remaining == NULL) {
		perror("calloc");
		exit (EXIT_FAILURE);
	}
	for (int i = 0; i < nb_values; i ++)
		remaining[i] = values[i];
	nb_remaining = nb_values;

	for (int i = 0; i < nb_bits; i ++) {
		long int mask = 1 << (nb_bits - 1 - i);
		long int value = 0;
		if (most_common_bit(remaining, nb_remaining, i, nb_bits) == 1)
			value = mask;
		for (int j = 0; j < nb_remaining; j++) {
			if ((remaining[j] & mask) != value) {
				if (j < nb_remaining - 1) {
					memmove(&(remaining[j]), &(remaining[j + 1]), (nb_remaining - j - 1) * sizeof(long int));
				}
				nb_remaining --;
				j --;
			}
		}
		if (nb_remaining == 1) {
			break;
		}
	}
	(*oxygen_generator_rating) = remaining[0];

	for (int i = 0; i < nb_values; i ++)
		remaining[i] = values[i];
	nb_remaining = nb_values;

	for (int i = 0; i < nb_bits; i ++) {
		long int mask = 1 << (nb_bits - 1 - i);
		long int value = 0;
		if (most_common_bit(remaining, nb_remaining, i, nb_bits) == 0)
			value = mask;
		for (int j = 0; j < nb_remaining; j++) {
			if ((remaining[j] & mask) != value) {
				if (j < nb_remaining - 1) {
					memmove(&(remaining[j]), &(remaining[j + 1]), (nb_remaining - j - 1) * sizeof(long int));
				}
				nb_remaining --;
				j --;
			}
		}
		if (nb_remaining == 1) {
			break;
		}
	}
	(*co2_scrubber_rating) = remaining[0];

	free(remaining);
}



int most_common_bit(long int *values, int nb_values, int bit, int nb_bits)
{
	int zeroes = 0;
	int ones = 0;

	for (int i = 0; i < nb_values; i ++)
		if (values[i] & (1 << (nb_bits - bit - 1)))
			ones ++;
		else
			zeroes ++;
	if (ones >= zeroes)
		return 1;
	return 0;
}
