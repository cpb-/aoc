#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



int is_all_zero(long long int *list, int nb_items)
{
	for (int i = 0; i < nb_items; i ++)
		if (list[i] != 0)
			return 0;
	return 1;
}



long long int *compute_differences(long long int *list, int nb_items)
{
	long long int *differences = malloc((nb_items - 1) * sizeof(long long int));
	int i;

	if (differences == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < nb_items - 1; i ++)
		differences [i] = list[i + 1] - list[i];

	return differences;
}



long long int extrapolate_next_item_from_list(long long int *list, int nb_items)
{
	long long int *differences;
	long long int value;

	if (is_all_zero(list, nb_items))
		return 0;

	differences = compute_differences(list, nb_items);
	value = list[nb_items - 1] + extrapolate_next_item_from_list(differences, nb_items - 1);
	free(differences);

	return value;
}



long long int extrapolate_previous_item_from_list(long long int *list, int nb_items)
{
	long long int *differences;
	long long int value;

	if (is_all_zero(list, nb_items))
		return 0;

	differences = compute_differences(list, nb_items);
	value = list[0] - extrapolate_previous_item_from_list(differences, nb_items - 1);
	free(differences);

	return value;
}



long long int extrapolate_next_item_from_line(const char *buffer)
{
	long long int *list = NULL;
	int nb_items;
	int offset = 0;
	long long int value;
	int n;

	while (sscanf(&(buffer[offset]), "%lld %n", &value, &n) == 1) {
		offset += n;
		list = realloc(list, (nb_items + 1) * sizeof(long long int));
		if (list == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		list[nb_items] = value;
		nb_items ++;
	}
	value = extrapolate_next_item_from_list(list, nb_items);
	free(list);

	return value;
}



long long int extrapolate_previous_item_from_line(const char *buffer)
{
	long long int *list = NULL;
	int nb_items;
	int offset = 0;
	long long int value;
	int n;

	while (sscanf(&(buffer[offset]), "%lld %n", &value, &n) == 1) {
		offset += n;
		list = realloc(list, (nb_items + 1) * sizeof(long long int));
		if (list == NULL) {
			perror("realloc");
			exit(EXIT_FAILURE);
		}
		list[nb_items] = value;
		nb_items ++;
	}
	value = extrapolate_previous_item_from_list(list, nb_items);
	free(list);

	return value;
}



void part_1(void)
{
	char buffer[1024];
	long long int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL)
		sum += extrapolate_next_item_from_line(buffer);


	printf(" Sum = %lld\n", sum);
}



void part_2(void)
{
	char buffer[1024];
	long long int sum = 0;

	while (fgets(buffer, 1024, stdin) != NULL)
		sum += extrapolate_previous_item_from_line(buffer);


	printf(" Sum = %lld\n", sum);
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
