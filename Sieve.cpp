#include <iostream>
#include <cmath>
#include <cstring>

#include "Sieve.hpp"	// Contains code for prime sieve; this file is just a driver

int main(int argc, char **argv)
{
	unsigned int n;
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
		// Print statement slows down code; omitted for testing
		//std::cout << i << ": " << (isPrime ? "Prime" : "Composite") << std::endl;
	}
	std::cout << "Found " << numPrimes << " primes between 0 and " << n << std::endl;

	delete[] composites;
	return 0;
}