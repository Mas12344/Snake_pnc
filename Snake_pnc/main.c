#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "Snake.h"
#include "Globals.h"
#include "Draw.h"

typedef struct point { int x; int y; } Point;

SDL_Window* window;
SDL_Renderer* renderer;

int** Board;
SnakePart* Head;
Point Owocek = { 5, 12 };
Point Sciany[] = { {5, 4}, {5, 5}, {5, 6}, {5, 7} };
int ileScian = sizeof(Sciany) / sizeof(Sciany[0]);
int lastDirection = DOWN;

int UpKey    = 0;
int DownKey  = 1;
int LeftKey  = 0;
int RightKey = 0;

int wynik = 0;

bool isOnWall(int x, int y) {
	for (int i = 0; i < ileScian; i++) {
		if (x == Sciany[i].x && y == Sciany[i].y) {
			return true;
		}
	}
	return false;
}

bool init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Error init SDL: %s\n", SDL_GetError());
		return false;
	}

	if (TTF_Init() < 0) {
		fprintf(stderr, "Error init TTF: %s\n", TTF_GetError());
		return false;
	}

	window = SDL_CreateWindow("Snake pnc", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH*TILE_SIZE + BLANK_SPACE_PX, HEIGHT*TILE_SIZE, SDL_WINDOW_SHOWN);
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
		memset(Board[i], EMPTY, WIDTH * sizeof(*Board[i]));
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

int keyDownEvent(SDL_Event e) {
	
	switch (e.key.keysym.scancode) {
	case SDL_SCANCODE_DOWN:
		DownKey = UpKey = LeftKey = RightKey = 0;
		DownKey = 1;
		//lastDirection = DOWN;
		break;
	case SDL_SCANCODE_LEFT:
		DownKey = UpKey = LeftKey = RightKey = 0;
		LeftKey = 1;
		//lastDirection = LEFT;
		break;
	case SDL_SCANCODE_UP:
		DownKey = UpKey = LeftKey = RightKey = 0;
		UpKey = 1;
		//lastDirection = UP;
		break;
	case SDL_SCANCODE_RIGHT:
		DownKey = UpKey = LeftKey = RightKey = 0;
		RightKey = 1;
		//lastDirection = RIGHT;
		break;
	case SDL_SCANCODE_ESCAPE:
		return 1;
	}
	return 0;
}

bool Update(int direction)
{
	int dx=0, dy=0;
	
	switch (direction)
	{
	case UP:
		dx = 0;
		dy = -1;
		if (lastDirection == DOWN) {
			dy *= -1;
		}
		else lastDirection = UP;
		break;
	case DOWN:
		dx = 0;
		dy = 1;
		if (lastDirection == UP) {
			dy *= -1;
		}
		else lastDirection = DOWN;
		break;
	case LEFT:
		dx = -1;
		dy = 0;
		if (lastDirection == RIGHT) {
			dx *= -1;
		}
		else lastDirection = LEFT;
		break;
	case RIGHT:
		dx = 1;
		dy = 0;
		if (lastDirection == LEFT) {
			dx *= -1;
		}
		else lastDirection = RIGHT;
		break;
	default:
		fprintf(stderr, "nie powinno mnie tu byc\n");
	}
	int nx, ny;
	nx = Head->x + dx;
	ny = Head->y + dy;
	if (isPart(nx, ny, Head) || isOnWall(nx, ny) || nx >= WIDTH || nx < 0 || ny >= HEIGHT || ny < 0)
	{
		return false;
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
			wynik += 10;
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

	//2 stawiaj sciany
	for (int i = 0; i < ileScian; i++) {
		Board[Sciany[i].y][Sciany[i].x] = WALL;
	}

	//3 rysuj owoca
	Board[Owocek.y][Owocek.x] = FRUIT;

	//4 rysuj weza
	SnakePart* tmp = Head;
	while (tmp->next != NULL) {
		Board[tmp->y][tmp->x] = SNAKE;
		tmp = tmp->next;
	}

	return true;
}


int main() {
	if (!init()) {
		return -1;
	}
	
	bool running = true;
	float elapsedTime = 0;
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
			}
		}

		//physics loop
		if (elapsedTime >= 70.0f) {

			elapsedTime = 0;
			if (DownKey) {
				running = Update(DOWN);
			}
			else if (LeftKey) {
				running = Update(LEFT);
			}
			else if (UpKey) {
				running = Update(UP);
			}
			else if (RightKey) {
				running = Update(RIGHT);
			}
		}
		//rendering loop
		DrawBoard(Board, wynik, renderer);
		SDL_RenderPresent(renderer);

		Uint64 end = SDL_GetPerformanceCounter();
		
		elapsedTime += (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;
		//printf("lasdir - %d\n", lastDirection);
		//SDL_Delay(100);
		
	}
	
	quit();
	return 0;
}


// Lambert Lambert, ty *****!
