#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	char from[3];
	char to[3];
} link_t;


link_t *links = NULL;
int nb_links = 0;


int load_links(void)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {
		links = realloc(links, (nb_links + 1) * sizeof(link_t));
		links[nb_links].from[0] = line[0];
		links[nb_links].from[1] = line[1];
		links[nb_links].from[2] = '\0';
		links[nb_links].to[0] = line[3];
		links[nb_links].to[1] = line[4];
		links[nb_links].to[2] = '\0';
		nb_links++;
	}
	return 0;
}


typedef struct {
	char name[3];
	int  active;
	int *neighbours;
	int  nb_neighbours;
	int  nb_connections;
} node_t;

node_t *nodes = NULL;
int nb_nodes = 0;


int get_node_index(const char *name)
{
	for (int i = 0; i < nb_nodes; i++)
		if (strcmp(nodes[i].name, name) == 0)
			return i;
	nodes = realloc(nodes, (nb_nodes + 1) * sizeof(node_t));
	strcpy(nodes[nb_nodes].name, name);
	nodes[nb_nodes].neighbours = NULL;
	nodes[nb_nodes].nb_neighbours = 0;
	nb_nodes ++;
	return nb_nodes - 1;
}


void add_neighbour_to(int a, int b)
{
	for (int i = 0; i < nodes[a].nb_neighbours; i++)
		if (nodes[a].neighbours[i] == b)
			return;

	nodes[a].neighbours = realloc(nodes[a].neighbours, (nodes[a].nb_neighbours + 1) * sizeof(int));
	nodes[a].neighbours[nodes[a].nb_neighbours] = b;
	nodes[a].nb_neighbours ++;

	nodes[b].neighbours = realloc(nodes[b].neighbours, (nodes[b].nb_neighbours + 1) * sizeof(int));
	nodes[b].neighbours[nodes[b].nb_neighbours] = a;
	nodes[b].nb_neighbours ++;
}


int is_neighbour_of(int a, int b)
{
	for (int i = 0; i < nodes[b].nb_neighbours; i++)
		if (nodes[b].neighbours[i] == a)
			return 1;
	return 0;
}


void build_node_graph(void)
{
	for (int i = 0; i < nb_links; i++) {
		int n1 = get_node_index(links[i].from);
		int n2 = get_node_index(links[i].to);
		add_neighbour_to(n1, n2);
	}
	printf("Nb nodes: %d, nb_links: %d\n", nb_nodes, nb_links);
}


void part_1(void)
{
	long int count = 0;

	for (int i = 0; i < nb_nodes - 2; i++) {
		for (int j = i + 1; j < nb_nodes - 1; j++) {
			if (! is_neighbour_of(i, j))
				continue;
			for (int k = j + 1; k < nb_nodes; k++) {
				if (! is_neighbour_of(i, k))
					continue;
				if (! is_neighbour_of(j, k))
					continue;
				if ((nodes[i].name[0] == 't')
				 || (nodes[j].name[0] == 't')
				 || (nodes[k].name[0] == 't')) {
					//fprintf(stderr, "(%s, %s, %s)", nodes[i].name, nodes[j].name, nodes[k].name);
					count ++;
				}
			}
		}
	}
	printf("Count = %ld\n", count);
}



int fully_connected(void)
{
	for (int i = 0; i < nb_nodes - 1; i++) {
		if (! nodes[i].active)
			continue;
		for (int j = i + 1; j < nb_nodes; j++) {
			if (! nodes[j].active)
				continue;
			if (! is_neighbour_of(i, j))
				return 0;
		}
	}
	return 1;
}


void part_2(void)
{
	for (int i = 0; i < nb_nodes; i++) {
		nodes[i].nb_connections = 0;
		for (int j = 0; j < nodes[i].nb_neighbours - 1; j++)
			for (int k = j + 1; k < nodes[i].nb_neighbours; k++)
				if (is_neighbour_of(nodes[i].neighbours[j], nodes[i].neighbours[k]))
					nodes[i].nb_connections ++;
	}
	int max = 0;
	for (int i = 0; i < nb_nodes; i++) {
		if (nodes[i].nb_connections > max)
			max = nodes[i].nb_connections;
	}
	int *selected = malloc(nb_nodes * sizeof(int));
	int nb_selected = 0;

	for (int i = 0; i < nb_nodes; i++) {
		if (nodes[i].nb_connections == max)
			selected[nb_selected++] = i;
	}


	for (int i = 0; i < nb_nodes; i++) {
		nodes[i].nb_connections = 0;
	}
	for (int i = 0; i < nb_selected; i++) {
		for (int j = 0; j < nodes[selected[i]].nb_neighbours; j++) {
			for (int k = 0; k < nb_selected; k++) {
				if (selected[k] == nodes[selected[i]].neighbours[j]) {
					nodes[selected[i]].nb_connections ++;
					break;
				}
			}
		}
	}
	max = 0;
	for (int i = 0; i < nb_nodes; i++) {
		if (nodes[i].nb_connections > max)
			max = nodes[i].nb_connections;
	}
	for (int i = 0; i < nb_nodes; i++) {
		if (nodes[i].nb_connections == max)
			printf("%s ", nodes[i].name);
	}

	printf("\n\n\n");
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}


	load_links();
	build_node_graph();

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
