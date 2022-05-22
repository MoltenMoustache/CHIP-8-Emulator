#include <stdio.h>
#include "SDL.h"
#include "chip8.h"

int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* texture = NULL;
	chip8* chip = new chip8();

	printf("Intialising SDL...\n");
	// SDL returns 0 if successfully initialised
	if (SDL_Init(SDL_INIT_EVERYTHING != 0))
	{
		printf("Could not initialize SDL! %s.\n", SDL_GetError());
		return -1;
	}
	printf("SDL initialised!\n");

	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1024, 512, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return -2;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

	chip->LoadROM("C:/Users/Josh/Downloads/IBM Logo.ch8");

	// Emulator Loop
	while (true)
	{
		chip->EmulateCycle();

		SDL_UpdateTexture(texture, nullptr, chip->gfx, sizeof(chip->gfx[0]) * 64);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) exit(0);
		}
	}

	printf("Quitting SDL...\n");
	SDL_Quit();

	return 0;
}