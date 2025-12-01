
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


static int part_1(int argc, char *argv[]);
static int part_2(int argc, char *argv[]);


int main(int argc, char *argv[])
{
	int part;

	if ((argc < 2)
	 || (sscanf(argv[1], "%d", &part) != 1)
	 || (part < 1)
	 || (part > 2)) {
		fprintf(stderr, "usage: %s [1|2]...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	if (part == 1)
		return part_1(argc, argv);

	return part_2(argc, argv);
}



static int part_1(int argc, char *argv[])
{
	FILE *fp;

	if ((argc < 3)
	 || ((fp = fopen(argv[2], "r")) == NULL)) {
		fprintf(stderr, "usage: %s 1 <filename>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char line[256];
	int  position = 50;
	int  count = 0;

	while (fgets(line, 255, fp) != NULL) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		int move;
		if (sscanf(&(line[1]), "%d", &move) != 1) {
			fprintf(stderr, "Wrong line: %s\n", line);
			exit(EXIT_FAILURE);
		}
		if (line[0] == 'L')
			position = position - move;
		else
			position = position + move;
		while (position < 0)
			position += 100;
		while (position > 99)
			position -= 100;
		if (position == 0)
			count ++;

		// fprintf(stderr, "%c %d -> position: %d\n", line[0], move, position);
	}
	fprintf(stdout, "Count: %d\n", count);
	fclose(fp);
	return EXIT_SUCCESS;
}



static int part_2(int argc, char *argv[])
{
	FILE *fp;

	if ((argc < 3)
	 || ((fp = fopen(argv[2], "r")) == NULL)) {
		fprintf(stderr, "usage: %s 2 <filename>...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char line[256];
	int  position = 50;
	int  count = 0;

	while (fgets(line, 255, fp) != NULL) {
		if (line[strlen(line) - 1] == '\n')
			line[strlen(line) - 1] = '\0';
		int move;
		if (sscanf(&(line[1]), "%d", &move) != 1) {
			fprintf(stderr, "Wrong line: %s\n", line);
			exit(EXIT_FAILURE);
		}
		int previous = position;
		if (line[0] == 'L')
			position = position - move;
		else
			position = position + move;

		if (position < 0) {
			while (position < 0) {
				position += 100;
				count ++;
			}
			if (previous == 0)
				count --;
		}

		if (position > 99) {
			while (position > 99) {
				position -= 100;
				count ++;
			}
			if (position == 0)
				count --;
		}

		if (position == 0)
			count ++;

		// fprintf(stderr, "%c %d -> position: %d, count: %d\n", line[0], move, position, count);
	}
	fprintf(stdout, "Count: %d\n", count);
	fclose(fp);
	return EXIT_SUCCESS;
}

