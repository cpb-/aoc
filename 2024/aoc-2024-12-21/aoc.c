#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define WIDTH 16
#define HEIGHT 16

char doorpad[WIDTH][HEIGHT]  = { { '#', '#', '#', '#', '#' } , { '#', '7', '8', '9' , '#' }, { '#', '4', '5', '6', '#' }, { '#', '1', '2', '3', '#' }, { '#', '#', '0', 'A', '#' }, { '#', '#', '#', '#', '#' } };
char robotpad[WIDTH][HEIGHT] = { { '#', '#', '#', '#', '#' } , { '#', '#', '^', 'A' , '#' }, { '#', '<', 'v', '>', '#' }, { '#', '#', '#', '#', '#' } };

char *doorpadmove[11][11] = {
/*              To:    0        1       2        3        4       5       6         7        8        9        A    */
/* From 0 */	{     "A",   "^<A",   "^A",   "^>A",  "^^<A",  "^^A", "^^>A",  "^^^<A",  "^^^A", "^^^>A",     ">A"  },
/* From 1 */	{   ">vA",     "A",   ">A",   ">>A",    "^A",  "^>A", "^>>A",    "^^A",  "^^>A", "^^>>A",   ">>vA"  },
/* From 2 */	{    "vA",    "<A",    "A",    ">A",   "^<A",   "^A",  "^>A",   "^^<A",   "^^A",  ">^^A",    ">vA"  },
/* From 3 */	{   "v<A",   "<<A",   "<A",     "A",  "^<<A",  "^<A",   "^A",  "^^<<A",  "^^<A",   "^^A",     "vA"  },
/* From 4 */	{  ">vvA",    "vA",  ">vA",  ">>vA",     "A",   ">A",  ">>A",     "^A",   "^>A",  "^>>A",  ">>vvA"  },
/* From 5 */	{   "vvA",   "<vA",   "vA",   ">vA",    "<A",    "A",   ">A",    "^<A",    "^A",   "^>A",   ">vvA"  },
/* From 6 */	{  "<vvA",  "<<vA",  "<vA",    "vA",   "<<A" ,  "<A",    "A",   "^<<A",   "^<A",    "^A",    "vvA"  },
/* From 7 */	{ ">vvvA",   "vvA", ">vvA", ">>vvA",    "vA",  ">vA", ">>vA",      "A",    ">A",   ">>A", ">>vvvA"  },
/* From 8 */	{  "vvvA",  "<vvA",  "vvA",  ">vvA",   "<vA",   "vA",  ">vA",     "<A",     "A",    ">A",  ">vvvA"  },
/* From 9 */	{ "<vvvA", "<<vvA", "<vvA",   "vvA",  "<<vA",  "<vA",   "vA",    "<<A",    "<A",     "A",   "vvvA"  },
/* From A */	{    "<A",  "^<<A",  "^<A",    "^A", "^^<<A", "^^<A",  "^^A", "^^^<<A", "^^^<A",  "^^^A",      "A"  }
};

char *arrowpadmove[5][5] = {
/*              To:    <        v       >       ^       A    */
/* From <*/     {     "A",    ">A",  ">>A",  ">^A", ">>^A"   },
/* From v*/     {    "<A",     "A",   ">A",   "^A",  ">^A"   },
/* From >*/     {   "<<A",    "<A",    "A",  "<^A",   "^A"   },
/* From ^*/     {   "v<A",    "vA",  "v>A",    "A",   ">A"   },
/* From A*/     {  "v<<A",   "<vA",   "vA",   "<A",    "A"   }
};

int door_index(char c)
{
	if (c == 'A')
		return 10;
	return c - '0';
}


int arrow_index(char c)
{
	switch(c) {
		case '<': return 0;
		case 'v': return 1;
		case '>': return 2;
		case '^': return 3;
		default: return 4;
	}
}


long long int complexity_of_code(const char *code)
{
	char doormove[1024];
	doormove[0] = '\0';

	char prev = 'A';
	for (int n = 0; code[n] != '\0'; n++) {
		strcat(doormove, doorpadmove[door_index(prev)][door_index(code[n])]);
		prev = code[n];
	}
	printf("Door: %s (%ld)\n", doormove, strlen(doormove));



	char arrowmove1[1024];
	arrowmove1[0] = '\0';

	prev = 'A';
	for (int n = 0; doormove[n] != '\0'; n++) {
		int from = arrow_index(prev);
		int to = arrow_index(doormove[n]);
		strcat(arrowmove1, arrowpadmove[from][to]);
		prev = doormove[n];
	}
	printf("Arrow 1: %s (%ld)\n", arrowmove1, strlen(arrowmove1));



	char arrowmove2[1024];
	arrowmove2[0] = '\0';

	prev = 'A';
	for (int n = 0; arrowmove1[n] != '\0'; n++) {
		int from = arrow_index(prev);
		int to = arrow_index(arrowmove1[n]);
		strcat(arrowmove2, arrowpadmove[from][to]);
		prev = arrowmove1[n];
	}
	printf("Arrow 2: %s (%ld)\n", arrowmove2, strlen(arrowmove2));

	int val;
	long int l = strlen(arrowmove2);

	sscanf(code, "%d", &val);
	fprintf(stderr, "ret = %ld x %d = %ld\n", l, val, l*val);
	return l * val;


	return 0;

}


void part_1(void)
{
	long long int sum;

	printf("Example 1:\n\n");
	sum = 0;
	sum += complexity_of_code("029A");
	sum += complexity_of_code("980A");
	sum += complexity_of_code("179A");
	sum += complexity_of_code("456A");
	sum += complexity_of_code("379A");
	printf("Sum = %lld\n\n", sum);

	printf("Input:\n\n");
	sum = 0;
	sum += complexity_of_code("789A");
	sum += complexity_of_code("968A");
	sum += complexity_of_code("286A");
	sum += complexity_of_code("349A");
	sum += complexity_of_code("170A");
	printf("Sum = %lld\n\n", sum);
}


void part_2(void)
{
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
