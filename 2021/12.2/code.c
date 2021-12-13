#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NB_NODES_MAX 16

typedef struct s_node {
	char name[32];
	int big;
	int *links;
	int nb_links;
} node_t;

char seen[NB_NODES_MAX];
int first_small = 0;

node_t *nodes = NULL;
int nb_nodes;

int start_index;
int end_index;
int nb_paths = 0;


void add_link(const char *src, const char *dst);
void add_node(const char *name);
void start_traverse(int idx);
void depth_traverse(int idx);


int main(int argc, char *argv[])
{
	char buffer[4096];

	while (fgets(buffer, 4095, stdin) != NULL) {
		int dash;
		for (dash = 0; (buffer[dash] != '-') && (buffer[dash] != '\0'); dash++)
			;
		buffer[dash] = '\0';
		for (int i = dash + 1; buffer[i] != '\0'; i++) {
			if (!isalpha(buffer[i])) {
				buffer[i] = '\0';
				break;
			}
		}
		add_link(buffer, &(buffer[dash + 1]));
	}
	fprintf(stdout, "Number of nodes = %d\n", nb_nodes);
	for (int i = 0; i < nb_nodes; i++) {
		fprintf(stderr, "%s\n", nodes[i].name);
		for(int j = 0; j < nodes[i].nb_links; j++)
			fprintf(stderr, "  - %s\n", nodes[nodes[i].links[j]].name);
	}

	for (int i = 0; i < nb_nodes; i++) {
		if (strcmp(nodes[i].name, "end") == 0) {
			end_index = i;
			break;
		}
	}
	//fprintf(stdout, "End index = %d\n", end_index);
	for (int i = 0; i < nb_nodes; i++) {
		if (strcmp(nodes[i].name, "start") == 0) {
			start_index = i;
			memset(seen, 0, NB_NODES_MAX);
			start_traverse(i);
			break;
		}
	}
	fprintf(stdout, "Number of paths = %d\n", nb_paths);

	return 0;
}



void add_link(const char *src, const char *dst)
{
	int i;
	int j;

	for (i = 0; i < nb_nodes; i++) {
		if (strcmp(nodes[i].name, src) == 0)
			break;
	}
	if (i == nb_nodes) {
		add_node(src);
	}
	for(j = 0; j < nb_nodes; j++) {
		if (strcmp(nodes[j].name, dst) == 0)
			break;
	}
	if (j == nb_nodes) {
		add_node(dst);
	}
	nodes[i].links = realloc(nodes[i].links, (nodes[i].nb_links + 1) * sizeof(int));
	nodes[i].links[nodes[i].nb_links++] = j;
	nodes[j].links = realloc(nodes[j].links, (nodes[j].nb_links + 1) * sizeof(int));
	nodes[j].links[nodes[j].nb_links++] = i;
}



void add_node(const char *name)
{
	nodes = realloc(nodes, (nb_nodes + 1) * sizeof(node_t));
	strcpy(nodes[nb_nodes].name, name);
	nodes[nb_nodes].big = isupper(name[0]);
	nodes[nb_nodes].links = NULL;
	nodes[nb_nodes].nb_links = 0;
	nb_nodes ++;
}



void start_traverse(int idx)
{
	char saved_seen[NB_NODES_MAX];
	int saved_first;

	fprintf(stdout, "start_traverse(%s)\n", nodes[idx].name);
	if (idx == end_index) {
		fprintf(stdout, "end_traverse(%s)\n", nodes[idx].name);
		nb_paths ++;
		return;
	}

	if (! nodes[idx].big) {
		if (seen[idx]) {
			if (idx == start_index)
				return;
			if (first_small)
				return;
			first_small = 1;
		}
		seen[idx] = 1;
	}

	for (int i = 0; i < nodes[idx].nb_links; i ++) {
		memcpy(saved_seen, seen, NB_NODES_MAX);
		saved_first = first_small;
		start_traverse(nodes[idx].links[i]);
		memcpy(seen, saved_seen, NB_NODES_MAX);
		first_small = saved_first;
	}
}

