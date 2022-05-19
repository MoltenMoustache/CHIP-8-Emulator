#include <stdio.h>
#include "SDL.h"
#include "chip8.h"

int main(int argc, char* args[])
{
	SDL_Window* window = NULL;
	chip8* chip = NULL;

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

	// Emulator Loop
	while (true)
	{
		chip->EmulateCycle();
	}

	printf("Quitting SDL...\n");
	SDL_Quit();

	return 0;
}