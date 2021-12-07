#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void read_initial_fish(void);
void add_fish(int value);
void handle_fishes(int nb_fish);

char *fish = NULL;
int nb_fish = 0;


int main(int argc, char *argv[])
{
	read_initial_fish();

	long long int sum = 0;

	for (int f = 0; f < nb_fish; f++) {
		switch(fish[f]) {
			case 0: sum += 6703087164; break;
			case 1: sum += 6206821033; break;
			case 2: sum += 5617089148; break;
			case 3: sum += 5217223242; break;
			case 4: sum += 4726100874; break;
			case 5: sum += 4368232009; break;
			case 6: sum += 3989468462; break;
			case 7: sum += 3649885552; break;
			case 8: sum += 3369186778; break;
			default:
				fprintf(stderr, "invalid value %d\n", fish[f]);
				exit(EXIT_FAILURE);
		}
	}
	fprintf(stdout, "sum = %lld\n", sum);

	return EXIT_SUCCESS;
}


void read_initial_fish(void)
{
	char buffer[4096];

	if (fgets(buffer, 4095, stdin) == NULL) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}

	int start = 0;
	int value;
	int consummed;
	while (sscanf(&(buffer[start]), "%d%n", &value, &consummed) == 1) {
		add_fish(value);
		start += consummed;
		if (buffer[start] == ',')
			start ++;
		if (! isdigit(buffer[start]))
			break;
	}
	fprintf(stderr, "Found %lld fish\n", nb_fish);
}

void add_fish(int value)
{
	fish = realloc(fish, (nb_fish + 1) * sizeof(int));
	if (fish == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}
	fish[nb_fish] = value;
	nb_fish ++;
}

