n = tonumber(arg[1])

if not n or n < 2 then
	return 1
end

sievePrimes = require("Prime-Sieve_mod")

composites = sievePrimes(n)

numPrimes = 0
for i = 0, n do
	if composites[i] then
		--print(i, "Composite")
	else
		--print(i, "Prime")
		numPrimes = numPrimes + 1
	end
end

print(string.format("Found %d primes between 1 and %d", numPrimes, n))
