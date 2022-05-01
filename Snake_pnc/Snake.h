#pragma once
#ifndef SNAKE_H
#include <stdlib.h>

struct snakePart
{
	struct snakePart* prev;
	struct snakePart* next;
	int y;
	int x;
};

typedef struct snakePart SnakePart;

bool isPart(int _x, int _y, SnakePart* head) {
	SnakePart* tmp = head;
	while (tmp->next != NULL) {
		if (tmp->x == _x && tmp->y == _y) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

void removeEnd(SnakePart* head) {
	SnakePart* tmp = head;
	SnakePart* p = NULL;
	while (tmp->next != NULL) {
		p = tmp;
		tmp = tmp->next;
	}

	p->next = NULL;
	free(tmp);
}

void newEndPart(SnakePart* head, int _x, int _y) {
	SnakePart* newEnd = malloc(sizeof(SnakePart));
	SnakePart* tmp = head;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	
	tmp->next = newEnd;
	newEnd->x = _x;
	newEnd->y = _y;
	newEnd->prev = tmp;
	newEnd->next = NULL;
}

SnakePart* newFrontPart(SnakePart* head, int _x, int _y) {
	SnakePart* newFront = malloc(sizeof(SnakePart));
	newFront->next = head;
	newFront->prev = NULL;
	newFront->x = _x;
	newFront->y = _y;
	head->prev = newFront;

	return newFront;
}

SnakePart* createHead(int _x, int _y) {
	SnakePart* head = malloc(sizeof(SnakePart));
	head->x = _x;
	head->y = _y;
	head->prev = NULL;
	head->next = NULL;
	return head;
}

SnakePart* initSnake(int _x, int _y) {
	SnakePart* HEAD = createHead(_x, _y);
	int i;
	for (i = 1; i < 4; i++) {
		newEndPart(HEAD, _x, _y - i);
	}
	
	return HEAD;
}



#endif // !SNAKE_H
