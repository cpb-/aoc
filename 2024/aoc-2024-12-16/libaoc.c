
#include "libaoc.h"


static long long int *Primes = NULL;
static int Nb_primes = 0;


int isprime(long long int value)
{
	for (int i = 2; i < value; i ++)
		if (value %i == 0)
			return 0;
	return 1;
}


static void compute_primes_table(void)
{
	for (int i = 2; i < 20000; i++) {
		if (isprime(i)) {
			Primes = realloc(Primes, (Nb_primes + 1) * sizeof(long long int));
			Primes[Nb_primes] = i;
			Nb_primes ++;
		}
	}
}



long long int *compute_prime_factors(long long int num)
{
	long long int *list = NULL;
	int nb_primes = 0;
	int i = 0;

	if (Primes == NULL)
		compute_primes_table();

	while ((i < Nb_primes) && (Primes[i] <= num)) {
		if (num % Primes[i] == 0) {
			list = realloc(list, (nb_primes + 1) * sizeof(long long int));
			list[nb_primes] = Primes[i];
			nb_primes ++;
			num = num / Primes[i];
			continue;
		}
		i++;
	}
	list = realloc(list, (nb_primes + 1) * sizeof(long long int));
	list[nb_primes] = 0; // END
	nb_primes ++;

	return list;

}



long long int compute_gcd(long long int n1, long long int n2)
{
	long long int *primes = NULL;
	int nb_primes = 0;
	long long int gcd = 1;

	long long int *prime_factors_1 = compute_prime_factors(n1);
	long long int *prime_factors_2 = compute_prime_factors(n2);


	for (int i = 0; prime_factors_1[i] != 0; i++) {
		for (int j = 0; prime_factors_2[j] != 0; j ++) {
			if (prime_factors_1[i] == prime_factors_2[j]) {
				primes = realloc(primes, (nb_primes + 1) * sizeof(long long int));
				primes[nb_primes] = prime_factors_1[i];
				nb_primes ++;
				prime_factors_2[j] = -1; // already used
				break;
			}
		}
	}
	free(prime_factors_1);
	free(prime_factors_2);

	for (int i = 0; i < nb_primes; i ++)
		gcd *= primes[i];
	free(primes);

	return gcd;
}



long long int compute_lcm(long long int n1, long long int n2)
{
	return n1 * n2 / compute_gcd(n1, n2);
}



long long int compute_multiple_lcm(long long int *table, int nb)
{
	long long int lcm = 1;

	for (int i = 0; i < nb; i ++) {
		lcm = compute_lcm(table[i], lcm);
	}

	return lcm;
}




typedef struct s_node {
	void          *payload;
	struct s_node *prev;
	struct s_node *next
} node_t;

node_t *node_head = NULL;
node_t *node_tail = NULL;


void push(void *payload, size_t size)
{
	/* Insert on head */
	node_t *nd = malloc(sizeof(node_t));
	nd->payload = malloc(size);
	memcpy(nd->payload, payload, size);
	nd->next = node_head;
	node_head = nd;
	if (nd->next != NULL)
		nd->next->prev = nd;
	else
		node_tail = NULL;
}



int pull(void *payload, size_t size)
{
	/* Remove from tail (FIFO) */
	if (node_tail == NULL)
		return 0;

	node_t *nd = node_tail;
	node_tail = nd->prev;
	if (nd->prev != NULL)
		nd->prev->next = NULL;
	else
		node_head = NULL;
	memcpy(payload, nd->payload, size);
	free(nd->payload);
	free(nd);

	return 1;
}



int pop(void *payload, size_t size)
{
	/* Remove from HEAD (LIFO) */

	if (node_head == NULL)
		return 0;
	node_t *nd = node_head;
	node_head = nd->next;
	if (nd->next != NULL)
		nd->next-> = NULL;
	else
		node_tail = NULL;
	memcpy(payload, nd->payload, size);
	free(nd->payload);
	free(nd);

	return 1
}


