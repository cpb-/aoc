#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct {
	char a;
	char b;
	char c;
} rule_t;

rule_t *rules = NULL;
int nb_rules;


void add_rule(char a, char b, char c);
void apply_rules(char *src, char *dst);
void count_items(const char *string);



int main(int argc, char *argv[])
{
	char temp1[65536];
	char temp2[65536];
	char buffer[4096];

	fgets(temp1, 4095, stdin);
	fgets(buffer, 4095, stdin); // blank line

	while (fgets(buffer, 4095, stdin) != NULL) {
		char a, b, c;
		if (sscanf(buffer, "%c%c -> %c", &a, &b, &c) != 3)
			break;
		add_rule(a, b, c);
	}

	for (int step = 0; step < 10; step ++) {
		apply_rules(temp1, temp2);
		strcpy(temp1, temp2);
	}

	count_items(temp1);

	return 0;
}


void add_rule(char a, char b, char c)
{
	rules = realloc(rules, (nb_rules + 1) * sizeof(rule_t));
	rules[nb_rules].a = a;
	rules[nb_rules].b = b;
	rules[nb_rules].c = c;
	fprintf(stderr, "Added rules %c - %c -> %c\n", rules[nb_rules].a, rules[nb_rules].b, rules[nb_rules].c);
	nb_rules ++;
}


void apply_rules(char *src, char *dst)
{
	int i, j;
	for (i = 0, j = 0; isalpha(src[i]); i++, j++) {
		dst[j] = src[i];
		if (isalpha(src[i+1])) {
			for (int r = 0; r < nb_rules; r++) {
				if ((rules[r].a == src[i]) && (rules[r].b == src[i+1])) {
					j++;
					dst[j] = rules[r].c;
				}
			}
		}
	}
	dst[j++] = src[i];
	dst[j] = '\0';
	fprintf(stdout, "%s\n", dst);
}



void count_items(const char *string)
{
	char item[64];
	int count[64];
	int nb_items = 0;

	for (int i = 0; i < strlen(string); i++) {
		int j;
		for (j = 0; j < nb_items; j++) {
			if (string[i] == item[j]) {
				count[j] ++;
				break;
			}
		}
		if (j == nb_items) {
			item[j] = string[i];
			count[j] = 1;
			nb_items ++;
		}
	}
	for (int i = 0; i < nb_items; i ++) {
		fprintf(stdout, "%d  %c\n", count[i], item[i]);
	}

}
