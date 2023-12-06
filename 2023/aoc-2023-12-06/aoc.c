#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
	long long int time;
	long long int distance;
} race;


race races_example_1[3] = { {7, 9}, {15, 40}, {30, 200} };
race races_input[4] = { {46, 214}, {80, 1177}, {78, 1402}, {66, 1024} };

race race_example_2 = { 71530, 940200 };
race race_input_2 = { 46807866, 214117714021024 };



long long int number_of_ways(race *rc)
{
	long long int nb = 0;

	for (int i = 0; i < rc->time; i++)
		if ((i *(rc->time - i)) > rc->distance)
			nb ++;
	return nb;
}



void part_1(void)
{
	long long int product = 1;

	for (int i = 0; i < 3; i ++)
		product *= number_of_ways(&(races_example_1[i]));

	printf("  Product example 1 = %lld\n", product);



	product = 1;
	for (int i = 0; i < 4; i ++)
		product *= number_of_ways(&(races_input[i]));

	printf("  Product input 1 = %lld\n", product);
}




void part_2(void)
{
	printf("  Example 2 = %lld\n", number_of_ways(&race_example_2));

	printf("  Input 2 = %lld\n", number_of_ways(&race_input_2));
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
