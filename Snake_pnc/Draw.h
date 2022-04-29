#pragma once
#ifndef DRAW_H
#include <SDL.h>
#include "Globals.h"

void DrawBoard(int** board, SDL_Renderer* renderer) {
	SDL_Rect r;
	int y, x;
	r.w = TILE_SIZE;
	r.h = TILE_SIZE;
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			r.x = x * TILE_SIZE;
			r.y = y * TILE_SIZE;
			
			switch (board[y][x]) {
			case FRUIT:
				SDL_SetRenderDrawColor(renderer, 200, 80, 15, 255);
				break;
			case SNAKE:
				SDL_SetRenderDrawColor(renderer, 60, 200, 70, 255);
				break;
			case EMPTY:
				if ((y+x)%2==0) {
					SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
				}
				break;

			}
			SDL_RenderFillRect(renderer, &r);
			
		}
	}
	SDL_RenderPresent(renderer);
}


#endif // !DRAW_H
