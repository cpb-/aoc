#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{
	char *buffer;
	int *counter;
	int nb_bits;
	long int gamma_rate = 0;
	long int epsilon_rate = 0;


	if ((argc != 2) || (sscanf(argv[1], "%d", &nb_bits) != 1)) {
		fprintf(stderr, "usage: %s <nb-bits>\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	buffer = malloc(nb_bits + 3); // +3 because of eventual \r\n then \0
	if (buffer == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	counter = calloc(nb_bits, sizeof(int));
	if (counter == NULL) {
		perror("calloc");
		exit(EXIT_FAILURE);
	}
	for (int i = 0; i < nb_bits; i ++)
		counter[i] = 0;


	while (fgets(buffer, nb_bits + 2, stdin) != NULL) {
		for (int i = 0; i < nb_bits; i ++) {
			if (buffer[i] == '1')
				counter[i] ++;
			else
				counter[i] --;
		}
	}
	free(buffer);

	for (int i = 0; i < nb_bits; i ++) {
		int value = 1 << (nb_bits - 1 - i);
		if (counter[i] > 0)
			gamma_rate |= value;
		else
			epsilon_rate |= value;
	}
	free(counter);

	fprintf(stdout, "gamma = %ld, espilon = %ld, power consumption = %ld\n",
		gamma_rate, epsilon_rate, gamma_rate * epsilon_rate);

	return EXIT_SUCCESS;
}
