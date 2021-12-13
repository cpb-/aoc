#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

long long int check_line(const char *buffer);

int main(int argc, char *argv[])
{
	char buffer[4096];
	int n = 0;
	while (fgets(buffer, 4095, stdin) != NULL) {
		long long int score = check_line(buffer);
		if (score > 0) {
			n ++;
			fprintf(stdout, "%lld\n", score);
		}
	}
	fflush(stdout);
	fprintf(stderr, "Middle rank = %d\n", (n + 1) / 2);
	return 0;
}



int is_brace(char c)
{
	return (
	    (c == '{') || (c == '}')
	 || (c == '(') || (c == ')')
	 || (c == '[') || (c == ']')
	 || (c == '<') || (c == '>'));
}



long long int check_line(const char *buffer)
{
	int expected[4096]; // LIFO
	int size = 0;

	for (int i = 0; is_brace(buffer[i]); i++) {
		switch(buffer[i]) {
			case '{': expected[size ++] = '}'; break;
			case '(': expected[size ++] = ')'; break;
			case '[': expected[size ++] = ']'; break;
			case '<': expected[size ++] = '>'; break;
			case '}':
				if ((size == 0) || (expected[size - 1] != '}'))
					return 0;
				size --; break;
			case '>':
				if ((size == 0) || (expected[size - 1] != '>'))
					return 0;
				size --; break;
			case ']':
				if ((size == 0) || (expected[size - 1] != ']'))
					return 0;
				size --; break;
			case ')':
				if ((size == 0) || (expected[size - 1] != ')'))
					return 0;
				size --; break;
			default:
				break;
		}
	}

	long long int score = 0;

	for (int i = size -1; i >= 0; i --) {
			score *= 5;
		switch(expected[i]) {
			case ')': score += 1; break;
			case ']': score += 2; break;
			case '}': score += 3; break;
			case '>': score += 4; break;
			default: break;
		}
	}
	return score;
}

