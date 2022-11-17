import sys

def sievePrimes(n):
	if n < 2:
		exit(1);

	composites = [False for i in range(n + 1)]
	for i in range(2, n):
		if not composites[i]:
			for num in range(i + 1, n + 1):
				if num % i == 0:
					composites[num] = True
	return composites

if __name__ == "__main__":
	n = int(sys.argv[1])

	composites = sievePrimes(n)

	for i in range(n + 1):
		if composites[i]:
			print(f"{i} is composite")
		else:
			print(f"{i} is prime")