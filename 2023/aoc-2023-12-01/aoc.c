#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



void part_1(void)
{
	char buffer[1024];
	int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL) {
		int first = 0;
		int second = 0;
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (isdigit(buffer[i])) {
				first = buffer[i] - '0';
				break;
			}
		}
		for (int i = strlen(buffer) - 1; i >= 0; i--) {
			if (isdigit(buffer[i])) {
				second = buffer[i] - '0';
				break;
			}
		}
		sum += first * 10 + second;
	}
	fprintf(stdout, "Sum = %d\n", sum);
}


char *digits[] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };

void part_2(void)
{
	char buffer[1024];
	int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL) {
		int first = 0;
		int second = 0;
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (isdigit(buffer[i])) {
				first = buffer[i] - '0';
				break;
			}
			for (int j = 0; j < 9; j ++) {
				if (strncmp(&buffer[i], digits[j], strlen(digits[j])) == 0) {
					first = j + 1;
					break;
				}
			}
			if (first != 0)
				break;
		}
		for (int i = strlen(buffer) - 1; i >= 0; i--) {
			if (isdigit(buffer[i])) {
				second = buffer[i] - '0';
				break;
			}
			for (int j = 0; j < 9; j ++) {
				if (strncmp(&buffer[i], digits[j], strlen(digits[j])) == 0) {
					second = j + 1;
					break;
				}
			}
			if (second != 0)
				break;
		}
		sum += first * 10 + second;
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
