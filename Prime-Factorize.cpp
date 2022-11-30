#include <iostream>
#include <cmath>
#include <vector>

#include "Prime-Sieve.hpp"

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

	std::vector<int> factors;
	int numFactors = 0;

	for(int i = 1; i <= n; i++)
	{
		// Check if i is a factor of n and prime
		if(fmod(n, i) == 0 && (composites[i / 8] & (int) pow(2, i % 8)) == 0)
		{
			numFactors++;
			factors.push_back(i);
		}
	}

	for(int factor : factors)
	{
		std::cout << factor << std::endl;
	}
	std::cout << "Found " << numFactors << " prime factors between 1 and " << n << std::endl;

	return 0;
}
