#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char **array = NULL;
size_t width = 0;
size_t height = 0;


int load_array(void)
{
	char line[1024];

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		if (width == 0) {
			width = strlen(line);
		}
		array = realloc(array, (height + 1) * sizeof(char *));
		if (array == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}

		height ++;

		array[height - 1] = malloc(width + 1);
		if (array[height - 1] == NULL) {
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		strncpy(array[height - 1], line, width + 1);
	}
	return 0;
}



void display_array(void)
{
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++)
			fprintf(stderr, "%c", array[i][j]);
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
}



char **array_backup = NULL;


void backup_array(void)
{
	array_backup = malloc(height * sizeof(char *));
	if (array_backup == NULL) {
		perror("malloc 1");
		exit(EXIT_FAILURE);
	}
	for (size_t i = 0; i < height; i++) {
		array_backup[i] = malloc(width + 1);
		if (array_backup[i] == NULL) {
			perror("malloc 2");
			exit(EXIT_FAILURE);
		}
		strcpy(array_backup[i], array[i]);
	}
}


void clear_backup_array(void)
{
	for (size_t i = 0; i < height; i++)
		memset(array_backup[i], 0, width + 1);
}



void restore_backup(void)
{
	for (size_t i = 0; i < height; i++) {
		strcpy(array[i], array_backup[i]);
		free(array_backup[i]);
	}
	free(array_backup);
	array_backup = NULL;
}



int array_equals_to_backup(void)
{
	for (size_t i = 0; i < height; i++)
		if (strcmp(array_backup[i], array[i]) != 0)
			return 0;
	return 1;
}


typedef struct {
	size_t x;
	size_t y;
} pt_t;

typedef struct {
	pt_t   pt;
	int    count;
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
	if (payload != NULL)
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


size_t dist(size_t x1, size_t y1, size_t x2, size_t y2)
{
	int d = 0;

	if (x1 >= x2)
		d += x1 - x2;
	else
		d += x2 - x1;

	if (y1 >= y2)
		d += y1 - y2;
	else
		d += y2 - y1;

	return d;
}





int bfs(size_t xstart, size_t ystart, size_t xend, size_t yend, size_t allowed, size_t min_gain)
{
	step_t st;
	int found = 0;

	backup_array();

	while(pull(NULL, 0))
		;

	st.pt.x = xstart;
	st.pt.y = ystart;
	st.count = 0;

	push(&st, sizeof(step_t));

	while (pull(&st, sizeof(step_t))) {
		// fprintf(stderr, "(%lu, %lu) [%d] ", st.pt.x, st.pt.y);
		if ((st.pt.x == xend) && (st.pt.y == yend)) {
			found = 1;
			break;
		}

		step_t nxt;
		nxt.count = st.count + 1;
		if ((st.pt.y > 0) && (array[st.pt.y - 1][st.pt.x] == '.')) {
			array[st.pt.y - 1][st.pt.x] = 'v';
			nxt.pt.x = st.pt.x;
			nxt.pt.y = st.pt.y - 1;
			push(&nxt, sizeof(step_t));
		}
		if (( st.pt.y < height - 1) && (array[st.pt.y + 1][st.pt.x] == '.')) {
			array[st.pt.y + 1][st.pt.x] = '^';
			nxt.pt.x = st.pt.x;
			nxt.pt.y = st.pt.y + 1;
			push(&nxt, sizeof(step_t));
		}
		if ((st.pt.x > 0) && (array[st.pt.y][st.pt.x - 1] == '.')) {
			array[st.pt.y][st.pt.x - 1] = '>';
			nxt.pt.x = st.pt.x - 1;
			nxt.pt.y = st.pt.y;
			push(&nxt, sizeof(step_t));
		}
		if ((st.pt.x < width - 1) && (array[st.pt.y][st.pt.x + 1] == '.')) {
			array[st.pt.y][st.pt.x + 1] = '<';
			nxt.pt.x = st.pt.x + 1;
			nxt.pt.y = st.pt.y;
			push(&nxt, sizeof(step_t));
		}
	}
	if (! found)
		return 0;


	int nb = st.count + 1;
	pt_t *path = malloc(nb * sizeof(pt_t));
	for (int i = 0; i < nb; i++) {
		path[i].x = st.pt.x;
		path[i].y = st.pt.y;
		switch (array[path[i].y][path[i].x]) {
			case 'v': st.pt.y++; break;
			case '^': st.pt.y--; break;
			case '>': st.pt.x++; break;
			case '<': st.pt.x--; break;
		}
		if (array[path[i].y][path[i].x] == 'S')
			break;
	}

	int count = 0;

	for (int i = 0; i < nb - 1; i ++)
		for (int j = nb - 1; j > i + 2; j --) {
			size_t d = dist(path[i].x, path[i].y, path[j].x, path[j].y);
			if (d <= allowed)  {
				if (j - i - d >= min_gain) {
					count ++;
				}
			}
		}
	printf("Count = %d\n", count);
	return 0;
}



void part(size_t allowed, size_t min_gain)
{
	size_t xs, ys, xe, ye;

	for (ys = 0; ys < height; ys++) {
		for (xs = 0; xs < width; xs ++)
			if (array[ys][xs] == 'S') {
				break;
			}
		if (xs < width)
			break;
	}

	for (ye = 0; ye < height; ye++) {
		for (xe = 0; xe < width; xe ++)
			if (array[ye][xe] == 'E') {
				array[ye][xe] = '.';
				break;
			}
		if (xe < width)
			break;
	}

	bfs(xs, ys, xe, ye, allowed, min_gain);
}


void part_1(void)
{
	part(2, 100);
}


void part_2(void)
{
	part(20, 100);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_array();

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

