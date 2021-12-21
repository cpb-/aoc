#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 16384


int main(int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];

	long long int score[2];
	int position[2];
	int player = 0;
	int dice = 1;
	int times = 0;

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
	score[0] = score[1] = 0;

	for (;;) {

		for (int i = 0; i < 3; i ++) {
			position[player] += dice;
			times++;
			while (position[player] > 10)
				position[player] -= 10;
			dice ++;
			if (dice > 100)
				dice = 1;
		}
		score[player] += position[player];

		if (score[player] >= 1000)
			break;

		player = 1 - player;
	}
	fprintf(stderr, "Winner = %d, loser score = %lld, times =%d, result = %lld\n",
		player, score[1 - player], times, score[1 - player] * times);
	return 0;
}

