#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
	int matching_numbers;
	int count;
} card;

card *cards = NULL;
int nb_cards = 0;


int count_matching_numbers(const char *line)
{
	int offset;
	int n;
	int card_num;
	int winning_numbers[10];
	int nb_winning = 0;
	int number;
	int value = 0;

	cards = realloc(cards, (nb_cards + 1) * sizeof(card));
	if (cards == NULL)
		exit(EXIT_FAILURE);
	cards[nb_cards].matching_numbers = 0;
	cards[nb_cards].count = 1;
	nb_cards ++;

	if (sscanf(line, "Card %d: %n", &card_num, &offset) != 1)
		return 0;
	while (line[offset] != '|') {
		if (sscanf(&(line[offset]), "%d %n", &(winning_numbers[nb_winning]), &n) != 1)
			break;
		offset += n;
		nb_winning ++;
	}
	offset ++;
	while (line[offset] != '\0') {
		if (sscanf(&(line[offset]), "%d %n", &number, &n) != 1)
			break;
		offset += n;
		for (int i = 0; i < nb_winning; i++)
			if (winning_numbers[i] == number) {
				cards[nb_cards - 1].matching_numbers++;
				if (value == 0)
					value = 1;
				else
					value *= 2;
			}
	}
	return value;
}


void part_1(void)
{
	char line[1024];
	int sum = 0;

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		sum += count_matching_numbers(line);
	}
	printf(" Sum = %d\n", sum);
}


void part_2(void)
{
	char line[1024];
	int sum = 0;

	while (fgets(line, 1024, stdin) != NULL) {

		line[strlen(line) - 1] = '\0';
		if (line[0] == '\0')
			break;
		count_matching_numbers(line);
	}

	for (int c = 0; c < nb_cards; c++) {
		for (int i = 0; i < cards[c].matching_numbers; i++) {
			if (c + i + 1 >= nb_cards) {
				fprintf(stderr, "Error: nb_cards = %d, cards[%d].matching_numbers = %d\n", nb_cards, c, cards[c].matching_numbers);
				exit(EXIT_FAILURE);
			}
			cards[c + i + 1].count += cards[c].count;
		}
	}

	for (int c = 0; c < nb_cards; c++) {
		sum += cards[c].count;
	}

	printf(" Sum = %d\n", sum);
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
