#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>

#include "Snake.h"
#include "Globals.h"
#include "Draw.h"

SDL_Window* window;
SDL_Renderer* renderer;

int** Board;
SnakePart* Head;


bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Error init SDL: %s\n", SDL_GetError());
		return false;
	}

	window = SDL_CreateWindow("Snake pnc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH*TILE_SIZE, HEIGHT*TILE_SIZE, SDL_WINDOW_SHOWN);
	if (!window) {
		fprintf(stderr, "Error window create: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		fprintf(stderr, "Error renderer create: %s\n", SDL_GetError());
		return false;
	}

	Board = malloc(HEIGHT * sizeof(*Board));
	if (!Board) {
		fprintf(stderr, "Error no memmory\n");
		return false;
	}
	for (int i = 0; i < HEIGHT; i++) {
		Board[i] = malloc(WIDTH * sizeof(*Board[i]));
		if (!Board[i]) {
			fprintf(stderr, "Error no memmory\n");
			return false;
		}
		memset(Board[i], 0, WIDTH * sizeof(*Board[i]));
	}

	Head = createHead(WIDTH / 2, HEIGHT / 2);

	return true;
}

void quit() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	
	for (int i = 0; i < HEIGHT; i++) {
		free(Board[i]);
	}
	free(Board);

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
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_KEYDOWN:
				keyDownEvent(e);
				break;
			case SDL_KEYUP:
				keyUpEvent(e);
				break;
			}
		}

		DrawBoard(Board, renderer);
		
		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedTime = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		
	}
	
	quit();
	return 0;
}