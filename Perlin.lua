NOISE_WIDTH = 512
NOISE_HEIGHT = 512
NOISE_OCTAVES = 8

PERMUTATION_TABLE_SIZE = 256

vector2 = {
	x = 0,
	y = 0,
	dot = function(self, other)
		return self.x * other.x + self.y * other.y
	end
}
vector2.__index = vector2
vector2.__call = function(self, x, y)	-- Constructor
	return setmetatable({x = x, y = y}, vector2)
end
setmetatable(vector2, vector2)

local permutationTable
p = {}

function getConstantVector(permutation)
	h = permutation % 4
	v = vector2()	-- Lua doesn't care if a parameter is not given, this will create a vector2 with x and y as nil.
	if h == 0 then
		v.x = 1		v.y = 1
	elseif h == 1 then
		v.x = -1	v.y = 1
	elseif h == 2 then
		v.x = -1	v.y = -1
	else	-- 3
		v.x = 1		v.y = -1
	end
	return v
end

function fade(t)
	return ((6 * t - 15) * t + 10) * t * t * t
end

function lerp(t, a1, a2)
	return a1 + t * (a2 - a1)
end

-- Native Lua does not provide this function for whatever reason.
function round(x)
	if x - math.floor(x) < .5 then
		return math.floor(x)
	else
		return math.ceil(x)
	end
end

function perlin(x, y)
	local xUnit = math.floor(x % 256)
	local yUnit = math.floor(y % 256)

	local xRel = x - math.floor(x)
	local yRel = y - math.floor(y)

	local bottomLeft = vector2(xRel, yRel)
	local bottomRight = vector2(xRel - 1, yRel)
	local topLeft = vector2(xRel, yRel - 1)
	local topRight = vector2(xRel - 1, yRel - 1)

	-- Get permutations, add one since Lua tables start at one instead of zero.
	local pBottomLeft = p[p[xUnit + 1] + yUnit + 1]
	local pBottomRight = p[p[xUnit + 2] + yUnit + 1]
	local pTopLeft = p[p[xUnit + 1] + yUnit + 2]
	local pTopRight = p[p[xUnit + 2] + yUnit + 2]

	local dotBottomLeft = bottomLeft:dot(getConstantVector(pBottomLeft))
	local dotBottomRight = bottomRight:dot(getConstantVector(pBottomRight))
	local dotTopLeft = topLeft:dot(getConstantVector(pTopLeft))
	local dotTopRight = topRight:dot(getConstantVector(pTopRight))

	local xFade = fade(xRel)
	local yFade = fade(yRel)

	return lerp(xFade,
		lerp(yFade, dotBottomLeft, dotTopLeft),
		lerp(yFade, dotBottomRight, dotTopRight))
end

permutationsFD = io.open("Permutations", "rb")
if not permutationsFD then
	error("Unable to open permutation file")
	return 1
end
permutationTable = permutationsFD:read(PERMUTATION_TABLE_SIZE)
permutationsFD:close()

for i = 1, PERMUTATION_TABLE_SIZE * 2 do
	p[i] = string.byte(permutationTable:sub((i - 1) % PERMUTATION_TABLE_SIZE + 1, (i - 1) % PERMUTATION_TABLE_SIZE + 1))	-- Subtract and add one since Lua tables start at 1, and modulus overflows to zero.
end

noise = {}
for y = 0, NOISE_HEIGHT - 1 do
	for x = 0, NOISE_WIDTH - 1 do
		local accumulator = 0
		local freq = 0.005
		for o = 0, NOISE_OCTAVES - 1 do
			local n = perlin(x * freq, y * freq) * 2 ^ -o
			accumulator = accumulator + n
			freq = freq * 2
		end
		accumulator = (accumulator + 1) / 2
		local val = math.min(round(255 * accumulator), 255)
		noise[y * NOISE_WIDTH + x + 1] = val
	end
end

noiseFD = io.open("Noise", "wb")
if not noiseFD then
	error("Unable to write a noise output file")
	return 1
end

for i, v in ipairs(noise) do
	if noiseFD:write(string.char(v)) ~= noiseFD then
		error("Unable to write to the noise output file")
		return 1
	end
end
noiseFD:close()