
#ifndef LIB_AOC_H
#define LIB_AOC_H

int isprime(long long int value);

long long int *compute_prime_factors(long long int num);

long long int compute_gcd(long long int n1, long long int n2);

long long int compute_lcm(long long int n1, long long int n2);

long long int compute_multiple_lcm(long long int *table, int nb);

#endif
