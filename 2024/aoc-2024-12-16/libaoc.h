
#ifndef LIB_AOC_H
#define LIB_AOC_H

int isprime(long long int value);

long long int *compute_prime_factors(long long int num);

long long int compute_gcd(long long int n1, long long int n2);

long long int compute_lcm(long long int n1, long long int n2);

long long int compute_multiple_lcm(long long int *table, int nb)


void push(void *payload, size_t size);
int pull(void *payload, size_t size); // FIFO. Return 1 on success.
int pop(void *payload, size_t size);  // LIFO. Return 1 on success.

#endif
