n = tonumber(arg[1])

sievePrimes = require("Prime-Sieve_mod")

composites = sievePrimes(n)

factors = {}

for i = 1, n do
	-- Check if i is a factor of n and prime
	if n % i == 0 and not composites[i] then
		factors[#factors + 1] = i
		print(i)
	end
end

print(string.format("Found %d prime factors between 1 and %d", #factors, n))
