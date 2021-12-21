#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 16384


void compute_possibilities(int score_0, int score_1, int pos_0, int pos_1, long long int *win_0, long long int *win_1, int depth);

int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	int position[2];
	long long int win[2];

	if (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		if (sscanf(buffer, "Player 1 starting position: %d", &(position[0])) != 1) {
			fprintf(stderr, "Unable to read player 1 starting position\n");
			exit(EXIT_FAILURE);
		}
	}
	if (fgets(buffer, BUFFER_SIZE - 1, stdin) != NULL) {
		if (sscanf(buffer, "Player 2 starting position: %d", &(position[1])) != 1) {
			fprintf(stderr, "Unable to read player 2 starting position\n");
			exit(EXIT_FAILURE);
		}
	}
	win[0] = win[1] = 0;
	compute_possibilities(0, 0, position[0], position[1], &(win[0]), &(win[1]), 0);
	fprintf(stdout, "win[0] = %lld, win[1] = %lld\n", win[0], win[1]);

	return 0;
}


#define DIE_VALUES 7
int die_values[DIE_VALUES] = {3, 4, 5, 6, 7, 8, 9};
int die_probs[DIE_VALUES]  = {1, 3, 6, 7, 6, 3, 1};

void compute_possibilities(int score_0, int score_1, int pos_0, int pos_1, long long int *win_0, long long int *win_1, int depth)
{
//	for (int i = 0; i < depth; i++)	fprintf(stderr, "  ");
//	fprintf(stderr, "score_0 = %d, score_1 = %d, pos_0 = %d, pos_1 = %d\n", score_0, score_1, pos_0, pos_1);

	if (score_0 >= 21) {
		(*win_0) = 1;
		(*win_1) = 0;
//		fprintf(stderr, "->  win_0=%lld win_1 = %lld\n", *win_0, *win_1);
		return;
	}
	if (score_1 >= 21) {
		(*win_0) = 0;
		(*win_1) = 1;
//		fprintf(stderr, "->  win_0=%lld win_1 = %lld\n", *win_0, *win_1);
		return;
	}

	for (int d = 0; d < DIE_VALUES; d++) {
		int pos = pos_0 + die_values[d];
		while (pos > 10) pos -= 10;
		long long int win = 0;
		long long int loose = 0;
		compute_possibilities(score_1, score_0 + pos, pos_1, pos, &loose, &win, depth + 1);
		(*win_0) += win * die_probs[d];
		(*win_1) += loose * die_probs[d];
	}
//	for (int i = 0; i < depth; i++)	fprintf(stderr, "  ");
//	fprintf(stderr, "->  win_0=%lld win_1 = %lld\n", *win_0, *win_1);
}
