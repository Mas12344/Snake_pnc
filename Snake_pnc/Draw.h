#pragma once
#ifndef DRAW_H
#include <SDL.h>
#include "Globals.h"

void DrawBoard(int** board, int score_val, SDL_Renderer* renderer);

void DrawChoice(int choice, SDL_Renderer* renderer);
#endif // !DRAW_H
