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
Point Owocek = { 11, 11 };

Point Sciany1[] = { {0, 10} ,{1, 10}, {2, 10}, {3, 10}, {4, 10}, {5,10} ,{6, 10}, {7, 10}, {8, 10}, {9, 10}, {11,10} ,{12, 10}, {13, 10}, {14, 10}, {15, 10}, {16,10} ,{17, 10}, {18, 10}, {19, 10} };
Point Sciany2[] = { {9, 9}, {9, 10}, {10, 9}, {10, 10} , {3, 6}, {4, 6}, {5, 6}, {6, 6}, {6, 3}, {6, 4}, {6, 5} , {16, 6}, {15, 6}, {14, 6}, {13, 6}, {13, 5}, {13, 4}, {13, 3} , {6, 16}, {6, 15}, {6, 14}, {6, 13}, {5, 13}, {4, 13}, {3, 13} , {16, 13}, {15, 13}, {14, 13}, {13, 13}, {13, 14}, {13, 15}, {13, 16} };
Point Sciany3[] = { {5, 7}, {4, 8}, {5, 8}, {6, 8}, {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9} , {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {4, 11}, {5, 11}, {6, 11}, {5, 12}  ,  {14, 7}, {13, 8}, {14, 8}, {15, 8}, {12, 9}, {13, 9}, {14, 9}, {15, 9}, {16, 9} , {12, 10}, {13, 10}, {14, 10}, {15, 10}, {16, 10}, {13, 11}, {14, 11}, {15, 11}, {14, 12}      ,     {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {3, 1}, {4, 0}       ,      {19, 0}, {19, 1}, {19, 2}, {19, 3}, {19, 4}, {18, 0}, {18, 1}, {18, 2}, {18, 3}, {17, 0}, {17, 1}, {17, 2}, {16, 0}, {16, 1}, {15, 0}       ,      {0, 19}, {1, 19}, {2, 19}, {3, 19}, {4, 19}, {0, 18}, {1, 18}, {2, 18}, {3, 18}, {0, 17}, {1, 17}, {2, 17}, {0, 16}, {1, 16}, {0, 15}        ,      {15, 19}, {16, 19}, {17, 19}, {18, 19}, {19, 19}, {16, 18}, {17, 18}, {18, 18}, {19, 18}, {17, 17}, {18, 17}, {19, 17}, {18, 16}, {19, 16}, {19, 15} };

Point* Poziomy[3] = {
	Sciany1,
	Sciany2,
	Sciany3
};

int RozmiarPoziomow[3] = {
	LEN(Sciany1),
	LEN(Sciany2),
	LEN(Sciany3)
};

int obecnyPoziom = 0;

int lastDirection = DOWN;

int UpKey    = 0;
int DownKey  = 1;
int LeftKey  = 0;
int RightKey = 0;

int wynik = 0;

bool pause = 1;

bool isOnWall(int x, int y) {
	Point* Sciany = Poziomy[obecnyPoziom];
	int ileScian = RozmiarPoziomow[obecnyPoziom];

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
		break;
	case SDL_SCANCODE_LEFT:
		DownKey = UpKey = LeftKey = RightKey = 0;
		LeftKey = 1;
		break;
	case SDL_SCANCODE_UP:
		DownKey = UpKey = LeftKey = RightKey = 0;
		UpKey = 1;
		break;
	case SDL_SCANCODE_RIGHT:
		DownKey = UpKey = LeftKey = RightKey = 0;
		RightKey = 1;
		break;
	case SDL_SCANCODE_1:
		obecnyPoziom = 0;
		break;
	case SDL_SCANCODE_2:
		obecnyPoziom = 1;
		break;
	case SDL_SCANCODE_3:
		obecnyPoziom = 2;
		break;
	case SDL_SCANCODE_P:
		pause = !pause;
		break;
	case SDL_SCANCODE_ESCAPE:
		return 1;
	}
	return 0;
}

bool Update(int direction)
{
	int dx=0, dy=0;
	Point* Sciany = Poziomy[obecnyPoziom];
	int ileScian = RozmiarPoziomow[obecnyPoziom];
	
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
	if (isPart(nx, ny, Head) || isOnWall(nx, ny, Sciany, ileScian) || nx >= WIDTH || nx < 0 || ny >= HEIGHT || ny < 0)
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
	while (pause) {
		SDL_Event e;
		SDL_SetRenderDrawColor(renderer, 188, 188, 188, 255);
		SDL_RenderClear(renderer);
		DrawChoice(obecnyPoziom, renderer);
		if (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_KEYDOWN:
				keyDownEvent(e);
				break;
			}
		}
		SDL_Delay(50);
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
		
		if (!pause) {
			elapsedTime += (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.f;
		}
		
		
	}
	
	quit();
	return 0;
}

