#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_MAX_X 500
#define MAX_MAX_Y 500

int cells[MAX_MAX_X][MAX_MAX_Y];
int visited[MAX_MAX_X][MAX_MAX_Y];
int risk[MAX_MAX_X][MAX_MAX_Y];
int xmax=MAX_MAX_X;
int ymax=MAX_MAX_Y;

typedef struct s_node {
	int x;
	int y;
	int risk;
} node_t;

node_t *nodes = NULL;
int nb_nodes;

void insert_node(int x, int y, int risk);
node_t *extract_smallest_node(void);
void examine(node_t *nd);

int main(int argc, char *argv[])
{
	char buffer[4096];

	int x = 0;
	int y = 0;

	node_t *nd;

	while ((y < ymax) && (fgets(buffer, 4095, stdin) != NULL)) {
		for (x = 0; (x < xmax) && (isdigit(buffer[x])); x++) {
			cells[x][y] = buffer[x] - '0';
			visited[x][y] = 0;
			risk[x][y] = -1;
		}
		y++;
	}
	xmax = x;
	ymax = y;

	for (int rx = 0; rx < 5; rx++)
		for (int ry = 0; ry < 5; ry++)
			for (x = 0; x < xmax; x++)
				for (y = 0; y < ymax; y++) {
					cells[x + rx * xmax][y + ry * ymax] = cells[x][y] + rx + ry;
					if (cells[x + rx * xmax][y + ry * ymax] > 9)
						cells[x + rx * xmax][y + ry * ymax] -= 9;
					visited[x + rx * xmax][y + ry * ymax] = 0;
					risk[x + rx * xmax][y + ry * ymax] = -1;
				}

	xmax *= 5;
	ymax *= 5;

	fprintf(stderr, "xmax = %d, ymax = %d\n", xmax, ymax);

	for (y = 0; y < ymax; y++) {
		for (x = 0; x < xmax; x++) {
			fprintf(stderr, "%d", cells[x][y]);
		}
		fprintf(stderr, "\n");
	}

	insert_node(0, 0, 0);

	while ((nd = extract_smallest_node()) != NULL) {
		examine(nd);
		free(nd);
	}


	fprintf(stdout, "end\n");

	return 0;
}


void insert_node(int x, int y, int risk)
{
	if (visited[x][y])
		return;
	visited[x][y] = 1;

//	fprintf(stderr, "SET: "); for (int i = 0; i < nb_nodes; i++) fprintf(stderr, "(%d,%d)", nodes[i].x, nodes[i].y); fprintf(stderr, "\n");
//	fprintf(stderr, "Insert (%d, %d, %d)\n", x, y, risk);

	nodes = realloc(nodes, (nb_nodes + 1) * sizeof(node_t));
	nodes[nb_nodes].x = x;
	nodes[nb_nodes].y = y;
	nodes[nb_nodes].risk = risk;
	nb_nodes++;

//	fprintf(stderr, "SET: "); for (int i = 0; i < nb_nodes; i++) fprintf(stderr, "(%d,%d)", nodes[i].x, nodes[i].y); fprintf(stderr, "\n");
//	fprintf(stderr, "%d ", nb_nodes);
}


node_t *extract_smallest_node(void)
{
	node_t *nd = NULL;

	if (nb_nodes > 0) {
//		fprintf(stderr, "SET: "); for (int i = 0; i < nb_nodes; i++) fprintf(stderr, "(%d,%d)", nodes[i].x, nodes[i].y); fprintf(stderr, "\n");

		int min = 0;
		for (int i = 1; i < nb_nodes; i ++)
			if (nodes[i].risk <= nodes[min].risk)
				min = i;
		nd = malloc(sizeof(node_t));
		nd->x = nodes[min].x;
		nd->y = nodes[min].y;
		nd->risk = nodes[min].risk;
		if (min < nb_nodes - 1)
			memmove(&(nodes[min]), &(nodes[min + 1]), (nb_nodes - min - 1) * sizeof(node_t));
		nb_nodes --;
//		fprintf(stderr, "%d ", nb_nodes);
//		fprintf(stderr, "Extract (%d, %d, %d)\n", nd->x, nd->y, nd->risk);
//		fprintf(stderr, "SET: "); for (int i = 0; i < nb_nodes; i++) fprintf(stderr, "(%d,%d)", nodes[i].x, nodes[i].y); fprintf(stderr, "\n");
	}
	return nd;
}

void examine(node_t *nd)
{
	visited[nd->x][nd->y] = 1;
	risk[nd->x][nd->y] = nd->risk;

	if ((nd->x == xmax - 1) && (nd->y == ymax - 1)) {
		fprintf(stdout, "\n\nTotal risk = %d\n", risk[nd->x][nd->y]);
		exit(EXIT_SUCCESS);
	}
	if (nd->x < xmax - 1)
		insert_node(nd->x + 1, nd->y, risk[nd->x][nd->y] + cells[nd->x + 1][nd->y]);
	if (nd->y < ymax - 1)
		insert_node(nd->x, nd->y + 1, risk[nd->x][nd->y] + cells[nd->x][nd->y + 1]);
	if (nd->x > 0)
		insert_node(nd->x - 1, nd->y, risk[nd->x][nd->y] + cells[nd->x - 1][nd->y]);
	if (nd->y > 0)
		insert_node(nd->x, nd->y - 1, risk[nd->x][nd->y] + cells[nd->x][nd->y - 1]);
}
