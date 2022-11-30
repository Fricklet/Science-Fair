function sievePrimes(n)
	composites = {}
	if n < 2 then
		return 1
	end

	for i = 2, n do
		if not composites[i] then
			for num = i + 1, n do
				if num % i == 0 then
					composites[num] = true
				end
			end
		end
	end

	return composites
end

return sievePrimes