#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int first;
	int last;
} rule_t;

rule_t *rules = NULL;
int nb_rules = 0;


typedef struct {
	int *values;
	int  nb_values;
} update_t;


update_t *updates = NULL;
int nb_updates = 0;

int load_rules(void)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {
		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		rules = realloc(rules, (nb_rules + 1) * sizeof(rule_t));
		if (rules == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		nb_rules++;
		if (sscanf(line, "%d|%d", &(rules[nb_rules - 1].first), &(rules[nb_rules - 1].last)) != 2)
			break;
	}
	fprintf(stderr, "Loaded: %d rules\n", nb_rules);
	return 0;
}


int comprules(const void *a, const void *b)
{
	rule_t *ra = (rule_t *)a;
	rule_t *rb = (rule_t *)b;

	return ra->first < rb->first ? -1: ra->first > rb->first ? 1 : 0;
}


void sort_rules(void)
{
	qsort(rules, nb_rules, sizeof(rule_t), comprules);
}



int load_updates(void)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {
		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		updates = realloc(updates, (nb_updates + 1) * sizeof(update_t));
		if (updates == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		updates[nb_updates].values = NULL;
		updates[nb_updates].nb_values = 0;
		nb_updates++;

		int pos = 0;
		int nb = 0;
		int value;

		while (sscanf(&(line[pos]), "%d%n", &value, &nb) == 1) {
			pos += nb;
			updates[nb_updates - 1].values = realloc(updates[nb_updates - 1].values, (updates[nb_updates - 1].nb_values + 1) * sizeof(int));
			updates[nb_updates - 1].values[updates[nb_updates - 1].nb_values] = value;
			updates[nb_updates - 1].nb_values ++;
			if (line[pos] != ',')
				break;
			pos ++;
		}
	}
	fprintf(stderr, "Loaded: %d updates\n", nb_updates);
	return 0;
}



int ordered_update(update_t *up)
{
	for (int i = 1; i < up->nb_values; i ++) {
		int r = 0;
		while (rules[r].first < up->values[i])
			r++;
		while (rules[r].first == up->values[i]) {
			for (int j = 0; j < i; j++)
				if (rules[r].last == up->values[j])
					return 0;
			r++;
		}
	}
	return 1;
}



int middle_of(update_t *up)
{
	return up->values[up->nb_values / 2];
}



int compvalues(const void *va, const void *vb)
{
	int *a = (int *)va;
	int *b = (int *)vb;

	int r = 0;

	while (rules[r].first < *b)
		r++;
	while (rules[r].first == *b) {
		if (rules[r].last == *a)
			return 1;
		r++;
	}
	return -1;
}



void part_1(void)
{
	long long int sum = 0;
	for (int nu = 0; nu < nb_updates; nu++)
		if (ordered_update(&(updates[nu]))) {
			sum += middle_of(&(updates[nu]));
		}

	printf ("Sum += %lld\n", sum);
}



void part_2(void)
{
	long long int sum = 0;

	for (int nu = 0; nu < nb_updates; nu++) {
		if (ordered_update(&(updates[nu])))
			continue;
		qsort(updates[nu].values, updates[nu].nb_values, sizeof(int), compvalues);
		sum += middle_of(&(updates[nu]));
	}
	printf("Sum += %lld\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_rules();
	load_updates();
	sort_rules();

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
