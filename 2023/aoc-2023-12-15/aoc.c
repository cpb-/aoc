#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


char Line[65536];


void part_1(void)
{
	int i;
	long long int sum = 0;
	long long int hash = 0;

	if (fgets(Line, 65536, stdin) == NULL)
		exit(EXIT_FAILURE);

	for (i = 0; Line[i] != '\0'; i++) {
		if ((Line[i] == ',') || (Line[i] == '\n')) {
			sum += hash;
			hash = 0;
			continue;
		}
		hash += Line[i];
		hash *= 17;
		hash = hash%256;
	}

	printf("  Sum = %lld\n", sum);
}


typedef struct s_lens {
	char label[256];
	int value;
	struct s_lens *next;
	struct s_lens *prev;
} lens_t;


lens_t *Box[256];



int compute_hash(const char *label)
{
	int hash = 0;

	for (int i = 0; label[i] != '\0'; i++) {
		hash += label[i];
		hash *= 17;
		hash = hash%256;
	}
	return hash;
}



void remove_lens(const char *label)
{
	int hash = compute_hash(label);
	lens_t *lens;

	//fprintf(stderr, "Remove: %s\n", label);
	for (lens = Box[hash]; (lens != NULL) && (strcmp(lens->label, label) != 0); lens = lens->next)
		;
	if (lens != NULL) {
		if (lens->next != NULL)
			lens->next->prev = lens->prev;
		if (lens->prev != NULL)
			lens->prev->next = lens->next;
		else
			Box[hash] = lens->next;
		free(lens);
	}
}



void set_lens_value(const char *label, int value)
{
	int hash = compute_hash(label);
	lens_t *lens;

	//fprintf(stderr, "Set value: %s = %d\n", label, value);
	for (lens = Box[hash]; (lens != NULL) && (strcmp(lens->label, label) != 0); lens = lens->next)
		;
	if (lens != NULL) {
		lens->value = value;
	} else {
		lens = malloc(sizeof(lens_t));
		strcpy(lens->label, label);
		lens->value = value;
		lens->next = NULL;
		lens->prev = NULL;

		if (Box[hash] == NULL) {
			Box[hash] = lens;
		} else {
			lens_t *l;
			for (l = Box[hash]; (l->next != NULL); l = l->next)
	        	        ;
			l->next = lens;
			lens->prev = l;
		}
	}
}



void handle_instruction(const char *string, int len)
{
	int i;
	char label[256];
	int value = 0;

	for (i = 0; (i < len) && (string[i] != '-') && (string[i] != '='); i++)
		label[i] = string[i];
	label[i] = '\0';
	if (string[i] == '-')
		remove_lens(label);
	if (string[i] == '=') {
		for (i ++; (i < len) && isdigit(string[i]); i++) {
			value *= 10;
			value += string[i] - '0';
		}
		set_lens_value(label, value);
	}
}



void display_boxes(void)
{
	for (int num = 0; num < 256; num ++) {
		if (Box[num] != NULL) {
			fprintf(stderr, "Box %d: ", num);
			for (lens_t *l = Box[num]; l != NULL; l = l->next)
				fprintf(stderr, "%s (%d), ", l->label, l->value);
			fprintf(stderr, "\n");
		}
	}
}



long long int box_power(int num)
{
	long long int power = 0;
	int index = 1;

	if (Box[num] == NULL)
		return 0;

	for (lens_t *l = Box[num]; l != NULL; l = l->next) {
		power += (index ++) * l->value;
	}

	return (num + 1) * power;
}



void part_2(void)
{
	long long int sum = 0;
	int start = 0;

	for (int i = 0; i < 256; i ++)
		Box[256] = NULL;

	if (fgets(Line, 65536, stdin) == NULL)
		exit(EXIT_FAILURE);

	start = 0;
	for (int i = 0; Line[i] != '\0'; i++) {
		if ((Line[i] == ',') || (Line[i] == '\n')) {
			handle_instruction(&(Line[start]), i - start);
			//display_boxes();
			start = i + 1;
		}
	}
	for (int i = 0; i < 256; i ++)
		sum += box_power(i);

	printf("  Sum = %lld\n", sum);
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
