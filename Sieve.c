#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "Sieve.h"	// Contains code for prime sieve; this file is just a driver

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

	uint8_t *composites = sievePrimes(n);

	int numPrimes = 0;
	for(unsigned int i = 0; i <= n; i++)
	{
		bool isPrime = (composites[i / 8] & (int) pow(2, i % 8)) == 0;
		if(isPrime)
		{
			numPrimes++;
		}
		// This print statement slows down the program by quite a bit
		// printf("%i: %s\n", i, isPrime ? "Prime" : "Composite");
	}
	printf("Found %d primes between 0 and %d\n", numPrimes, n); 

	free(composites);
	return 0;
}