/*
 * boardGame.c
 */


#include <stdio.h>
#include "Checkers.h"
#include <stdlib.h>
#include "boardGame.h"
#include <string.h>

void boardsFree(boards *board){
	free(board->blockFixed);
	free(board->boardFixed);
	free(board->blockSolution);
	free(board->boardSolution);
	free(board->blockMain);
	free(board->boardMain);
	free(board->blockEror);
	free(board->boardError);
	free(board);
}

void boardsAllocate(int row, int col, int **block, int ***matBoards){
	int ind;

	*block = (int*)calloc(row*col*row*col, sizeof(int));
	memFuncError("calloc", block);
	*matBoards = (int**)calloc(row*col, sizeof(int*));
	memFuncError("calloc", matBoards);
	for(ind=0; ind<row*col; ind++){
		(*matBoards)[ind]=*block+row*col*ind;
	}
}

boards *createBoard(int row, int col){
	boards *board = (boards*)malloc(sizeof(boards));
	memFuncError("malloc", board);
	board->blockRow = row;
	board->blockCol = col;
	boardsAllocate(row, col, &(board->blockFixed), &(board->boardFixed));
	boardsAllocate(row, col, &(board->blockSolution), &(board->boardSolution));
	boardsAllocate(row, col, &(board->blockMain), &(board->boardMain));
	boardsAllocate(row, col, &(board->blockEror), &(board->boardError));
	return board;

}

void boardsCopy(boards* toBoard,boards* fromBoard){
	int col = fromBoard->blockCol;
	int row = toBoard->blockRow;
	memcpy(toBoard->blockEror,fromBoard->blockEror,row*col*row*col*sizeof(int));
	memcpy(toBoard->blockFixed,fromBoard->blockFixed,row*col*row*col*sizeof(int));
	memcpy(toBoard->blockSolution,fromBoard->blockSolution,row*col*row*col*sizeof(int));
	memcpy(toBoard->blockMain,fromBoard->blockMain,row*col*row*col*sizeof(int));
}
