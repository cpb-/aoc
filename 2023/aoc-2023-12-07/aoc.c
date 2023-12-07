#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define FIVE_OF_A_KIND   6
#define FOUR_OF_A_KIND   5
#define FULL_HOUSE       4
#define THREE_OF_A_KIND  3
#define TWO_PAIRS        2
#define ONE_PAIR         1
#define HIGH_CARD        0


typedef struct {
	char  set[6];
	int   bid;
	int   set_value[5];
	int   count[15];
	int   hand_value;
	int   type;
	int   rank;
} hand;


hand *hands = NULL;
int  nb_hands = 0;



int value(char c, int part)
{
	switch(c) {
		case 'A': return 14;
		case 'K': return 13;
		case 'Q': return 12;
		case 'J': if (part == 1) return 11; return 1;
		case 'T': return 10;
		default: break;
	}
	if (isdigit(c))
		return c - '0';
	fprintf(stderr, "Unknown value %c\n", c);
	exit(EXIT_FAILURE);
}



int read_hands(int part)
{
	char line[1024];


	while (fgets(line, 1024, stdin) != NULL) {
		if (strlen(line) < 8)
			break;
		hands = realloc(hands, (nb_hands + 1) * sizeof(hand));
		if (hands == NULL)
			exit (EXIT_FAILURE);
		for(int i = 0; i < 15; i++)
			hands[nb_hands].count[i] = 0;
		for(int i = 0; i < 5; i ++) {
			hands[nb_hands].set[i] = line[i];
			hands[nb_hands].set_value[i] = value(line[i], part);
			hands[nb_hands].count[hands[nb_hands].set_value[i]] ++;
		}
		hands[nb_hands].set[5] = '\0';
		if (sscanf(&(line[6]), "%d", &(hands[nb_hands].bid)) != 1) {
			fprintf(stderr, "Missing bid: %s\n", &(line[6]));
			return -1;
		}
		nb_hands ++;

	}
	fprintf(stderr, "Read %d hands\n", nb_hands);
	return 0;
}



void compute_hand_types(int part)
{
	hand *hd = NULL;
	int nb_pairs;

	for (int h = 0; h < nb_hands; h++) {
		hd = &(hands[h]);
		hd->type = HIGH_CARD;
		for (int i = 2; i < 15; i++) {
			if (hd->count[i] == 5) {
				hd->type = FIVE_OF_A_KIND;
				break;
			}
			if (hd->count[i] == 4) {
				hd->type = FOUR_OF_A_KIND;
				if (part == 2) {
					if (hd->count[1] > 0)
						hd->type = FIVE_OF_A_KIND;
				}
				break;
			}
		}
		if (hd->type != HIGH_CARD)
			continue;

		for (int i = 2; i < 15; i++) {
			if (hd->count[i] == 3) {
				hd->type = THREE_OF_A_KIND;
				if (part == 2) {
					if (hd->count[1] > 1) {
						hd->type = FIVE_OF_A_KIND;
						break;
					}
					if (hd->count[1] > 0) {
						hd->type = FOUR_OF_A_KIND;
						break;
					}
				}
				for (int j = 2; j < 15; j++)
					if (hd->count[j] == 2)
						hd->type = FULL_HOUSE;
				break;
			}
		}

		if (hd->type != HIGH_CARD)
			continue;

		nb_pairs = 0;
		for (int i = 2; i < 15; i++) {
			if (hd->count[i] == 2)
				nb_pairs ++;
		}

		if (nb_pairs > 1) {
			hd->type = TWO_PAIRS;
			if (part == 2) {
				if (hd->count[1] > 0) {
					hd->type = FULL_HOUSE;
				}
			}
		} else if (nb_pairs > 0) {
			hd->type = ONE_PAIR;
			if (part == 2) {
				if (hd->count[1] > 0) {
					hd->type = THREE_OF_A_KIND;
				}
				if (hd->count[1] > 1) {
					hd->type = FOUR_OF_A_KIND;
				}
				if (hd->count[1] > 2) {
					hd->type = FIVE_OF_A_KIND;
				}
			}
		} else {
			if (part == 2) {
				if (hd->count[1] > 0)
					hd->type = ONE_PAIR;
				if (hd->count[1] > 1)
					hd->type = THREE_OF_A_KIND;
				if (hd->count[1] > 2)
					hd->type = FOUR_OF_A_KIND;
				if (hd->count[1] > 3)
					hd->type = FIVE_OF_A_KIND;
			}
		}
	}
}




int hands_compare(const void *h1, const void *h2)
{
	hand *hd1 = (hand *)h1;
	hand *hd2 = (hand *)h2;
	if (hd1->type < hd2->type)
		return -1;
	if (hd1->type > hd2->type)
		return 1;
	for (int i = 0; i < 5; i ++) {
		if (hd1->set_value[i] < hd2->set_value[i])
			return -1;
		if (hd1->set_value[i] > hd2->set_value[i])
			return 1;
	}
	return 0;
}



void display_hands(void)
{
	for (int i = 0; i < nb_hands; i++)
		fprintf(stderr, "%s %d\n", hands[i].set, hands[i].bid);

}



void part_function(int part)
{
	long long int sum = 0;

	if (read_hands(part) != 0)
		exit(EXIT_FAILURE);

	compute_hand_types(part);

	qsort(hands, nb_hands, sizeof(hand), hands_compare);

//	display_hands();

	for (int i = 0; i < nb_hands; i++) {
		sum += (i + 1) * hands[i].bid;
	}
	printf("  Sum = %lld\n", sum);
}


void part_1(void)
{
	part_function(1);
}

void part_2(void)
{
	part_function(2);
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
