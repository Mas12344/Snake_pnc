#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

SDL_Window* window;
SDL_Renderer* renderer;


bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Error init SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Snake pnc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
	if (!window) {
		fprintf(stderr, "Error window create: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Error renderer create: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;

	SDL_Quit();
}
//input handling

void keyDownEvent(SDL_Event e) {

}

void keyUpEvent(SDL_Event e) {

}


int main() {
	if (!init()) {
		return -1;
	}

	bool running = true;
	while (running) {
		Uint64 start = SDL_GetPerformanceCounter();
		SDL_Event e;
		SDL_SetRenderDrawColor(renderer, 188, 188, 188, 255);
		SDL_RenderClear(renderer);
		/*============================
		event loop

		physics loop

		rendering loop
		=============================*/

		//event loop
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				keyDownEvent(e);
				break;
			case SDL_KEYUP:
				keyUpEvent(e);
			}
		}


		
		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedTime = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		SDL_Delay(floor(16.666f - elapsedTime));
	}
	
	quit();
	return 0;
}