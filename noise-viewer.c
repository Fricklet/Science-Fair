/*
	This file requires the SDL library for visuals.
	gcc -o noise-viewer noise-viewer.c -lSDL2
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

int main(int argc, char **argv)
{
	if(argc != 4)
	{
		errno = EINVAL;
		perror(argv[0]);
		return 0;
	}
	int width, height;
	if((width = atoi(argv[1])) == 0)
	{
		errno = EINVAL;
		perror(argv[0]);
		return errno;
	}if((height = atoi(argv[2])) == 0)
	{
		errno = EINVAL;
		perror(argv[0]);
		return errno;
	}

	FILE *noiseFD = fopen(argv[3], "rb");
	if(noiseFD == NULL)
	{
		perror(argv[0]);
		return errno;
	}
	uint8_t *noise = calloc(width * height, sizeof(uint8_t));
	fread(noise, sizeof(uint8_t), width * height, noiseFD);

	printf("Read binary noise file: %s (W: %d H: %d)\n", argv[3], width, height);

	// Initialize video.
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Error initializing SDL video: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}
	
	SDL_Window *window;
	if((window = SDL_CreateWindow("Noise visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) == NULL)
	{
		fprintf(stderr, "Error initializing SDL window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Renderer *renderer;
	if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)) == NULL)
	{
		fprintf(stderr, "Error initializing SDL renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_Texture *noiseTx;
	{
		SDL_Surface *noiseSf = SDL_CreateRGBSurface(0, width, height, 24, 0, 0, 0, 0);
		SDL_LockSurface(noiseSf);
		
		uint8_t *pixelData = calloc(3, width * height);
		for(int i = 0; i < width * height; i++)
		{
			memset(pixelData + i * 3, noise[i], 3);		// Copy three bytes since this is 24-bit RGB color coding
		}
		memcpy(noiseSf->pixels, pixelData, width * height * 3);
		free(pixelData);
		noiseTx = SDL_CreateTextureFromSurface(renderer, noiseSf);
		SDL_FreeSurface(noiseSf);
		free(noise);
	}

	
	SDL_Rect src;
	memset(&src, 0, sizeof(src));
	src.w = width;	src.h = height;
	SDL_Rect dst;
	memcpy(&dst, &src, sizeof(src));

	bool running = true;
	while(running)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_QUIT:
					running = false;
					break;
			}
		}
		
		//SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, noiseTx, &src, &dst);

		SDL_RenderPresent(renderer);
		SDL_Delay(1);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}
