#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void read_initial_values(void);
void add_value(int val);
int compute_fuel(int val);

int *values = NULL;
int nb_values = 0;


char led[10][10];
int digit[10];

char display[10][10];

int read_digits(const char *buffer);
void read_displays(const char *buffer);
int contains(const char *a, const char *b);
int union_length(const char *a, const char *b);
int equals(const char *a, const char *b);

int main(int argc, char *argv[])
{
	char buffer[1024];
	int pos;
	int led_1, led_4;
	long int sum = 0;

	while(fgets(buffer, 1023, stdin) != NULL) {
		pos = read_digits(buffer);
		if (pos < 0)
			break;
		for (int i = 0; i < 10; i ++)
			digit[i] = -1;
		for (int i = 0; i < 10; i ++) {
			if (digit[i] == -1) {
				if (strlen(led[i]) == 2) {
					digit[i] = 1;
					led_1 = i;
				} else if (strlen(led[i]) == 3) {
					digit[i] = 7;
				} else if (strlen(led[i]) == 4) {
					digit[i] = 4;
					led_4 = i;
				} else if (strlen(led[i]) == 7)
					digit[i] = 8;
			}
		}
		for (int i = 0; i < 10; i ++) {
			if (digit[i] != -1)
				continue;
			if (strlen(led[i]) != 6)
				continue;
			if (!contains(led[led_4], led[i]))
				continue;
			digit[i] = 9;
		}
		for (int i = 0; i < 10; i ++) {
			if (digit[i] != -1)
				continue;
			if (strlen(led[i]) != 6)
				continue;
			if (!contains(led[led_1], led[i]))
				continue;
			digit[i] = 0;
		}
		for (int i = 0; i < 10; i ++) {
			if (digit[i] != -1)
				continue;
			if (strlen(led[i]) != 6)
				continue;
			digit[i] = 6;
		}
		for (int i = 0; i < 10; i ++) {
			if (digit[i] != -1)
				continue;
			if (strlen(led[i]) != 5)
				continue;
			if (!contains(led[led_1], led[i]))
				continue;
			digit[i] = 3;
		}
		for (int i = 0; i < 10; i ++) {
			if (digit[i] != -1)
				continue;
			if (union_length(led[i], led[led_4]) == 7) {
				digit[i] = 2;
			} else {
				digit[i] = 5;
			}
		}

		for (int i = 0; i < 10; i ++) {
			fprintf (stderr, "%d ", digit[i]);
		}


		while ((buffer[pos] == ' ') || (buffer[pos] == '|'))
			pos ++;

		read_displays(&(buffer[pos]));

		int value = 0;
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 10; i++) {
				if (equals(display[j], led[i])) {
					value = value * 10 + digit[i];
				}
			}
		}
		fprintf(stderr, "%d ", value);
		sum += value;
		fprintf(stderr, "\n");
	}

	fprintf(stderr, "Value = %ld\n", sum);

	return 0;
}


int read_digits(const char *buffer)
{
	int pos = 0;
	for (int i = 0; i < 10; i ++) {
		int j = 0;
		led[i][j] = '\0';
		while(buffer[pos] == ' ')
			pos++;
		while(buffer[pos] != ' ') {
			led[i][j] = buffer[pos];
			pos ++;
			j ++;
		}
		led[i][j] = '\0';
	}
	return pos;
}


void read_displays(const char *buffer)
{
	int pos = 0;
	for (int i = 0; i < 4; i ++) {
		int j = 0;
		display[i][j] = '\0';
		while(buffer[pos] == ' ')
			pos++;
		while (isalpha(buffer[pos])){
			display[i][j] = buffer[pos];
			pos ++;
			j ++;
		}
		display[i][j] = '\0';
	}
}


char substract_digit(int a, int b)
{
	int i,j;

	for (i = 0; led[a][i] != '\0'; i ++) {
		for (j = 0; led[b][j] != '\0'; j ++) {
			if (led[a][i] == led[b][j])
				break;
		}
		if (led[b][j] == '\0')
			return led[a][i];
	}
	return '*';
}



int contains(const char *a, const char *b)
{
	int i,j;

	for (i = 0; a[i] != '\0'; i ++) {
		for (j = 0; b[j] != '\0'; j ++) {
			if (a[i] == b[j])
				break;
		}
		if (b[j] == '\0')
			return 0;
	}
	return 1;
}



int union_length(const char *a, const char *b)
{
	char string[20];
	int i,j = 0;
	string[0] = '\0';

	for (i = 0; a[i] != '\0'; i++) {
		for (j = 0; string[j] != '\0'; j++) {
			if (a[i] == string[j])
				break;
		}
		if (string[j] == '\0') {
			string[j++] = a[i];
			string[j++] = '\0';
		}
	}

	for (i = 0; b[i] != '\0'; i++) {
		for (j = 0; string[j] != '\0'; j++) {
			if (b[i] == string[j])
				break;
		}
		if (string[j] == '\0') {
			string[j++] = b[i];
			string[j++] = '\0';
		}
	}
	return strlen(string);
}



int equals(const char *a, const char *b)
{
	if (! contains(a, b))
		return 0;
	if (! contains(b, a))
		return 0;
	return 1;
}
