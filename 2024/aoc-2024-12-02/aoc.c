#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int safe_set(int *table, int nb, int removed)
{
	int up = -1;
	int last = -1;

	for (int i = 0; i < nb; i ++) {
		if (i == removed)
			continue;
		if (last == -1) {
			last = table[i];
			continue;
		}
		if (up == -1) {
			if (last < table[i])
				up = 1;
			else
				up = 0;
		}
		if (up) {
			if ((table[i] - last < 1) || (table[i] - last > 3))
				return 0;
		} else {
			if ((last - table[i] < 1) || (last - table[i] > 3))
				return 0;
		}
		last = table[i];
	}
	return 1;
}



int safe_report_1(const char *line)
{
	int pos = 0;
	int last = -1;
	int next = 0;
	int up = -1;
	int n;

	for (;;) {
		if (sscanf(&(line[pos]), "%d %n", &next, &n) != 1)
			return 1;
		pos += n;
		if (last == -1) {
			last = next;
			continue;
		}
		if (up == -1) {
			if (next > last)
				up = 1;
			else
				up = 0;
		}
		if (up) {
			if ((next - last < 1) || (next - last > 3)) {
				return 0;
			}
		} else {
			if ((last - next < 1) || (last - next > 3)) {
				return 0;
			}
		}
		last = next;
	}
}


int safe_report_2(const char *line)
{
	int pos = 0;
	int nb;
	int table[256];


	for (nb = 0; nb < 1024; nb++) {
		int n;
		if (sscanf(&(line[pos]), "%d %n", &table[nb], &n) != 1)
			break;
		pos += n;
	}

	if (safe_set(table, nb, -1))
		return 1;
	for (int i = 0; i < nb; i++)
		if (safe_set(table, nb, i))
			return 1;
	return 0;
}



void part_1(void)
{
	char line[128];
	int count = 0;

	while (fgets(line, 127, stdin) != NULL) {
		count += safe_report_1(line);
	}
	printf("Count = %d\n", count);
}


void part_2(void)
{
	char line[128];
	int count = 0;

	while (fgets(line, 127, stdin) != NULL) {
		count += safe_report_2(line);
	}
	printf("Count = %d\n", count);
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
