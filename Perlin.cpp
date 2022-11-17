// Thanks to https://adrianb.io/2014/08/09/perlinnoise.html and https://rtouti.github.io/graphics/perlin-noise-algorithm for providing insight and code on this complex algorithm. I wouldn't have been able to do this without lots of help!

#include <iostream>
#include <fstream>
#include <cerrno>
#include <cmath>
#include <cstdint>

#define NOISE_WIDTH 512
#define NOISE_HEIGHT 512
#define NOISE_OCTAVES 8

#define PERMUTATION_TABLE_SIZE 256

class vector2
{
public:
	double x, y;
	double dot(vector2 other)
	{
		return x * other.x + y * other.y;
	}
};

uint8_t *permutationTable;
uint8_t *p;

struct vector2 getConstantVector(uint8_t permutation)
{
	uint8_t h = permutation & 0b11;		// Get first two bits
	vector2 v;
	switch(h)
	{
		case 0:
			v.x = 1.0;		v.y = 1.0;
			break;
		case 1:
			v.x = -1.0;	v.y = 1.0;
			break;
		case 2:
			v.x = -1.0;	v.y = -1.0;
		default:	// 3
			v.x = 1.0;		v.y = -1.0;
	}
	return v;
}


// Ken Perlin defines this function as 6t^5 - 15t^4 + 10t^3
double fade(double t)
{
	return ((6 * t - 15) * t + 10) * t * t * t;
}

double lerp(double t, double a1, double a2)
{
	return a1 + t * (a2 - a1);
}

double perlin(double x, double y)
{
	int xUnit = (int) x % 256;	// Will round down x and y and lock them between 0 and 255, the range of Perlin noise
	int yUnit = (int) y % 256;	//

	double xRel = x - (int) x;	// Get the x and y values relative to their unit
	double yRel = y - (int) y;	//

	vector2 bottomLeft; bottomLeft.x = xRel; bottomLeft.y = yRel;
	vector2 bottomRight; bottomRight.x = xRel - 1.0; bottomRight.y = yRel;
	vector2 topLeft; topLeft.x = xRel; topLeft.y = yRel - 1.0;
	vector2 topRight; topRight.x = xRel - 1.0; topRight.y = yRel - 1.0;

	// Get the permutations of the unit's corners.
	uint8_t pBottomLeft, pBottomRight, pTopLeft, pTopRight;
	pBottomLeft = p[p[xUnit] + yUnit];
	pBottomRight = p[p[xUnit + 1] + yUnit];
	pTopLeft = p[p[xUnit] + yUnit + 1];
	pTopRight = p[p[xUnit + 1] + yUnit + 1];

	double dotBottomLeft = bottomLeft.dot(getConstantVector(pBottomLeft));
	double dotBottomRight = bottomRight.dot(getConstantVector(pBottomRight));
	double dotTopLeft = topLeft.dot(getConstantVector(pTopLeft));
	double dotTopRight = topRight.dot(getConstantVector(pTopRight));

	// Interpolation
	double xFade = fade(xRel);	// Distort the noise by making it gravitate a little bit towards the edges of the unit
	double yFade = fade(yRel);	//

	// Linearly interpolate the linear interpolations of the right and left together to get a final value.
	return lerp(xFade,
		lerp(yFade, dotBottomLeft, dotTopLeft),
		lerp(yFade, dotBottomRight, dotTopRight));
}

int imin(int a, int b)
{
	if(a > b)
	{
		return b;
	}else {
		return a;
	}
}

int main(int argc, char **argv)
{
	permutationTable = new uint8_t[PERMUTATION_TABLE_SIZE];
	std::ifstream permutationFD("Permutations", std::ios::in | std::ios::binary);
	if(!permutationFD.is_open())
	{
		perror(argv[0]);
		return errno;
	}
	permutationFD.read((char *) permutationTable, PERMUTATION_TABLE_SIZE);
	permutationFD.close();

	// The reason the referenced permutation table is twice the size as the regular one is to prevent overflow. i.e. let x = PERMUTATION_TABLE_SIZE, if p[x + 1] is accessed (see hash function), there will be an overflow, and the program will break.
	p = new uint8_t[PERMUTATION_TABLE_SIZE * 2];
	for(int i = 0; i < PERMUTATION_TABLE_SIZE * 2; i++)
	{
		p[i] = permutationTable[i % PERMUTATION_TABLE_SIZE];
	}

	uint8_t *noise = new uint8_t[NOISE_WIDTH * NOISE_HEIGHT];
	for(int y = 0; y < NOISE_HEIGHT; y++)
	{
		for(int x = 0; x < NOISE_WIDTH; x++)
		{
			double accumulator = 0;
			double freq = 0.005;
			for(int o = 0; o < NOISE_OCTAVES; o++)
			{
				double n = perlin(x * freq, y * freq) * pow(2, -o);
				accumulator += n;
				freq *= 2;
			}
			accumulator += 1;
			accumulator /= 2;
			uint8_t val = imin(round(255 * accumulator), 255);
			noise[y * NOISE_WIDTH + x] = val;	// Add it to the noise
		}
	}

	std::ofstream noiseFD("Noise", std::ios::out | std::ios::binary);
	if(!noiseFD.is_open())
	{
		perror(argv[0]);
		return errno;
	}
	noiseFD.write((char *) noise, NOISE_WIDTH * NOISE_HEIGHT);
	noiseFD.close();

	delete[] permutationTable;	delete[] p;	delete[] noise;

	return 0;
}