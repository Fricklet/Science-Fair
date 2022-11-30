#pragma once

uint8_t *sievePrimes(unsigned int n)
{
	if(n < 2)
	{
		return NULL;
	}

	uint8_t *composites = malloc(ceil(n / 8));
	
	for(unsigned int i = 2; i < n; i++)
	{
		// Check if this number is not marked composite (0)
		if((composites[i / 8] & (int) pow(2, i % 8)) == 0)
		{
			for(unsigned int num = i + 1; num <= n; num++)
			{
				if(num % i == 0)
				{
					composites[num / 8] |= (int) pow(2, num % 8);
				}
			}
		}
	}
	return composites;
}