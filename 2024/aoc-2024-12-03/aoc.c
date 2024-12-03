#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void part_1(void)
{
	int c;

	int len_a;
	int len_b;
	char mul_a[10];
	char mul_b[10];
	int a, b;
	long long int sum = 0;
	int status = 0;
	int dont = 0;

	while ((c = fgetc(stdin)) != EOF) {
		if (c == 'd') {
			status = 6;
			continue;
		}
		switch (status) {
			case 6:
				if (c != 'o') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 7:
				if ((c != '(') && (c != 'n')) {
					status = 0;
					continue;
				}
				if (c == '(') {
					status = 12;
					continue;
				}
				status ++;
				continue;
			case 8:
				if (c != '\'') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 9:
				if (c != 't') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 10:
				if (c != '(') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 11:
				if (c != ')') {
					status = 0;
					continue;
				}
				dont = 1;
				status = 0;
				continue;
			case 12:
				if (c != ')') {
					status = 0;
					continue;
				}
				dont = 0;
				status = 0;
				continue;

			case 0:
				if (c != 'm')
					continue;
				status ++;
				continue;
			case 1:
				if (c != 'u') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 2:
				if (c != 'l') {
					status = 0;
					continue;
				}
				status ++;
				continue;
			case 3:
				if (c != '(') {
					status = 0;
					continue;
				}
				status ++;
				len_a = len_b = 0;
				continue;
			case 4:
				if (c == ',') {
					if (len_a == 0) {
						status = 0;
						continue;
					}
					status ++;
					continue;
				}
				if ((c < '0') || (c > '9')) {
					status = 0;
					continue;
				}
				if (len_a == 3) {
					status = 0;
					continue;
				}
				mul_a[len_a++] = c;
				mul_a[len_a] = '\0';
				continue;
			case 5:
				if (c == ')') {
					if (len_b == 0) {
						status = 0;
						continue;
					}
					break;
				}
				if ((c < '0') || (c > '9')) {
					status = 0;
					continue;
				}
				if (len_b == 3) {
					status = 0;
					continue;
				}
				mul_b[len_b++] = c;
				mul_b[len_b] = '\0';
				continue;
			default:
				continue;
		}
		if (dont)
			continue;
		sscanf(mul_a, "%d", &a);
		sscanf(mul_b, "%d", &b);
		sum += a * b;
		status = 0;
	}
	printf("Sum  = %lld\n", sum);
}


void part_2(void)
{
	part_1();
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

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
