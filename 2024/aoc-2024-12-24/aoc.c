#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	char name[5];
	int  value;
	int set;
} line_t;

line_t lines[512]; // avoid realloc to keep pointers on previous lines.
int nb_lines = 0;

typedef struct {
	line_t *in_a;
	line_t *in_b;
	line_t *out;
	int op;  // 0: AND, 1: OR, 2: XOR
} gate_t;

gate_t *gates = NULL;
int nb_gates = 0;

line_t * add_line(const char *name, int value)
{
	for (int i = 0; i < nb_lines; i++) {
		if (strcmp(lines[i].name, name) == 0)
			return &(lines[i]);
	}
	strcpy(lines[nb_lines].name, name);
	if (value >= 0) {
		lines[nb_lines].value = value;
		lines[nb_lines].set = 1;
	} else {
		lines[nb_lines].value = 0;
		lines[nb_lines].set = 0;
	}
	nb_lines ++;
	return &lines[nb_lines - 1];
}


void add_gate(const char * a_name, const char *op_name, const char *b_name, const char *out_name)
{
	gates = realloc(gates, (nb_gates + 1) * sizeof(gate_t));
	gates[nb_gates].in_a = add_line(a_name, -1);
	gates[nb_gates].in_b = add_line(b_name, -1);
	gates[nb_gates].out  = add_line(out_name, -1);
	gates[nb_gates].op   = strcmp(op_name, "AND") == 0 ? 0 : strcmp(op_name, "OR") == 0 ? 1 : 2;
	nb_gates++;
}


void load_lines_and_gates(void)
{
	char buffer[128];

	while(fgets(buffer, 128, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0';
		if (strlen(buffer) == 0)
			break;
		// 012345
		// XXX: n
		buffer[3] = '\0';
		add_line(buffer, buffer[5] - '0');
	}

	while(fgets(buffer, 128, stdin) != NULL) {
		buffer[strlen(buffer) - 1] = '\0';
		if (strlen(buffer) == 0)
			break;

		// 012345678901234567890
		// XXX OR XXX -> XXX
		// XXX AAA XXX -> XXX

		buffer[3] = '\0';

		int l = 3;
		if (buffer[4] == 'O')
			l = 2;
		buffer[4 + l] = '\0';
		buffer[8 + l] = '\0';
		add_gate(&buffer[0], &buffer[4], &buffer[5+l], &buffer[12+l]);
	}

	printf("Loaded: %d lines and %d gates\n", nb_lines, nb_gates);
}


void run(void)
{
	int not_set = 1;

	while (not_set) {
		not_set = 0;

		for (int i = 0; i < nb_gates; i++) {
			int result;

			gate_t *g = &gates[i];

			if ((! g->in_a->set) || (! g->in_b->set)) {
				not_set ++;
				continue;
			}
			if (g->op == 0)
				result = g->in_a->value & g->in_b->value;
			else if (g->op == 1)
				result = g->in_a->value | g->in_b->value;
			else
				result = g->in_a->value ^ g->in_b->value;

			g->out->value = result;
			g->out->set = 1;
		}
	}
}


void part_1(void)
{
	run();

	unsigned long long int number = 0;

	for (int i = 0; i < nb_lines; i++) {
		if ((lines[i].name[0] == 'z') && (lines[i].value == 1)){
			int n;
			sscanf(&(lines[i].name[1]), "%d", &n);
			unsigned long long int v = 1;
			v <<= n;
			number |= v;
		}
	}
	printf("Number: %llu\n", number);
}



void part_2(void)
{
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_lines_and_gates();

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


