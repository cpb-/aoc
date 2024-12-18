#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

long long int A, B, C, I;

int prog[32];
int nb_instr = 0;

void load_registers_and_prog(void)
{
	char line[64];

	if ((fgets(line, 64, stdin) == NULL) || (sscanf(line, "Register A: %lld", &A) != 1))
		exit(1);
	if ((fgets(line, 64, stdin) == NULL) || (sscanf(line, "Register B: %lld", &B) != 1))
		exit(1);
	if ((fgets(line, 64, stdin) == NULL) || (sscanf(line, "Register C: %lld", &C) != 1))
		exit(1);
	I = 0;

	if (fgets(line, 64, stdin) == NULL)
		exit(1);
	if (fgets(line, 64, stdin) == NULL)
		exit(1);
	if (strncmp(line, "Program:", 8) != 0)
		exit(1);
	int pos = 8;
	int n = 0;
	while (sscanf(&(line[pos]), "%d%n", &(prog[nb_instr]), &n) == 1) {
		pos += n;
		nb_instr ++;

		if (line[pos] != ',')
			break;
		pos ++; // ','
	}
	printf("Read %d instructions\n", nb_instr);
}


int combo(int n)
{
	int v = n & 0x07;
	return v < 4 ? v : v == 4 ? A : v == 5 ? B : C;
}


int literal(int n)
{
	return n;
}


int *out = NULL;
int nb_out = 0;

void run(int verbose)
{
	out = realloc(out, 0);
	nb_out = 0;

	while (I < nb_instr) {
		int denom;
		switch(prog[I++]) {
			case 0: // ADV
				denom = 1 << combo(prog[I++]);
				A = A / denom;
				break;
			case 1: // BXL
				B = B ^ literal(prog[I++]);
				break;
			case 2: // BST
				B = combo(prog[I++]) & 0x07;
				break;
			case 3: //JNZ
				if (A == 0)
					I++;
				else
					I = literal(prog[I]);
				break;
			case 4: // BXC
				B = B ^C;
				I++;
				break;
			case 5:
				out = realloc(out, (nb_out + 1) * sizeof(int));
				out[nb_out] = combo(prog[I++]) & 0x07;
				nb_out ++;
				break;
			case 6: // BDV
				denom = 1 << combo(prog[I++]);
				B = A / denom;
				break;
			case 7: // CDV
				denom = 1 << combo(prog[I++]);
				C = A / denom;
				break;
		}
	}

	if (verbose) {
		for (int i = 0; i < nb_out; i ++) {
			printf("%d", out[i]);
			if (i < nb_out - 1)
				printf(",");
		}
		printf("\n");
	}
}


void part_1(void)
{
	run(1);
}


int search_digit(int d, long long int a)
{
	if (d > 16) {
		printf("a = %lld  out = ", a);
		A = a;
		B = C = I = 0;
		run(1);
		return 1;
	}
	for (int i = 0; i < 8; i++) {
		A = a << 3 | i;
		B = C = I = 0;
		run(0);
		int j;
		for (j = 0; j < d; j ++) {
			if (out[nb_out - 1 - j] != prog[15 - j])
				break;
		}
		if (j == d) {
			if (search_digit(d + 1, a << 3 | i))
				return 1;
		}
	}
	return 0;
}


void part_2(void)
{
	int program[16] = { 2, 4, 1, 7, 7, 5, 4, 1, 1, 4, 5, 5, 0, 3, 3, 0 };

	memcpy(prog, program, 16 * sizeof(int));
	nb_instr = 16;

	search_digit(0, 0);
}



int main(int argc, char *argv[])
{
	if ((argc < 2) || (argc > 2)) {
		fprintf(stderr, "usage: %s [1|2]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	load_registers_and_prog();

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
