#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		errno = EINVAL;
		perror(argv[0]);
		return errno;
	}
	int n;
	if((n = atoi(argv[1])) == 0)
	{
		errno = EINVAL;
		perror(argv[0]);
		return errno;
	}

	uint8_t *source = calloc(n, sizeof(uint8_t));
	for(int i = 0; i < n; i++)
	{
		source[i] = i;
	}

	time_t t;
	srand((unsigned) time(&t));
	for(int i = 0; i < n; i++)
	{
		int index = rand() % n;
		printf("%d\n", index);
		int originV = source[i];
		source[i] = source[index];
		source[index] = originV;
	}
	
	FILE *fp = fopen("Permutations", "wb");
	if(fp == NULL)
	{
		perror(argv[0]);
		return errno;
	}
	fwrite(source, sizeof(uint8_t), n, fp);
	fclose(fp);
	free(source);

	return 0;
}