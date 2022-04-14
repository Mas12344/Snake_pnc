#include <stdio.h>
#include <SDL.h>

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = NULL;
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Error on window creation: %s\n", SDL_GetError());
		SDL_Quit();
		return -1;
	}
	SDL_UpdateWindowSurface(window);
	SDL_Delay(2000);
	SDL_DestroyWindow(window);

	
	return 0;
}