#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void read_initial_fish(void);
void add_fish(int value);
void handle_fishes(int nb_fish);

int *fish = NULL;
int nb_fish = 0;


int main(int argc, char *argv[])
{
	read_initial_fish();

	for (int day = 0; day < 80; day++) {
		handle_fishes(nb_fish);
	}

	fprintf(stdout, "After 80 days: %d fish\n", nb_fish);

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
	fprintf(stderr, "Found %d fish\n", nb_fish);
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


void handle_fishes(int nb)
{
	for (int i = 0; i < nb; i++) {
		fish[i] --;
		if (fish[i] < 0) {
			fish[i] = 6;
			add_fish(8);
		}
	}
}

