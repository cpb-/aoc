#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct {
	int a;
	int b;
	int c;
} rule_t;

rule_t *rules = NULL;
int nb_rules;

#define MAX_ITEM 32

long long int count[MAX_ITEM];

long long  pairs[MAX_ITEM][MAX_ITEM];
long long  new_pairs[MAX_ITEM][MAX_ITEM];

void add_rule(int a, int b, int c);

#define NB_STEPS 40

int main(int argc, char *argv[])
{
	char temp[4096];
	char buffer[4096];

	fgets(temp, 4095, stdin);
	temp[strlen(temp) - 1] = '\0';
	fgets(buffer, 4095, stdin); // blank line

	while (fgets(buffer, 4095, stdin) != NULL) {
		char a, b, c;
		if (sscanf(buffer, "%c%c -> %c", &a, &b, &c) != 3)
			break;
		add_rule(a - 'A', b - 'A', c - 'A');
	}

	memset(count, 0, MAX_ITEM * sizeof(long long int));

	for (int i = 0; i < MAX_ITEM; i ++)
		for (int j = 0; j < MAX_ITEM; j ++)
			pairs[i][j] = 0;

	for (int i = 0; isalpha(temp[i]); i++) {
		count[temp[i] - 'A'] ++;
	}

	for (int i = 0; isalpha(temp[i+1]); i++) {
		pairs[temp[i] - 'A'][temp[i+1] - 'A'] ++;
	}

	int step;
	for (step = 0; step < NB_STEPS; step ++) {
		fprintf(stderr, "step %d\n", step);
		for (int i = 0; i < MAX_ITEM; i ++) {
			if (count[i] != 0)
				fprintf(stderr, "  count[%c] = %lld\n", i + 'A', count[i]);
		}


		memcpy(new_pairs, pairs, MAX_ITEM * MAX_ITEM * sizeof(long long int));
		for (int r = 0; r < nb_rules; r++) {
			new_pairs[rules[r].a][rules[r].c] += pairs[rules[r].a][rules[r].b];
			new_pairs[rules[r].c][rules[r].b] += pairs[rules[r].a][rules[r].b];
			new_pairs[rules[r].a][rules[r].b] -= pairs[rules[r].a][rules[r].b];
			count[rules[r].c] += pairs[rules[r].a][rules[r].b];
		}
		memcpy(pairs, new_pairs, MAX_ITEM * MAX_ITEM * sizeof(long long int));
	}
	fprintf(stderr, "step %d\n", step);
	for (int i = 0; i < MAX_ITEM; i ++) {
		if (count[i] != 0)
			fprintf(stderr, "  count[%c] = %lld\n", i + 'A', count[i]);
	}

	return 0;
}


void add_rule(int a, int b, int c)
{
	rules = realloc(rules, (nb_rules + 1) * sizeof(rule_t));
	rules[nb_rules].a = a;
	rules[nb_rules].b = b;
	rules[nb_rules].c = c;
	nb_rules ++;
}


