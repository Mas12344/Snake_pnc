#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>
#include <time.h>

#include "Snake.h"
#include "Globals.h"
#include "Draw.h"

typedef struct fruit { int x; int y; } Fruit;

SDL_Window* window;
SDL_Renderer* renderer;

int** Board;
SnakePart* Head;
Fruit Owocek = { 5, 10 };
int lastDirection = DOWN;


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

	Head = initSnake(WIDTH / 2, HEIGHT / 2);
	srand(time(NULL));

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

void Update(int direction)
{
	int dx, dy;
	
	switch (direction)
	{
	case UP:
		if (lastDirection == DOWN) break;
		dx = 0;
		dy = -1;
		break;
	case DOWN:
		if (lastDirection == UP) break;
		dx = 0;
		dy = 1;
		break;
	case LEFT:
		if (lastDirection == RIGHT) break;
		dx = -1;
		dy = 0;
		break;
	case RIGHT:
		if (lastDirection == LEFT) break;
		dx = 1;
		dy = 0;
		break;
	default:
		fprintf(stderr, "nie powinno mnie tu byc\n");
	}
	int nx, ny;
	nx = Head->x + dx;
	ny = Head->y + dy;
	if (isPart(nx, ny, Head) || nx >= WIDTH || nx < 0 || ny >= HEIGHT || ny < 0)
	{
		return; // koniec gry wszedl w siebie debil
	}
	else
	{
		//tutaj dodac czesc z przodu (nx,ny)
		Head = newFrontPart(Head, nx, ny);
		if (!(nx == Owocek.x && ny == Owocek.y)) {
			removeEnd(Head);
		}
		else {
			Owocek.x = -1;
			Owocek.y = -1;
		}
	}
	//tutaj owoc musi byc dobry
	if (Owocek.x == -1) {
		int new_x = rand() % WIDTH;
		int new_y = rand() % HEIGHT;

		while (Board[new_y][new_x] != EMPTY) {
			new_x = rand() % WIDTH;
			new_y = rand() % HEIGHT;
		}

		Owocek.x = new_x;
		Owocek.y = new_y;
	}



	//1 czyscimy boarda
	for (int y = 0; y < HEIGHT; y++) {
		memset(Board[y], EMPTY, WIDTH * sizeof(*Board[y]));
	}
	//2 rysuj owoca
	Board[Owocek.y][Owocek.x] = FRUIT;
	//3 rysuj weza
	SnakePart* tmp = Head;
	while (tmp->next != NULL) {
		Board[tmp->y][tmp->x] = SNAKE;
		tmp = tmp->next;
	}

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

		Update(LEFT);

		DrawBoard(Board, renderer);
		
		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedTime = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		SDL_Delay(1000);
		
	}
	
	quit();
	return 0;
}