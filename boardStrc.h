/*
 * boardStrc.h
 */
#include "doubleList.h"

#ifndef BOARDSTRC_H_
#define BOARDSTRC_H_

/**
 * The boardStrc holds: the list which contains the user moves, the boards of the game and
 * the mark errors parameter.
 */
typedef struct boardStrc{
	gameList* moveList;
	boards* board;
	int markErrors;
}boardStrc;

/**
 * The function gets the boardStrc and frees it.
 */
void freeGame(boardStrc* gameBoard);

/**
 * This function gets the game's boards and the value of the mark error parameter.
 * it creates the boardStrc struct, and returns a new boardStrc with initialized parameteres.
 */

boardStrc* generateGame(boards* board, int valMarkErrors);



#endif /* BOARDSTRC_H_ */
