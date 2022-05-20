#pragma once
#ifndef DRAW_H
#include <string.h>
#include <SDL.h>
#include "Globals.h"

void DrawBoard(int** board, int score_val, SDL_Renderer* renderer) {
	SDL_Rect r, mr;
	int y, x;
	mr.w = TILE_SIZE - TILE_SIZE / 5;
	mr.h = TILE_SIZE - TILE_SIZE / 5;

	r.w = TILE_SIZE;
	r.h = TILE_SIZE;

	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
	SDL_RenderClear(renderer);


	// board
	for (y = 0; y < HEIGHT; y++) {
		for (x = 0; x < WIDTH; x++) {
			r.x = x * TILE_SIZE;
			r.y = y * TILE_SIZE;
			
			switch (board[y][x]) {
			case FRUIT:
				SDL_SetRenderDrawColor(renderer, 200, 80, 15, 255);
				break;
			case SNAKE:
				mr.x = r.x + TILE_SIZE / 10;
				mr.y = r.y + TILE_SIZE / 10;
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
			case WALL:
				SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
				break;
			}
			SDL_RenderFillRect(renderer, &r);
			SDL_SetRenderDrawColor(renderer, 60, 140, 70, 255);
			SDL_RenderFillRect(renderer, &mr);
			
		}
	}

	// text
	char points[256] = "Punkty: ";
	char score[12];
	sprintf(score, "%d", score_val);
	strcat(points, score);


	TTF_Font* Sans = TTF_OpenFont("OpenSans-Regular.ttf", 128);

	SDL_Color White = { 255, 255, 255 };

	SDL_Surface* s_Message = TTF_RenderText_Solid(Sans, points, White);

	SDL_Texture* t_Message = SDL_CreateTextureFromSurface(renderer, s_Message);

	SDL_Rect Message_rect;
	Message_rect.x = WIDTH * TILE_SIZE + 20;
	Message_rect.y = 20;
	Message_rect.w = BLANK_SPACE_PX - 40;
	Message_rect.h = 45;

	int w = SDL_RenderCopy(renderer, t_Message, NULL, &Message_rect);
	//printf("czcionka: %d\n", w);

	SDL_FreeSurface(s_Message);
	SDL_DestroyTexture(t_Message);
	TTF_CloseFont(Sans);


	SDL_RenderPresent(renderer);
}


#endif // !DRAW_H
