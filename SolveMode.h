/*
 * SolveMode.h
 */

#ifndef SOLVEMODE_H_
#define SOLVEMODE_H_
#include "gurobi_c.h"
#include "possibleValuesList.h"

/**
 * SolveMode holds all the functions that we need when solving the sudoku board while using Gurobi
 */

/**
 * The function frees the memory located for env(the located environment), model(the gurobi model we created),
 * solution(the solution we found using ilp),numRep(the numeric representation chosen for variables usually binary),
 * solutionInt(solutions in integer representation), x (the lower bound)
 * error is the result of GRBgetdblattrarray function on the given model
 * This function returns NULL always
 */
int* freeGurobi(int error, GRBenv* env, GRBmodel* model, double* solution,double* x, char* numRep, int* solutionInt);

/**
 * The function frees the memory located for env(the located environment), model(the gurobi model we created),
 * solution(the solution we found using lp),numRep(the numeric representation chosen for variables continuous),
 * solutionDouble(solutions in double representation), x (the lower bound)
 * error is the result of GRBgetdblattrarray function on the given model
 * This function returns NULL always
 */
double* LPfreeGurobi(int error, GRBenv* env, GRBmodel* model, double* solution,double* x, char* numRep, double* solutionDouble);

/**
 * The function gets 3 parameters:
 * model=the current Gurobi model that we want to add the constraint to it, situation=the situation might be 'v': check that each cell gets a value,
 * 'r': check that each value appears once in every row,'c':  checks that each value appears once in every column
 * N= the dimension of the board
 * According to the specified situation the function decides to add a new constraint to model. It returns 0 if no errors occurred, 1 otherwise.
 */
int addConstModel(GRBmodel* model, char situation, int N);

/**
 * The function gets 4 parameters:
 * model=the current Gurobi model that we want to add the constraint to it, rowBL=the row of a block colBL=the column of a block,N= the dimension of the board
 * The function adds the following constraint to the given model: Make sure each value appears once in every sub-grid. Returns 0 if no errors occurred, 1 otherwise.
 */
int addConstModelSubG(GRBmodel* model, int rowBL, int colBL, int N);

/**
 * The function gets 4 parameters:
 * boardG is a boardGame structure describing boards of the game, x is set to 1 always and it is the lower bound,
 * N the dimension of the board, numRep is the numeric representation chosen for variables it will be a binary representation
 * The function builds an empty GRBmodel that will be used for the integer linear programming later on
 */
void buildGRBoard(boards* boardG, double* x, char* numRep, int N);

/**
 * The function gets 4 parameters:
 * boardG is a boardGame structure describing boards of the game, x is set to 1 always and it is the lower bound,
 * N the dimension of the board, numRep is the numeric representation chosen for variables it will be a continuous representation
 * The function builds an empty GRBmodel that will be used for the linear programming later on
 */
void LPbuildGRBoard(boards* boardG, double* x, char* numRep, int N);

/**The function gets 5 parameters:
 * arr is the array that contains the optimal solution from IP
 * row,col is the cell we want to get the first solution to it from arr with score greater or equal to thresh(the threshold)
 * dim is the dimension of the board
 * returns the first solution with score greater than thresh in specified cell
 */
int LPgetSolForCellThresh(double* arr, int row, int col, int dim, float thresh);

/**
 * The function gets 5 parameters:
 * arr is the array that contains the optimal solution from IP
 * row,col is the chosen cell
 * dim is the dimension of the board
 * prints the solutions for the cell that have a score greater than 0, and prints their matching scores
 * It returns 0 if no errors occurred, 1 otherwise.
 */
int LPgetSolForCell(double* arr, int row, int col, int dim);

/**
 * The function gets 2 parameters:
 * model- is the gurobi model we built previously
 * N is the dimension of the board
 * The function adds to the gurobi model the goal function: max (sum (wijk*xijk))
 * where wijk is a weight randomly chosen from 1 to N. and the sum contains all possible xijk
 * It returns 0 if no errors occurred, 1 otherwise.
 */
int addGoal(GRBmodel* model , int N);

/**
 * The function gets 2 parameters:
 * model- is the gurobi model we built previously
 * N is the dimension of the board
 * The function adds to the model all the constraints of the form: 0<=xijk<=1
 * for every possible i, j, k
 * It returns 0 if no errors occurred, 1 otherwise.
 */
int addConstBetween0And1(GRBmodel* model, int N); /*adding the weights for each variable xijk between 1 to N*/

/**
 * The function get 1 parameter:
 * boardG is a boardGame structure describing boards of the game
 * The function implements the linear programming method with the help of Gurobi and returns a pointer to an array with
 * the optimal solution if found else returns NULL
 */
double* LPboardSolver(boards* boardG);

/**
 * The function get 1 parameter:
 * boardG is a boardGame structure describing boards of the game
 * The function implements the integer linear programming method with the help of Gurobi and returns a pointer to an array with
 * the optimal solution if found else returns NULL
 */
int* boardSolver(boards* boardG);

/**
 * The function gets 4 parameters:
 * boardG is a boardGame structure describing boards of the game including the user's board
 * val is the value that will be checked, currRow is the row of the cell we want to insert val to, currCol is the col of that cell
 * The function checks if val can be inserted to the cell in currRow and currCol if yes return 1, if it is not possible return 0.
 */
int checkValue(boards* boardG, int val, int currRow, int currCol);

/**
 * The function gets boardG (boards of the game), row(the row of the specific cell), col(column of the same specific cell)
 * and it checks whether that cell has only a single option for a value if it does returns that value, 0 otherwise
 */
int oneSolutionForCell(boards* boardG, int row, int col);

/**
 * The function gets 1 parameter: boardG(a boardGame structure describing boards of the game including the user's board).
 * The function returns the number of empty cells in boardG
 */
int countEmpty(boards* boardG);

/**
 * The function gets 1 parameter: boardG(a boardGame structure describing boards of the game including the user's board).
 * The function returns a list of indexes of all the empty cells in the current board.
 */
int* listEmpty(boards* boardG);

/**
 * The function gets 1 parameter: boardG(a boardGame structure describing boards of the game including the user's board where we will insert the optimal solution).
 * The function implements the exhaustive backtracking algorithm, it exhausts all the values that could be placed in the empty cell of the board and returns
 * the number of solutions found for board
 */
int exhBackTSolver(boards* boardG);

/**
 * The function gets 3 parameters:
 * boardG(a boardGame structure describing boards of the game including the user's board).
 * row(row of the cell), col(column of the cell).
 * The function stores all valid values for the cell with the specified row and column in a new possibleValuesList structure and returns that struct
 */
possibleValuesList *newPossibleValuesList(boards* boardG, int row, int col);

/**
 * The function gets 1 parameter: boardG(a boardGame structure describing boards of the game including the user's board).
 * The function empties all the cells of the user's board
 */
void emptyUser(boards* boardG);

/**
 * The function gets 2 parameters:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * solutionILP(the solution of the board using integer linear programming)
 * The function puts the results of the integer linear programming into boardG
 */
void putOptimalToBoard(boards* boardG, int* solutionILP);

/**
 * The function gets 3 parameters:
 * boardG(a boardGame structure describing boards of the game including the user's board)
 * num1(is the number of cells we choose to fill with valid values),num2(is the number of cells that will not be removed)
 * The function generates a Board by first choosing num1 random cells(if there are num1 empty cells otherwise clears the whole boardG and continues)
 *  then finds valid values for them and fills them out, then it solves the board with integer
 * linear programming, then it chooses num2 cells that will not be cleared all the other once will be cleared.
 */
void generateMode(boards* boardG, int num1, int num2);

#endif /* SOLVEMODE_H_ */
