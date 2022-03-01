/*
 * boardGame.h
 */

#ifndef BOARDGAME_H_
#define BOARDGAME_H_

/**
 * "boards" holds the game's boards: it has the main board which the user see, the solution for
 * the board, the fixed board which tells us for each cell if it is fixed or not and all the cells
 * which are error.
 */
typedef struct boards
{
	int *blockMain;
	int *blockSolution;
	int *blockFixed;
	int *blockEror;
	int **boardMain;
	int **boardSolution;
	int **boardFixed;
	int **boardError;
	int blockRow;
	int blockCol;
}boards;

/**
 * The function gets boards and it frees all of them.
 */
void boardsFree(boards *board);


/**
 * The function gets the number of rows and column for a block, a pointer to a pointer of the block
 * and the pointer of a pointer of the pointers array. it allocates space for the boards.
 */
void boardsAllocate(int row, int col, int **block, int ***matBoards);

/**
 * The function gets the number of rows and column for a block, and it generates the board and returns it.
 */
boards *createBoard(int row, int col);


/**
 * This function gets the boards we want to copy and the destination boards,
 * and it copies the boards from one to the other.
 */
void boardsCopy(boards* toBoard,boards* fromBoard);

#endif /* BOARDGAME_H_ */
