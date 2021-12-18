#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 4096

typedef struct s_pair {
	int type[2];
		#define INT_TYPE  1
		#define PAIR_TYPE 2
	int intval[2];
	struct s_pair *pairval[2];
	struct s_pair *parent;
} pair_t;

pair_t * read_pair (const char *buffer, int *start, pair_t *parent);

void reduce(pair_t *pair);
void display(pair_t *pair);
pair_t *add_pair(pair_t *a, pair_t *b);
long long int magnitude(pair_t *pair);

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	pair_t *pair = NULL;

	while (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		int start = 0;
		pair_t *new_pair = NULL;
		new_pair = read_pair(buffer, &start, NULL);
		if (pair != NULL) {
			pair = add_pair(pair, new_pair);
		} else {
			pair = new_pair;
		}
		reduce(pair);
	}
	display(pair);
	fprintf(stdout, "Magnitude = %lld\n", magnitude(pair));
	return 0;
}

pair_t *nested_level(pair_t *tree, int level);
pair_t *greater_than_10(pair_t *tree);
void explode(pair_t *tree, pair_t *pair);
void split(pair_t *pair);
int replace_by_zero(pair_t *tree, pair_t *pair);
int add_to_first_left_value(pair_t *node, pair_t *from, int value);
int add_to_first_right_value(pair_t *node, pair_t *from, int value);


long long int magnitude(pair_t *pair)
{
	long long int ret = 0;

	if (pair->type[0] == INT_TYPE) {
		ret = 3 * pair->intval[0];
	} else {
		ret = 3 * magnitude(pair->pairval[0]);
	}

	if (pair->type[1] == INT_TYPE) {
		ret += 2 * pair->intval[1];
	} else {
		ret += 2 * magnitude(pair->pairval[1]);
	}

	return ret;
}

void reduce(pair_t *pair)
{
	pair_t *p;

	while (1) {
		p = nested_level(pair, 4);
		if (p != NULL) {
			explode(pair, p);
			continue;
		}
		p = greater_than_10(pair);
		if (p != NULL) {
			split(p);
			continue;
		}
		break;
	}
}

pair_t *add_pair(pair_t *a, pair_t *b)
{
	pair_t *n = malloc(sizeof(pair_t));

	n->parent = NULL;

	n->type[0] = PAIR_TYPE;
	n->pairval[0] = a;
	a->parent = n;

	n->type[1] = PAIR_TYPE;
	n->pairval[1] = b;
	b->parent = n;

	return n;
}


void display(pair_t *pair)
{
	fprintf(stdout, "[");
	if (pair->type[0] == PAIR_TYPE) {
		display(pair->pairval[0]);
	} else {
		fprintf(stdout, "%d", pair->intval[0]);
	}
	fprintf(stdout, ",");
	if (pair->type[1] == PAIR_TYPE) {
		display(pair->pairval[1]);
	} else {
		fprintf(stdout, "%d", pair->intval[1]);
	}
	fprintf(stdout, "]");
}


pair_t *nested_level(pair_t *tree, int level)
{
	pair_t *p;

	if (level == 0)
		return tree;
	if (tree->type[0] == PAIR_TYPE) {
		p = nested_level(tree->pairval[0], level - 1);
		if (p != NULL)
			return p;
	}
	if (tree->type[1] == PAIR_TYPE) {
		p = nested_level(tree->pairval[1], level - 1);
		if (p != NULL)
			return p;
	}
	return NULL;
}

pair_t *greater_than_10(pair_t *node)
{
	pair_t *p;

	if (node != NULL) {

		if (node->type[0] == INT_TYPE) {
			if (node->intval[0] >= 10) {
				return node;
			}
		} else {
			p = greater_than_10(node->pairval[0]);
			if (p != NULL)
				return p;
		}

		if (node->type[1] == INT_TYPE) {
			if (node->intval[1] >= 10)
				return node;
		} else {
			p = greater_than_10(node->pairval[1]);
			if (p != NULL)
				return p;
		}
	}
	return NULL;
}


void explode(pair_t *tree, pair_t *pair)
{
	add_to_first_left_value(pair->parent, pair, pair->intval[0]);
	add_to_first_right_value(pair->parent, pair, pair->intval[1]);
	replace_by_zero(tree, pair);
}


int replace_by_zero(pair_t *tree, pair_t *pair)
{
	if (tree->type[0] == PAIR_TYPE) {
		if (tree->pairval[0] == pair) {
			tree->type[0] = INT_TYPE;
			tree->intval[0] = 0;
			return 1;
		}
		if (replace_by_zero(tree->pairval[0], pair))
			return 1;
	}
	if (tree->type[1] == PAIR_TYPE) {
		if (tree->pairval[1] == pair) {
			tree->type[1] = INT_TYPE;
			tree->intval[1] = 0;
			return 1;
		}
		if (replace_by_zero(tree->pairval[1], pair))
			return 1;
	}
	return 0;
}


int add_to_first_left_value(pair_t *node, pair_t *from, int value)
{
	if (node == NULL)
		return 0;

	if (from == node->parent) {
		if (node->type[1] == INT_TYPE) {
			node->intval[1] += value;
			return 1;
		}
		if (add_to_first_left_value(node->pairval[1], node, value))
			return 1;

		if (node->type[0] == INT_TYPE) {
			node->intval[0] += value;
			return 1;
		}
		if (add_to_first_left_value(node->pairval[0], node, value))
			return 1;
		return 0;
	}

	if ((node->type[1] == PAIR_TYPE) && (from == node->pairval[1])) {
		if (node->type[0] == INT_TYPE) {
			node->intval[0] += value;
			return 1;
		}
		if (add_to_first_left_value(node->pairval[0], node, value))
			return 1;
	}

	return add_to_first_left_value(node->parent, node, value);
}



int add_to_first_right_value(pair_t *node, pair_t *from, int value)
{
	if (node == NULL)
		return 0;

	if (from == node->parent) {

		if (node->type[0] == INT_TYPE) {
			node->intval[0] += value;
			return 1;
		}
		if (add_to_first_right_value(node->pairval[0], node, value))
			return 1;
		return 0;

		if (node->type[1] == INT_TYPE) {
			node->intval[1] += value;
			return 1;
		}
		if (add_to_first_right_value(node->pairval[1], node, value))
			return 1;
	}

	if ((node->type[0] == PAIR_TYPE) && (from == node->pairval[0])) {
		if (node->type[1] == INT_TYPE) {
			node->intval[1] += value;
			return 1;
		}
		if (add_to_first_right_value(node->pairval[1], node, value))
			return 1;
	}

	return add_to_first_right_value(node->parent, node, value);
}



void split(pair_t *pair)
{
	pair_t *newpair = malloc(sizeof(pair_t));
	newpair->parent = pair;


	if ((pair->type[0] == INT_TYPE) && (pair->intval[0] >= 10)) {
		pair->type[0] = PAIR_TYPE;
		pair->pairval[0] = newpair;
		newpair->type[0] = newpair->type[1] = INT_TYPE;
		newpair->intval[0] = (pair->intval[0] / 2);
		newpair->intval[1] = ((pair->intval[0] + 1)/ 2);
		return;
	}
	if ((pair->type[1] == INT_TYPE) && (pair->intval[1] >= 10)) {
		pair->type[1] = PAIR_TYPE;
		pair->pairval[1] = newpair;
		newpair->type[0] = newpair->type[1] = INT_TYPE;
		newpair->intval[0] = (pair->intval[1] / 2);
		newpair->intval[1] = ((pair->intval[1] + 1)/ 2);
		return;
	}
}





pair_t * read_pair (const char *buffer, int *start, pair_t *parent)
{

	pair_t *pair = NULL;

	if (buffer[*(start)] != '[') {
		return NULL;
	}
	(*start) ++;

	pair = malloc(sizeof(pair_t));
	pair->parent = parent;

	if (isdigit(buffer[(*start)])) {
		pair->type[0] = INT_TYPE;
		pair->intval[0] = buffer[*start] - '0';
		(*start) ++;
	} else {
		pair->type[0] = PAIR_TYPE;
		pair->pairval[0] = read_pair(buffer, start, pair);
	}

	if (buffer[*(start)] != ',') {
		return NULL;
	}
	(*start) ++;

	if (isdigit(buffer[(*start)])) {
		pair->type[1] = INT_TYPE;
		pair->intval[1] = buffer[*start] - '0';
		(*start) ++;
	} else {
		pair->type[1] = PAIR_TYPE;
		pair->pairval[1] = read_pair(buffer, start, pair);
	}

	if (buffer[*(start)] != ']') {
		return NULL;
	}
	(*start) ++;

	return pair;
}
