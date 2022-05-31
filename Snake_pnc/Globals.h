#pragma once
#ifndef GLOBALS_H

#define LEN(X) (sizeof(X) / sizeof(X[0]))

#define HEIGHT 20
#define WIDTH 20

#define BLANK_SPACE_PX 300

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
