/*
 * boardStrc.c
 */

#include <stdio.h>
#include "boardStrc.h"
#include <stdlib.h>
#include "Checkers.h"
#include <string.h>

void freeGame(boardStrc* gameBoard){
	boardsFree(gameBoard->board);
	freegameList(gameBoard->moveList);
	free(gameBoard);
}

boardStrc* generateGame(boards* board, int valMarkErrors){
	boardStrc* newGameBoard = (boardStrc*)malloc(sizeof(boardStrc));
	memFuncError("malloc", newGameBoard);
	newGameBoard->markErrors = valMarkErrors;
	newGameBoard->board = board;
	newGameBoard->moveList = NULL;
	newGameBoard->moveList = addgameList(newGameBoard->moveList,newGameBoard->board);
	return newGameBoard;
}
