#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int x;
	int y;
} pt_t;

pt_t *pts = NULL;
int nb_pts = 0;

int width = 0;
int height = 0;

void load_points(void)
{
	char line[64];

	while (fgets(line, 64, stdin) != NULL) {
		int x, y;
		if (sscanf(line, "%d,%d", &x, &y) != 2)
			break;
		pts = realloc(pts, (nb_pts + 1) * sizeof(pt_t));
		pts[nb_pts].x = x;
		pts[nb_pts].y = y;
		nb_pts ++;
		if (x > width - 1)
			width = x + 1;
		if (y > height - 1)
			height = height + 1;
	}
	printf("Nb points: %d, width = %d, heigth = %d\n", nb_pts, width, height);
}


typedef struct {
	pt_t pt;
	int     count;
} step_t;



typedef struct s_item {
	void *payload;
	struct s_item *next;
	struct s_item *prev;
} item_t;

item_t *item_head = NULL;
item_t *item_tail = NULL;

void push(void *payload, size_t size)
{
	item_t *it = malloc(sizeof(item_t));
	it->payload = malloc(size);
	memcpy(it->payload, payload, size);
	it->next = item_head;
	it->prev = NULL;
	item_head = it;
	if (it->next == NULL)
		item_tail = it;
	else
		it->next->prev = it;
}


int pull(void *payload, size_t size)
{
	item_t *it = item_tail;
	if (it == NULL)
		return 0;
	memcpy(payload, it->payload, size);
	free(it->payload);
	item_tail = it->prev;
	if (it->prev == NULL)
		item_head = NULL;
	else
		it->prev->next = NULL;
	free(it);
	return 1;
}



char **array = NULL;

void init_array(void)
{
	array = malloc(height * sizeof(char *));

	for (int j = 0; j < height; j++) {
		array[j] = malloc(width * sizeof(char));
	}
}


void clear_array(void)
{
	for (int j = 0; j < height; j++)
		memset(array[j], '.', width);
}




int bfs(int nb_blocs)
{
	step_t st;

	st.pt.x = 0;
	st.pt.y = 0;
	st.count = 0;

	clear_array();

	for (int n = 0; n < nb_blocs; n++)
		array[pts[n].y][pts[n].x] = '#';

	while(pull(&st, sizeof(step_t)))
		;

	push(&st, sizeof(step_t));


	while (pull(&st, sizeof(step_t))) {
		if ((st.pt.x == width - 1) && (st.pt.y == height - 1)) {
			return st.count;
		}
		array[st.pt.y][st.pt.x] = 'O';


		step_t nxt;
		nxt.count = st.count + 1;
		if ((st.pt.y > 0) && (array[st.pt.y - 1][st.pt.x] == '.')) {
			array[st.pt.y - 1][st.pt.x] = 'O';
			nxt.pt.x = st.pt.x;
			nxt.pt.y = st.pt.y - 1;
			push(&nxt, sizeof(step_t));
		}
		if (( st.pt.y < height - 1) && (array[st.pt.y + 1][st.pt.x] == '.')) {
			array[st.pt.y + 1][st.pt.x] = 'O';
			nxt.pt.x = st.pt.x;
			nxt.pt.y = st.pt.y + 1;
			push(&nxt, sizeof(step_t));
		}
		if ((st.pt.x > 0) && (array[st.pt.y][st.pt.x - 1] == '.')) {
			array[st.pt.y][st.pt.x - 1] = 'O';
			nxt.pt.x = st.pt.x - 1;
			nxt.pt.y = st.pt.y;
			push(&nxt, sizeof(step_t));
		}
		if ((st.pt.x < width - 1) && (array[st.pt.y][st.pt.x + 1] == '.')) {
			array[st.pt.y][st.pt.x + 1] = 'O';
			nxt.pt.x = st.pt.x + 1;
			nxt.pt.y = st.pt.y;
			push(&nxt, sizeof(step_t));
		}
	}
	return 0;
}


void part_1(void)
{
	if (width == 7)
		printf("Count = %d\n", bfs(12));
	else
		printf("Count = %d\n", bfs(1024));
}


void part_2(void)
{
	int i;

	for (i = 0; i < nb_pts; i++)
		if (bfs(i) == 0)
			break;
	printf("(X, Y) = %d,%d\n", pts[i - 1].x, pts[i - 1].y);

}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_points();
	init_array();

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
