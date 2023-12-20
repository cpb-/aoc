#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "libaoc.h"

#define MODULE_BROADCASTER  1
#define MODULE_FLIP_FLOP    2
#define MODULE_CONJUNCTION  3

#define NAME_MAX     16
#define NB_DEST_MAX  8
#define NB_SRC_MAX   16

typedef struct s_module {
	int type;
	char name[NAME_MAX];
	int state;
	char dest_name[NAME_MAX][NB_DEST_MAX];
	struct s_module *dest[NB_DEST_MAX];
	struct s_module *src[NB_SRC_MAX];
	int rcv[NB_SRC_MAX];
} module_t;


module_t *Modules = NULL;
size_t Nb_modules = 0;


void load_modules(void)
{
	char line[1024];

	while(fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;

		Modules = realloc(Modules, (Nb_modules + 1) * sizeof(module_t));
		module_t *m = &(Modules[Nb_modules]);
		Nb_modules++;

		for (size_t i = 0; i < NB_DEST_MAX; i++) {
			m->dest_name[i][0] = '\0';
			m->dest[i] = NULL;
		}
		for (size_t i = 0; i < NB_SRC_MAX; i++) {
			m->src[i] = NULL;
			m->rcv[i] = 0;
		}
		m->state = 0;

		int idx = 0;
		if (strncmp(line, "broadcaster", 11) == 0) {
			m->type = MODULE_BROADCASTER;
			strcpy(m->name, "broadcaster");
			idx += 15;
		} else if (line[0] == '%') {
			m->type = MODULE_FLIP_FLOP;
			strncpy(m->name, &(line[1]), 2);
			m->name[2] = '\0';
			idx += 7;
		} else if (line[0] == '&') {
			m->type = MODULE_CONJUNCTION;
			strncpy(m->name, &(line[1]), 2);
			m->name[2] = '\0';
			idx += 7;
		} else {
			fprintf(stderr, "Unknown module type '%c' on line %s\n", line[0], line);
			exit(EXIT_FAILURE);
		}

		int nb = 0;
		while (line[idx] != '\0') {
			strncpy(m->dest_name[nb], &(line[idx]), 2);
			m->dest_name[nb][2] = '\0';
			idx += 2;
			if (line[idx] == ',')
				idx += 2;
			nb ++;
		}
	}


	for (size_t i = 0; i < Nb_modules; i++) {
		module_t *m = &(Modules[i]);
		for (size_t n = 0; m->dest_name[n][0] != '\0'; n++) {
			size_t j;
			for (j = 0; j < Nb_modules; j++) {
				if (strcmp(m->dest_name[n], Modules[j].name) == 0) {
					m->dest[n] = &(Modules[j]);
					break;
				}
			}
			if (j == Nb_modules) {
				// fprintf(stderr, "Dest module '%s' not found\n", m->dest_name[n]);
				m->dest[n] = NULL;
			}
		}
	}


	for (size_t i = 0; i < Nb_modules; i++) {
		module_t *m = &(Modules[i]);
		if (m->type == MODULE_BROADCASTER)
			continue;
		for (size_t j = 0; m->dest[j] != NULL; j++) {
			if (m->dest[j]->type != MODULE_CONJUNCTION)
				continue;
			size_t k;
			for (k = 0; m->dest[j]->src[k] != NULL; k++)
				;
			m->dest[j]->src[k] = m;
		}
	}

	fprintf(stderr, "   Loaded: %lu modules\n", Nb_modules);
}


typedef struct s_pulse {
	int pulse_state;
	module_t *dest;
	module_t *from;
	struct s_pulse *next;
	struct s_pulse *prev;
} pulse_t;


pulse_t *Fifo = NULL;


void push_pulse(int pulse, module_t *m, module_t *p)
{
	pulse_t *ps = malloc(sizeof(pulse_t));

	// fprintf(stderr, "    -> %s %d\n", m->name, pulse);
	ps->pulse_state = pulse;
	ps->dest = m;
	ps->from = p;
	ps->next = Fifo;
	if (ps->next != NULL)
		ps->next->prev = ps;
	ps->prev = NULL;
	Fifo = ps;
}



pulse_t *get_pulse(void)
{
	pulse_t *ps = NULL;

	if (Fifo == NULL)
		return NULL;
	for (ps = Fifo; ps->next != NULL; ps = ps->next)
		;
	if (Fifo == ps)
		Fifo = NULL;
	else
		ps->prev->next = NULL;

	return ps;
}


long long int Cycles = 0;
long long int Parent_cycles[4] = { 0, 0, 0, 0 };
const char *Parent_names[4] = { "nh", "mf", "fd", "kb" };


int handle_pulses(long long int *lo, long long int *hi)
{
	pulse_t *ps = NULL;

	while ((ps = get_pulse()) != NULL) {

		module_t *m = ps->dest;
		module_t *p = ps->from;
		int pulse = ps->pulse_state;
		free(ps);

//fprintf(stderr, "%s ", m->name);
		// fprintf(stderr, "%s (from %s) %d %lld %lld\n", m->name, (p != NULL) ? p->name : "-", pulse, *lo, *hi);

		if (pulse == 0) {
			if (lo != NULL)
				(*lo)++;
		} else {
			if (hi != NULL)
				(*hi)++;
		}

		if (m == NULL)
			continue;

		switch (m->type) {
			case MODULE_BROADCASTER:
				for (size_t i  = 0; m->dest_name[i][0] != '\0'; i++) {
					push_pulse(pulse, m->dest[i], m);
				}
				break;
			case MODULE_FLIP_FLOP:
				if (! pulse) {
					m->state = 1 - m->state;
					for (size_t i = 0; m->dest_name[i][0] != '\0'; i++) {
						if (m->dest[i] != NULL) {
							push_pulse(m->state, m->dest[i], m);
						} else {
							if (lo != NULL)
								(*lo) += (m->state == 0);
							if (hi != NULL)
								(*hi) += (m->state == 1);
						}
					}
				}
				break;
			default: // conjonction
				size_t i;
				for (i = 0; m->src[i] != NULL; i++)
					if (p == m->src[i])
						break;
				assert (m->src[i] != NULL);
				m->rcv[i] = pulse;
				for (i = 0; m->src[i] != NULL; i++)
					if (m->rcv[i] != 1)
						break;
				if (m->src[i] == NULL) {
					// All 1
					for (int i = 0;  i < 4; i ++) {
						if (strcmp(m->name, Parent_names[i]) == 0) {
							Parent_cycles[i] = Cycles;
						}
					}
					for (i = 0; m->dest_name[i][0] != '\0'; i++) {
						if (m->dest[i] != NULL) {
							push_pulse(0, m->dest[i], m);
						} else {
							if (lo != NULL)
								(*lo)++;
						}
					}
				} else {
					for (i = 0; m->dest_name[i][0] != '\0'; i++) {
						if (m->dest[i] != NULL) {
							push_pulse(1, m->dest[i], m);
						} else {
							if (hi != NULL)
								(*hi)++;
						}
					}
				}
				break;
		}
	}
	return 0;
}




int push_button(long long int *lo, long long int *hi)
{
	module_t *m = NULL;
	for (size_t i = 0; i < Nb_modules; i++) {
		if (Modules[i].type == MODULE_BROADCASTER) {
			m = &(Modules[i]);
			break;
		}
	}
	assert(m != NULL);
	push_pulse(0, m, NULL);

	return handle_pulses(lo, hi);
}


void part_1(void)
{
	load_modules();

	long long int lo_count = 0;
	long long int hi_count = 0;
	for (int i = 0; i < 1000; i ++) {
		push_button(&lo_count, &hi_count);
	}
	printf("   Result: %lld * %lld = %lld\n", lo_count, hi_count, lo_count * hi_count);
}


void part_2(void)
{
	load_modules();

	Cycles = 0;
	for (;;) {
		int i;
		for (i = 0; i < 4; i++)
			if (Parent_cycles[i] == 0)
				break;
		if (i == 4)
			break;
		Cycles ++;
		if (push_button(NULL, NULL))
			break;
	}

	printf("  Result: %lld\n", compute_multiple_lcm(Parent_cycles, 4));

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
			part_1();
			break;
		case 2:
			part_2();
			break;
		default:
			break;
	}
	return EXIT_SUCCESS;
}
