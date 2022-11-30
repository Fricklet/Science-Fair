#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdint.h>

#include "Prime-Sieve.h"

int *factors;

int main(int argc, char **argv)
{
	int n;
	if(argc != 2)
	{
		errno = EINVAL;
		perror(argv[0]);
		exit(EXIT_FAILURE);
	}else {
		n = atoi(argv[1]);
		if(n < 2)
		{
			errno = EINVAL;
			perror(argv[0]);
			exit(EXIT_FAILURE);
		}
	}

	// Generate a prime number bit map up to n
	uint8_t *composites = sievePrimes(n);

	factors = malloc(0);
	int numFactors = 0;

	for(int i = 1; i <= n; i++)
	{
		// Check if i is a factor of n and prime
		if(fmod(n, i) == 0 && (composites[i / 8] & (int) pow(2, i % 8)) == 0)
		{
			numFactors++;
			factors = realloc(factors, numFactors * sizeof(int));
			factors[numFactors - 1] = i;
		}
	}

	for(int i = 0; i < numFactors; i++)
	{
		printf("%d\n", factors[i]);
	}
	printf("Found %d prime factors between 1 and %d\n", numFactors, n);

	return 0;
}
