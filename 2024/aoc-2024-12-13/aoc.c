#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct {
	long long int ax;
	long long int ay;
	long long int bx;
	long long int by;
	long long int px;
	long long int py;
	long long int na;
	long long int nb;
	long long int tk;
} mach_t;


mach_t *machs = NULL;
int nb_machs = 0;


void load_machines(void)
{
	char buffer[1024];
	for (;;) {
		if (fgets(buffer, 1024, stdin) == NULL)
			break;
		machs = realloc(machs, (nb_machs + 1) * sizeof(mach_t));
		mach_t *m = &(machs[nb_machs]);
		if (sscanf(buffer, "Button A: X+%lld, Y+%lld", &(m->ax), &(m->ay)) != 2)
			break;
		if (fgets(buffer, 1024, stdin) == NULL)
			break;
		if (sscanf(buffer, "Button B: X+%lld, Y+%lld", &(m->bx), &(m->by)) != 2)
			break;
		if (fgets(buffer, 1024, stdin) == NULL)
			break;
		if (sscanf(buffer, "Prize: X=%lld, Y=%lld", &(m->px), &(m->py)) != 2)
			break;
		nb_machs ++;
		if (fgets(buffer, 1024, stdin) == NULL)
			break;
	}
	printf("Loaded %d machines\n", nb_machs);
}


void compute_machine_cost(mach_t *m)
{
	m->na = m->nb = m->tk = 0;


	long long int down = m->ax * m->by - m->bx * m->ay;

	if (down == 0)
		return;

	long long int up = m->ax * m->py - m->px * m->ay;

	if (up == 0)
		return;

	m->nb = up / down;

	if (down * m->nb != up)
		return;

	m->na = (m->px - m->nb * m->bx) / m->ax;

	if ((m->na * m->ax + m->nb * m->bx != m->px) || (m->na * m->ay + m->nb * m->by != m->py))
		return;

	m->tk = 3 * m->na + m->nb;
}


void apply_offset(mach_t *m)
{
	m->px += 10000000000000;
	m->py += 10000000000000;
}


void part_1(void)
{
	long long int sum = 0;
	for (int m = 0; m < nb_machs; m++) {
		compute_machine_cost(& (machs[m]));
		sum = sum + machs[m].tk;
	}

	printf("Sum = %lld\n", sum);
}


void part_2(void)
{
	long long int sum = 0;
	for (int m = 0; m < nb_machs; m++) {
		apply_offset(&(machs[m]));
		compute_machine_cost(& (machs[m]));
		sum = sum + machs[m].tk;
	}

	printf("Sum = %lld\n", sum);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_machines();

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
