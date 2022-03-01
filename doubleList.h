/*
 * doubleList.h
 */

#ifndef DOUBLELIST_H_
#define DOUBLELIST_H_

#include "boardGame.h"

/**
 * This is a doubly linked list. each object is a node that holds pointers
 * to the previous node, the next node and the current boards.
 */
typedef struct gameList
{
	struct gameList *prev;
	boards* board;
	struct gameList *next;
}gameList;

/**
 * The function gets a node and it frees the gameList.
 */
void freegameList(gameList *freeNode);

/**
 * The function gets the board and the current node, and it adds a new node
 * to the list that holds the current board. the functions returns the new gameList (pointer on the new node).
 */
gameList* addgameList(gameList *node,boards* boards);

/**
 * The function gets the beginning node and it frees the gameList from a specific node till the end.
 */
void freeRightgameList(gameList *freeNode);

#endif /* DOUBLELIST_H_ */
