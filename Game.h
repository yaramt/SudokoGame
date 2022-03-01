/*
 * Game.h
 */

#ifndef GAME_H_
#define GAME_H_
#include "boardStrc.h"
#include "Parser.h"
#include "boardGame.h"

/**
 * The Game module holds all the functions related to the game and for proper execution of user's request.
 */

/**
 * The function gets 1 parameter: num = number of hyphens we want to print
 * The function prints a line of num hyphens
 */
void printLineHyphens(int num);

/**
 * The function gets 3 parameters:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * markErrors( the current mark error as described in HW)
 * mode(the desired mode: 's'=save,'e'=edit , 'i'=init)
 * The function prints the user's board. It displays cells as errors if markErrors=1
 */
void printBoard(boards *boardG, int markErrors, char mode);

/**
 * The function gets 4 parameters:
 * solIlp which is the array of the solutions from the integer linear programming
 * row is the number of the row of the cell
 * col is the number of the column of the cell
 * dim is the dimension of the board
 * The function returns the solution for the specifies cell that was acquired from ilp
 */
int getSolForCellIlp(int* solIlp,int row,int col,int dim);

/**
 * The function gets 1 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * The function returns 1 if the user's main board is empty, 0 otherwise
 */
int isEmpty(boards* boardG);

/**
 * The function gets 2 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * x is the number of cells
 * The function returns 1 if the user's main board has x empty cells, 0 otherwise
 */
int isEmptyXCells(boards* boardG,int x);

/**
 * The function gets 1 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * It returns 1 if no cell is empty, 0 otherwise
 */
int boardFull(boards* boardG);

/**
 * The function receives 1 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * The function marks every error in the main (user) board
 */
void markUserErrors(boards* boardG);

/**
 * The function gets 2 parameters:
 * path is a path that leads to a board that we want to load
 * mode is the desired mode: 's'=save,'e'=edit , 'i'=init)
 * The function loads board from path and returns it in boardGame structure.
 */
boards* pathLoading(char* path,char mode);

/**
 * The function receives 3 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * row is the row of the specific cell, col is the column of the specific cell
 * The function checks if the specified cell causes any errors in the
 * board and updates the boardError in boardG accordingly
 */
void updateErrorBoardCell(boards* boardG, int row, int col);

/**
 * The function receives 2 parameters:
 * boardStr is a boardStrc that holds the current markError setting
 * val is the new value of the markError that can be only 0 or 1
 * The function sets markErrors to val only if it is 1 or 0 otherwise prints error message
 */
void markErrorVALUE(boardStrc* boardStr,int val);

/**
 * The function gets 1 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * It returns 1 if there are errors in the user's board else returns 0
 */
int isWrong(boards* boardG);

/**
 * The function receives 3 parameters:
 * command is the command the user chose after parsing
 * boardStr is a boardStrc that holds all boards
 * commandUser is the command the user inserted
 * The function frees the memory from everything related to the game
 */
void freeMemoAllGame(commands* command, boardStrc* boardStr, char* commandUser);

/**
 * The function receives 4 parameters:
 * command is the command the user chose after parsing, boardStr is a boardStrc that holds all boards
 * commandUser is the command the user inserted, markErrV is the current mark errors value
 * This function starts a new puzzle in solve mode and returns 1 if we started a new puzzle successfully, 0 otherwise
 */
int solve(commands* command, boardStrc* boardStr, char* commandUser,int markErrV);

/**
 * The function receives 4 parameters:
 * command is the set of commands with all values needed
 * boardStr is a boardStrc that holds all boards
 * commandUser is the command the user inserted, markErrV is the current mark errors value
 * This function starts a new puzzle in edit mode, returns 1 if the puzzle starts successfully else 0
 */
int edit(commands* command, boardStrc* boardStr, char* commandUser,int markErrV);

/**
 * The function receives 4 parameters:
 * command is the set , boardStr is a boardStrc that holds all boards
 * commandUser is the command the user inserted
 * mode is the 's'-save, 'i'-init,'e'-edit
 * The function translate command into <x,y>  and the value to set to the cell <x,y>
 * It returns 1 if the value that we set on the board created a full board, 0 otherwise
 */
int set(boardStrc* boardStr, commands* command, char mode,char* commandUser);

/**
 * The function gets 1 parameter:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * The function validates the current board using ILP ,ensuring it is solvable
 */
void validate(boards *boardG);

/**
 * The function receives 3 parameters:
 * boardStr is a boardStrc that holds all boards, x is the number of cells we need to fill
 * y is the number of cell that won't be cleared from the solver board
 * The function generates a puzzle by randomly filling x cells with random legal
 * values, running ILP to solve the board, and then clearing all but y random cells.
 */
void generate(boardStrc* boardStr, int x, int y);

/**
 * The function gets the command: redo/undo, the board before we did the move and the board
 * after the move, and it prints the moves that we have done by this command.
 */
void reUnPrint(char* cmd,boards* fromBoard, boards* toBoard);
/**
 * The function gets the mode of the game (Init/Save/Edit) and the struct of "boardStrc",
 * and it undos the previous moves that the user has done.
 */
void undo(char modeGame, boardStrc* game);

/**
 *  The function gets the mode of the game (Init/Save/Edit) and the struct of "boardStrc"
 * and it redos a move previously undone by the user.
 */
void redo(char modeGame, boardStrc* game);

/**
 * The function gets the mode of the game (Init/Save/Edit), the struct of "boards"
 * and the path that leads to a board that we want to load and it saves the current
 * board to the file.
 */
void saveBoards(char mode, boards* board, char* path);

/**
 * The function gets the row and column of a cell and the struct of "boards", and
 * it gives a hint(shows the cell's value).
 */
void hint(int x, int y, boards *board);

/**
 * The function gets the struct of "boards" (with all the boards which are relevent for the game),
 * and it prints how many solutions there are for the current board.
 */
void solutionsNum(boards* board);

/**
 * The function gets the struct of "boardStrc", the command of the user after parsing,
 * the command of the user without the parsing and the mode (Init/Edit/Save).
 * it fills cells that contain a single legal value. It returns 1 if the board is solved successfully,
 * otherwise it returns 0.
 */
int autofill(boardStrc* game, commands* parsedComm, char* userComm,char mode);

/**
 * The function gets the struct of "boardStrc" struct and the game mode (Edit/Init/Save),
 * and it undos all the moves.
 */
void reset(boardStrc* game,char mode);

/**
 * The function gets the commands after we parsed it, an object from the struct of "boardStrc" and
 * the command that was inserted by the user. it frees all the memory allocations and than exit.
 * it returns 1 - which mean we exit from the game.
 */
int gameExit(commands* commParsed, boardStrc* game, char* comm);

/**
 * The function gets the command after it has been parsed, the mode of the game (Save/Edit/Init),
 * an object from the struct "boardStrc", the command that has been inserted,
 * the mark errors value and the EOF (checks whether we arrive to end of file).
 * It finds which command is given, and calls the correct function.
 * It returns 0 if we continue the game, and 1 if we end the game.
 */
int runComm(commands* commParsed,char mode, boardStrc* game, char* origComm,char* eof, int valMarkErrors);

/**
 * The function gets the command after it has been parsed, and the mode : 'i'=init 'e'=edit 's'=solve
 * It prints an error message if the command is not available in the current mode
 * It returns 1 if there was an error found, 0 otherwise.
 */
int commandNotAvailableInCurrMode(commands* cmd,char mode);

/**
 * The function gets the command after it has been parsed
 * It prints that the command name is invalid if the command name is none of the following:
 * exit,solve,edit,mark_errors,print_board,set,validate,generate,undo,redo,save,hint,guess,
 *  guess_hint,num_solutions,autofill,reset
 *  Returns 1 is command name not valid, 0 otherwise
 */
int commandNameNotValid(commands* cmd);

/**
 * The function gets the command after it has been parsed, an object from the struct of "boardStrc"
 * and the command that was inserted by the user. It checks whether we got EOF after the command of
 * the user. if we got commands with more than 256 characters, it cleans
 * the stdin buffer. if we have EOF, the game will be finished. the function returns 0 if we have no EOF,
 * else it returns 1.
 */
int handleEof(commands* commParsed, boardStrc* game, char* userCommand);

/**
 * The function gets the path to the file, the mode of the game (Edit/Save/Init) and the
 * mark errors value. It initializes the game's object and it manages the game.
 */
void startGame(char* path, char mode, int markErrorsVal);

/**
 * The function gets an object from the struct "boardstrc", two in integers x and y, where x is the row and y is the column
 * of the cell we want to set it's value to set_value, the function always returns 0, and prints nothing, it sets he cell
 * only if it is not fixed and is not full, and only if the value is legal in the cell iootherwise does nothing
 */
int setGuess(boardStrc* boardStr, int x,int y,int set_value);
/**
 * The function gets 4 parameters: commParsed is the command struct that was built when guess was typed by the user,
 * x is the threshold, game is the boardstrc that holds the current board, origComm is the string holding user's guess
 * command and its parameters
 * The fucntion guesses a solution to the current board using LP , with threshold X.
 * If the board is erroneous, the program prints an error message and the command is not executed.
 * The command fills all cell values with a score of X or greater. It ignores invalid cell values regardless
 *  of their score.
 */
void guess(commands* commParsed,float x, boardStrc* game,char* origComm);
/**
 * The function gets 3 parameters:
 * x-row number
 * y-column number
 * board-boards struct holding the current board
 * The function shows a guess to the user for a single cell x,y.
 * If the board is erroneous, cell <X,Y> is fixed, or cell <X,Y> already contains a value, it is an error (in that order).
 * Run LP for the current board. If the board is unsolvable, it is an error. Otherwise,
 * print to the user all legal values of cell <X,Y> and their scores.
 * You should only print values with a score greater than 0.
 */
void guessHint(int x, int y, boards *board);

#endif /* GAME_H_ */
