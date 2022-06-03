#pragma once
#ifndef SNAKE_H
#include <stdbool.h>

struct snakePart
{
	struct snakePart* prev;
	struct snakePart* next;
	int y;
	int x;
};

typedef struct snakePart SnakePart;

bool isPart(int _x, int _y, SnakePart* head);

void removeEnd(SnakePart* head);

void newEndPart(SnakePart* head, int _x, int _y);

SnakePart* newFrontPart(SnakePart* head, int _x, int _y);

SnakePart* createHead(int _x, int _y);

SnakePart* initSnake(int _x, int _y);



#endif // !SNAKE_H
