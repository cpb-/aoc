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



typedef struct {
	char   prev;
	char   next;
	int    robot;
	long long int length;
} memo;

memo *Memo = NULL;
int Nb_memo = 0;

long long int check_memoization(char prev, char next, int robot)
{
	for (int n = 0; n < Nb_memo; n++)
		if ((prev == Memo[n].prev) && (next == Memo[n].next) && (robot == Memo[n].robot))
			return Memo[n].length;
	return -1;
}

void memoize(char prev, char next, int robot, long long int length)
{
	memo *m;
	m = realloc(Memo, (Nb_memo + 1) * sizeof(memo));
	if (m == NULL)
		return;
	Memo = m;
	Memo[Nb_memo].prev = prev;
	Memo[Nb_memo].next = next;
	Memo[Nb_memo].robot = robot;
	Memo[Nb_memo].length = length;
	Nb_memo ++;
}




long long int length_of_code(char prev, char next, int robot)
{
	long long int ret = check_memoization(prev, next, robot);
	if (ret >= 0)
		return ret;
	ret = 0;

	char *move = arrowpadmove[arrow_index(prev)][arrow_index(next)];

	if (robot == 1) {
		ret = strlen(move);
	} else {
		ret += length_of_code('A', move[0], robot - 1);
		for (int i = 1; move[i] != '\0'; i++)
			ret += length_of_code(move[i-1], move[i], robot - 1);
	}
	memoize(prev, next, robot, ret);
	return ret;
}



long long int complexity_of_code(const char *code, int nb_robots)
{
	long long int length = 0;
	char prev;

	prev = 'A';
	for (int n = 0; code[n] != '\0'; n++) {
		char *move = doorpadmove[door_index(prev)][door_index(code[n])];
		length += length_of_code('A', move[0], nb_robots);
		for (int m = 1; move[m] != '\0'; m++)
			length += length_of_code(move[m-1], move[m], nb_robots);
		prev = code[n];
	}

	long long int val;
	sscanf(code, "%lld", &val);

	return length * val;
}



void sum_of_complexities(int nb_robots)
{
	long long int sum;

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
}



void part_1(void)
{
	sum_of_complexities(2);
}


void part_2(void)
{
	sum_of_complexities(25);
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


