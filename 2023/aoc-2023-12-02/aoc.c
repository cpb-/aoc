#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_RED   12
#define MAX_GREEN 13
#define MAX_BLUE  14

int is_game_possible(const char *buffer, int part)
{
	int id;
	int n;
	int offset = 0;

	int min_red = 0;
	int min_green = 0;
	int min_blue = 0;

	if (sscanf(buffer, "Game %d: %n", &id, &offset) != 1)
		return 0;

	while (buffer[offset] != '\0') {

		int red = 0;
		int green = 0;
		int blue = 0;

		while ((buffer[offset] != ';') && (buffer[offset] != '\0')) {
			int number;
			if (buffer[offset] == ',')
				offset += 2;
			if (sscanf(&(buffer[offset]), "%d%n", &number, &n) == 1) {
				offset += n + 1; // white space
				if (strncmp(&(buffer[offset]), "blue", 4) == 0) {
					blue += number;
					offset += 4;
					continue;
				}
				if (strncmp(&(buffer[offset]), "green", 5) == 0) {
					green += number;
					offset += 5;
					continue;
				}
				if (strncmp(&(buffer[offset]), "red", 3) == 0) {
					red += number;
					offset += 3;
					continue;
				}
			}
		}
		if (part == 1) {
			if ((red > MAX_RED) || (green > MAX_GREEN) || (blue > MAX_BLUE))
				return 0;
		}
		if (red > min_red)
			min_red = red;
		if (green > min_green)
			min_green = green;
		if (blue > min_blue)
			min_blue = blue;

		offset ++;
	}
	if (part == 1)
		return id;

	return min_red * min_green * min_blue;
}



void part_1(void)
{
	char buffer[1024];
	int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0'; // final \n
		sum += is_game_possible(buffer, 1);
	}
	fprintf(stdout, "Sum = %d\n", sum);
}


void part_2(void)
{
	char buffer[1024];
	int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0'; // final \n
		sum += is_game_possible(buffer, 2);
	}
	fprintf(stdout, "Sum = %d\n", sum);
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
