#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char *doorpadmove[11][11] = {
/*              To:    0        1       2        3        4       5       6         7        8        9        A    */
/* From 0 */	{     "A",   "^<A",   "^A",   "^>A",  "^^<A",  "^^A", "^^>A",  "^^^<A",  "^^^A", "^^^>A",     ">A"  },
/* From 1 */	{   ">vA",     "A",   ">A",   ">>A",    "^A",  "^>A", "^>>A",    "^^A",  "^^>A", "^^>>A",   ">>vA"  },
/* From 2 */	{    "vA",    "<A",    "A",    ">A",   "<^A",   "^A",  "^>A",   "<^^A",   "^^A",  "^^>A",    "v>A"  },
/* From 3 */	{   "<vA",   "<<A",   "<A",     "A",  "<<^A",  "<^A",   "^A",  "<<^^A",  "<^^A",   "^^A",     "vA"  },
/* From 4 */	{  ">vvA",    "vA",  "v>A",  "v>>A",     "A",   ">A",  ">>A",     "^A",   "^>A",  "^>>A",  ">>vvA"  },
/* From 5 */	{   "vvA",   "<vA",   "vA",   "v>A",    "<A",    "A",   ">A",    "<^A",    "^A",   "^>A",   "vv>A"  },
/* From 6 */	{  "<vvA",  "<<vA",  "<vA",    "vA",   "<<A" ,  "<A",    "A",   "<<^A",   "<^A",    "^A",    "vvA"  },
/* From 7 */	{ ">vvvA",   "vvA", "vv>A", "vv>>A",    "vA",  "v>A", "v>>A",      "A",    ">A",   ">>A", ">>vvvA"  },
/* From 8 */	{  "vvvA",  "<vvA",  "vvA",  "vv>A",   "<vA",   "vA",  "v>A",     "<A",     "A",    ">A",  "vvv>A"  },
/* From 9 */	{ "<vvvA", "<<vvA", "<vvA",   "vvA",  "<<vA",  "<vA",   "vA",    "<<A",    "<A",     "A",   "vvvA"  },
/* From A */	{    "<A",  "^<<A",  "<^A",    "^A", "^^<<A", "<^^A",  "^^A", "^^^<<A", "<^^^A",  "^^^A",      "A"  }
};


char *arrowpadmove[5][5] = {
/*              To:    <        v       >       ^       A    */
/* From <*/     {     "A",    ">A",  ">>A",  ">^A", ">>^A"   },
/* From v*/     {    "<A",     "A",   ">A",   "^A",  "^>A"   },
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



#define LG_MAX (1024*1024)
char *moves[2];



long long int complexity_of_code(const char *code, int nb_robots)
{

	moves[1][0] = '\0';
	char prev = 'A';
	for (int n = 0; code[n] != '\0'; n++) {
		strcat(moves[1], doorpadmove[door_index(prev)][door_index(code[n])]);
		prev = code[n];
	}

	memmove(moves[0], moves[1], LG_MAX-1);

	for (int r = 0; r < nb_robots; r++) {
		moves[1][0] = '\0';
		prev = 'A';
		int pos = 0;
		for (int n = 0; moves[0][n] != '\0'; n++) {
			int from = arrow_index(prev);
			int to = arrow_index(moves[0][n]);
			strcpy(&(moves[1][pos]), arrowpadmove[from][to]);
			pos += strlen(&(moves[1][pos]));
			prev = moves[0][n];
		}
		memmove(moves[0], moves[1], LG_MAX-1);
	}


	long long int val;
	long long int l = strlen(moves[0]);

	sscanf(code, "%lld", &val);

	//fprintf(stderr, "%s: %lld x %lld = %lld\n", code, l, val, l*val);
	return l * val;
}



void sum_of_complexities(int nb_robots)
{
	long long int sum;

	moves[0] = malloc(LG_MAX);
	if (moves[0] == NULL) {
		perror("moves 0");
		exit(1);
	}
	moves[1] = malloc(LG_MAX);
	if (moves[1] == NULL) {
		perror("moves ");
		exit(1);
	}

	sum = 0;
	sum += complexity_of_code("029A", nb_robots);
	sum += complexity_of_code("980A", nb_robots);
	sum += complexity_of_code("179A", nb_robots);
	sum += complexity_of_code("456A", nb_robots);
	sum += complexity_of_code("379A", nb_robots);
	printf("Example Sum = %lld\n\n", sum);

	sum = 0;
	sum += complexity_of_code("789A", nb_robots);
	sum += complexity_of_code("968A", nb_robots);
	sum += complexity_of_code("286A", nb_robots);
	sum += complexity_of_code("349A", nb_robots);
	sum += complexity_of_code("170A", nb_robots);
	printf("Input data sum = %lld\n\n", sum);

	free(moves[1]);
	free(moves[0]);
}



void part_1(void)
{
	sum_of_complexities(2);
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


