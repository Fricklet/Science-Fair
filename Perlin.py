import math

NOISE_WIDTH = 512
NOISE_HEIGHT = 512
NOISE_OCTAVES = 8

PERMUTATION_TABLE_SIZE = 256

class vector2:
	x = 0
	y = 0
	def __init__(self, x, y):
		self.x = x
		self.y = y
	def dot(self, other):
		return self.x * other.x + self.y * other.y

def getConstantVector(permutation):
	h = permutation % 4
	v = vector2(0, 0)
	if h == 0:
		v.x = 1
		v.y = 1
	elif h == 1:
		v.x = -1
		v.y = 1
	elif h == 2:
		v.x = -1
		v.y = -1
	else:
		v.x = 1
		v.y = -1
	return v

def fade(t):
	return ((6 * t - 15) * t + 10) * t * t * t

def lerp(t, a1, a2):
	return a1 + t * (a2 - a1)

def perlin(x, y):
	xUnit = math.floor(x % 256)
	yUnit = math.floor(y % 256)
	
	xRel = x - math.floor(x)
	yRel = y - math.floor(y)
	
	bottomLeft = vector2(xRel, yRel)
	bottomRight = vector2(xRel - 1, yRel)
	topLeft = vector2(xRel, yRel - 1)
	topRight = vector2(xRel - 1, yRel - 1)
	
	pBottomLeft = p[p[xUnit] + yUnit]
	pBottomRight = p[p[xUnit + 1] + yUnit]
	pTopLeft = p[p[xUnit] + yUnit + 1]
	pTopRight = p[p[xUnit + 1] + yUnit + 1]
	
	dotBottomLeft = bottomLeft.dot(getConstantVector(pBottomLeft))
	dotBottomRight = bottomRight.dot(getConstantVector(pBottomRight))
	dotTopLeft = topLeft.dot(getConstantVector(pTopLeft))
	dotTopRight = topRight.dot(getConstantVector(pTopRight))
	
	xFade = fade(xRel)
	yFade = fade(yRel)
	
	return lerp(xFade,
		lerp(yFade, dotBottomLeft, dotTopLeft),
		lerp(yFade, dotBottomRight, dotTopRight))

if __name__ == "__main__":
	permutationsTable = []
	permutationsFD = open("Permutations", "rb")
	for i in range(PERMUTATION_TABLE_SIZE):
		permutationsTable.append(ord(permutationsFD.read(1)))
	permutationsFD.close()

	p = [permutationsTable[i % PERMUTATION_TABLE_SIZE] for i in range(PERMUTATION_TABLE_SIZE * 2)]	# Python magic: Fills p with two copies of permutationsTable

	noise = []
	for y in range(NOISE_HEIGHT):
		for x in range(NOISE_WIDTH):
			accumulator = 0
			freq = 0.005
			for o in range(NOISE_OCTAVES):
				n = perlin(x * freq, y * freq) * 2 ** -o
				accumulator += n
				freq *= 2
			accumulator += 1
			accumulator /= 2
			val = min(round(255 * accumulator), 255)
			noise.append(val)
	
	noiseFD = open("Noise", "wb")
	noiseFD.write(bytes(noise))
	noiseFD.close()