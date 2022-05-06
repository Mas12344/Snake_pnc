#pragma once
#ifndef GLOBALS_H

#define HEIGHT 20
#define WIDTH 20

#define TILE_SIZE 30

enum tile {
	EMPTY,
	SNAKE,
	FRUIT,
	WALL
};

enum dir {
	UP,
	DOWN,
	LEFT,
	RIGHT
};
#endif // !GLOBALS_H
