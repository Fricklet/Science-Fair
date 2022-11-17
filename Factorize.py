import Sieve
import sys

n = int(sys.argv[1])
composites = Sieve.sievePrimes(n)

factors = []

for i in range(1, n):
	if n % i == 0 and not composites[i]:
		factors.append(i)
		print(i)

print(f"Found {len(factors)} prime factors between 1 and {n}")