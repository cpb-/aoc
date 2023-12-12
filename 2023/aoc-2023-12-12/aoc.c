#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char *extract_pattern(const char *line)
{
	int idx = 0;
	char *pattern = NULL;

	while (line[idx] != ' ')
		idx++;

	pattern = malloc(idx + 1);
	memcpy(pattern, line, idx);
	pattern[idx] = '\0';

	return pattern;
}



int *extract_sequence(const char *line)
{
	int *sequence = NULL;
	int nb_numbers = 0;
	int number;
	int n;
	int offset = 0;

	while (line[offset] != ' ')
		offset++;
	offset++;

	while (sscanf(&(line[offset]), "%d%n", &number, &n) == 1) {
		sequence = realloc(sequence, (nb_numbers + 1) * sizeof(int));
		sequence[nb_numbers] = number;
		nb_numbers ++;
		offset += n;
		if (line[offset] != ',')
			break;
		offset ++;
	}

	sequence = realloc(sequence, (nb_numbers + 1) * sizeof(int));
	sequence[nb_numbers] = 0;

	return sequence;
}



typedef struct {
	const char *a;
	int  *b;
	int   c;
	long long int r;
} memo;


memo *Memo = NULL;
int Nb_memo = 0;


long long int check_memoization(const char *a, int *b, int c)
{
	for (int n = 0; n < Nb_memo; n++)
		if ((Memo[n].a == a) && (Memo[n].b == b) && (Memo[n].c == c))
			return Memo[n].r;
	return -1;
}



void memoize(const char *a, int *b, int c, long long int r)
{
	memo *m;
	m = realloc(Memo, (Nb_memo + 1) * sizeof(memo));
	if (m == NULL)
		return;
	Memo = m;
	Memo[Nb_memo].a = a;
	Memo[Nb_memo].b = b;
	Memo[Nb_memo].c = c;
	Memo[Nb_memo].r = r;
	Nb_memo ++;
}



void clear_memoization(void)
{
	free(Memo);
	Memo = NULL;
	Nb_memo = 0;
}



long long int compute_patterns_matching (const char *pattern, int *sequence, int current)
{
	long long int count = 0;

	int remaining = 0;
	int hash_count = 0;


	count = check_memoization(pattern, sequence, current);
	if (count >= 0)
		return count;
	count = 0;

	if (pattern[0] == '\0') {
		// No more sequences: Ok
		if (sequence[0] == 0) {
			memoize(pattern, sequence, current, 1);
			return 1;
		}
		// Last sequence just terminated: Ok
		if ((current == sequence[0]) && (sequence[1] == 0)) {
			memoize(pattern, sequence, current, 1);
			return 1;
		}
		// Still more sequences: impossible.
		memoize(pattern, sequence, current, 0);
		return 0;
	}

	for (int i = 0; sequence[i] != 0; i++)
		remaining += sequence[i];
	remaining -= current;

	// Not enough remaining `#` and `?`: impossible.
	for (int i = 0; pattern[i] != '\0'; i++)
		hash_count += (pattern[i] != '.');
	if (hash_count < remaining) {
		memoize(pattern, sequence, current, 0);
		return 0;
	}

	// Too many remaining `#`: impossible.
	hash_count = 0;
	for (int i = 0; pattern[i] != '\0'; i++)
		hash_count += (pattern[i] == '#');
	if (hash_count > remaining) {
		memoize(pattern, sequence, current, 0);
		return 0;
	}

	if ((pattern[0] == '.') && (current != 0) && (sequence[0] != current)) {
		// Sequence in progress not terminated: impossible.
		memoize(pattern, sequence, current, 0);
		return 0;
	}

	if ((pattern[0] == '#') && (sequence[0] < current + 1)) {
		// Current sequence too short: impossible.
		memoize(pattern, sequence, current, 0);
		return 0;
	}

	if (pattern[0] == '.') {
		if (current != 0) {
			count += compute_patterns_matching(pattern + 1, sequence + 1, 0);
		} else {
			count += compute_patterns_matching(pattern + 1, sequence, 0);
		}
	}

	if (pattern[0] == '#') {
		count += compute_patterns_matching(pattern + 1, sequence, current + 1);
	}

	if (pattern[0] == '?') {
		// .
		if (current != 0) {
			if (sequence[0] == current)
				count += compute_patterns_matching(pattern + 1, sequence + 1, 0);
		} else {
			count += compute_patterns_matching(pattern + 1, sequence, 0);
		}
		// #
		if (sequence[0] >= current + 1) {
			count += compute_patterns_matching(pattern + 1, sequence, current + 1);
		}
	}

	memoize(pattern, sequence, current, count);
	return count;
}



long long int compute_arrangements(const char *line, int part)
{
	char *pattern = extract_pattern(line);
	int  *sequence = extract_sequence(line);
	long long int count;
	//static int linenum = 0;

	if (part == 2) {
		char *p = malloc((strlen(pattern) + 1) * 5);
		for (int i = 0; i < 5; i ++) {
			memcpy(p + (strlen(pattern) + 1) * i, pattern, strlen(pattern));
			p[(strlen(pattern) + 1) * i + strlen(pattern)] = '?';
		}
		p[(strlen(pattern) + 1) * 5 - 1] = '\0';
		free(pattern);
		pattern = p;

		int n;
		for (n = 0; sequence[n] != 0; n ++)
			;
		int *s = malloc(((n * 5) + 1) * sizeof(int));
		for (int i = 0; i < 5; i ++)
			for (int j = 0; j < n; j++)
				s[i * n + j] = sequence[j];
		s[n * 5] = 0;
		free(sequence);
		sequence = s;

	}

	//fprintf(stderr, "[%d] Checking %s vs  ", 1 + linenum++, pattern);
	//for (int i = 0; sequence[i] != 0; i++)
	//	fprintf(stderr, "%d ", sequence[i]);

	count = compute_patterns_matching(pattern, sequence, 0);

	//fprintf(stderr, "  --> %lld\n", count);

	free(sequence);
	free(pattern);

	clear_memoization();

	return count;
}



char Lines[1024][1024];
int Nb_lines = 0;

void read_lines(void)
{
	while (fgets(Lines[Nb_lines], 1024, stdin) != NULL)
		Nb_lines++;
}



void part_func(int part_num)
{
	long long int sum = 0;

	read_lines();

	for (int i = 0; i < Nb_lines; i ++)
		sum += compute_arrangements(Lines[i], part_num);

	printf("   Sum = %lld\n", sum);
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
			part_func(1);
			break;
		case 2:
			part_func(2);
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
