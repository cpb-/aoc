#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int check_line(const char *buffer);

int main(int argc, char *argv[])
{
	char buffer[4096];
	int sum = 0;
	while (fgets(buffer, 4095, stdin) != NULL) {
		sum += check_line(buffer);
	}
	fprintf(stderr, "Syntax error code = %d\n", sum);
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


int score(char c)
{
	switch(c) {
		case ')': return 3;
		case ']': return 57;
		case '}': return 1197;
		case '>': return 25137;
		default: return 0;
	}
}


int check_line(const char *buffer)
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
					return score('}');
				size --; break;
			case '>':
				if ((size == 0) || (expected[size - 1] != '>'))
					return score('>');
				size --; break;
			case ']':
				if ((size == 0) || (expected[size - 1] != ']'))
					return score(']');
				size --; break;
			case ')':
				if ((size == 0) || (expected[size - 1] != ')'))
					return score(')');
				size --; break;
			default:
				break;
		}
	}
	return 0;
}

