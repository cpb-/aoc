#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	long int destination;
	long int source;
	long int range;
} map;

map *maps[7] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int  count[7] = {0, 0, 0, 0, 0, 0, 0};


long int *seeds = NULL;
int seeds_count = 0;



typedef struct {
	long int start;
	long int range;
} seed_range;

seed_range *seed_ranges = NULL;
int seed_ranges_count = 0;



int load_map(int num)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		maps[num] = realloc(maps[num], (count[num] + 1) * sizeof(map));
		if (sscanf(line, "%ld %ld %ld", &(maps[num][count[num]].destination), &(maps[num][count[num]].source), &(maps[num][count[num]].range)) != 3)
			return -1;
		count[num] ++;
	}
	return 0;
}



void display_map(int num)
{
	for (int n = 0; n < count[num]; n ++) {
		fprintf(stderr, "%ld -> %ld (%ld)\n", maps[num][n].source, maps[num][n].destination, maps[num][n].range);
	}
}



int load_seeds(void)
{
	char line[1024];
	int offset;
	long int seed;
	int n;

	if (fgets(line, 1024, stdin) == NULL)
		return -1;

	if (strncmp(line, "seeds: ", 7) != 0)
		return -1;

	offset = 7;
	while (sscanf(&(line[offset]), "%ld %n", &seed, &n) == 1) {
		seeds = realloc(seeds, (seeds_count + 1) * sizeof(long int));
		seeds[seeds_count] = seed;
		seeds_count ++;
		offset += n;
	}

	fgets(line, 1024, stdin);
	return 0;
}



int load_seed_ranges(void)
{
	char line[1024];
	int offset;
	long int start, range;
	int n;

	if (fgets(line, 1024, stdin) == NULL)
		return -1;

	if (strncmp(line, "seeds: ", 7) != 0)
		return -1;

	offset = 7;
	while (sscanf(&(line[offset]), "%ld %ld %n", &start, &range, &n) == 2) {
		seed_ranges = realloc(seed_ranges, (seed_ranges_count + 1) * sizeof(seed_range));
		seed_ranges[seed_ranges_count].start = start;
		seed_ranges[seed_ranges_count].range = range;
		seed_ranges_count ++;
		offset += n;
	}
	fprintf(stderr, "%d seed ranges loaded\n", seed_ranges_count);
	fgets(line, 1024, stdin);
	return 0;
}



int load_maps(void)
{
	char line[1024];

	for (int m = 0; m < 7; m ++) {
		if (fgets(line, 1024, stdin) == NULL)
			return -1;
		fprintf(stderr, "loading : %s", line);
		load_map(m);
	}
	return 0;
}



void display_maps(void)
{
	for (int m = 0; m < 7; m ++) {
		display_map(m);
		fprintf(stderr, "\n");
	}
}



long int process_input(long int input, int map_num)
{
	for (int n = 0; n < count[map_num]; n++) {
		if (input < maps[map_num][n].source)
			continue;
		if (input > maps[map_num][n].source + maps[map_num][n].range)
			continue;
		return maps[map_num][n].destination + input - maps[map_num][n].source;
	}
	return input;
}



long int process_seed(long int seed)
{
	long int in = seed;

	for (int m = 0; m < 7; m++) {
		in = process_input(in, m);
	}
	return in;
}



void part_1(void)
{
	long int lowest = -1;
	long int out;

	if ((load_seeds() != 0) || (load_maps() != 0))
		exit(EXIT_FAILURE);

	for (int s = 0; s < seeds_count; s ++) {
		out = process_seed(seeds[s]);
		if ((lowest < 0) || (out < lowest))
			lowest = out;
	}

	printf("  Lowest location = %ld\n", lowest);
}



void part_2(void)
{
	long int lowest = -1;
	long int out;

	if ((load_seed_ranges() != 0) || (load_maps() != 0))
		exit(EXIT_FAILURE);

	// Just for fun: bruteforce on every seeds. ~12 minutes computing.
	for (int s = 0; s < seed_ranges_count; s ++) {
		fprintf(stderr, "Processin seed range starting at %ld\n", seed_ranges[s].start);
		for (long int seed = seed_ranges[s].start; seed < seed_ranges[s].start + seed_ranges[s].range; seed ++) {
			out = process_seed(seed);
			if ((lowest < 0) || (out < lowest))
				lowest = out;
		}
	}

	printf("  Lowest location = %ld\n", lowest);
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
