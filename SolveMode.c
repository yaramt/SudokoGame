/*
 * SolveMode.c
 */
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "Checkers.h"
#include "Game.h"
#include "possibleValuesList.h"
#include "gurobi_c.h"
#include "listStrc.h"
#include "boardGame.h"
#define MAXTRIES 1000 /*max tries as for generating the board*/

int* freeGurobi(int error, GRBenv* env, GRBmodel* model, double* solution,double* x, char* numRep, int* solutionInt){
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		free(solutionInt);
	}
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(solution);
	free(numRep);
	free(x);
	return NULL;
}

double* LPfreeGurobi(int error, GRBenv* env, GRBmodel* model, double* solution,double* x, char* numRep, double* solutionDouble){
	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		free(solutionDouble);
	}
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(solution);
	free(numRep);
	free(x);
	return NULL;
}

int addConstModel(GRBmodel* model, char situation, int N) {

	int error, i, j, k;
	double* val;
	int* ind;

	val = (double*)calloc(N, sizeof(double));
	ind = (int*)calloc(N, sizeof(double));
	memFuncError("calloc", val);
	memFuncError("calloc", ind);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				 if (situation == 'r')
					ind[k] = j * N * N + k * N + i; /*making sure every value appears once in each row*/
				 else if (situation == 'c')
					ind[k] = k * N * N + j * N + i; /*making sure every value appears once in each column*/
				 else if (situation == 'v')
				 	ind[k] = i * N * N + j * N + k; /*making sure every cell gets a value */
				val[k] = 1.0;
			}
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				free(ind);
				free(val);
				return error;
			}
		}
	}
	free(ind);
	free(val);
	return error;
}

int addConstModelSubG(GRBmodel* model, int rowBL, int colBL, int N) {
	int i,j,x,grid_1,grid_2,c,error;
	double* val;
	int* ind;
	val = (double*)calloc(N, sizeof(double));
	ind = (int*)calloc(N, sizeof(double));
	memFuncError("calloc", val);
	memFuncError("calloc", ind);
	for (x = 0; x < N; x++)
	{
		for (grid_1 = 0; grid_1 < colBL; grid_1++)
		{
			for (grid_2 = 0; grid_2 < rowBL; grid_2++)
			{
				c = 0;
				for (i = grid_1 * rowBL; i < (rowBL*grid_1 + rowBL) ; i++)
				{
					for (j = grid_2 * colBL; j < (colBL*grid_2 + colBL); j++)
					{
						val[c] = 1.0;
						ind[c] = x + i * N * N + j * N ;
						c++;
					}
				}
				error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0,NULL );
				if (error) {
					free(ind);
					free(val);
					return error;
				}
			}
		}
	}
	free(ind);
	free(val);
	return error;
}

void buildGRBoard(boards* boardG, double* x, char* numRep, int N) {
	int i,j,k;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 1; k <= N; k++)
			{
				numRep[j * N + i * N * N + k - 1] = GRB_BINARY;
				if (boardG->boardMain[i][j] == k)
					x[j * N + i * N * N + k - 1] = 1;

			}
		}}}


int* boardSolver(boards* boardG) {
	int optimstatus,i,error=0;
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int N = row * col;
	GRBmodel *model = NULL;
	GRBenv *env = NULL;
	double* x = (double*)calloc(N * N * N, sizeof(double));
	char* numRep = (char*)calloc(N * N * N, sizeof(char));
	double* solution = (double*)calloc(N * N * N, sizeof(double));
	int* solutionInt = (int*)calloc(N * N * N, sizeof(int));
	memFuncError("calloc", numRep);
	memFuncError("calloc", solution);
	memFuncError("calloc", solutionInt);
	memFuncError("calloc", x);
	buildGRBoard(boardG, x, numRep, N);
	if(GRBloadenv(&env, "sudoku.log"))
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0))
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, x, NULL, numRep, NULL ))
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (addConstModel(model, 'v', N))	/*give every cell a value*/
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (addConstModel(model, 'r', N))	/*makes sure every valid value appears once in each row*/
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (addConstModel(model, 'c', N))	/*makes sure every valid value appears once in each column*/
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (addConstModelSubG(model, row, col, N)) /*makes sure every valid value appears once in each sub-grid*/
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (GRBoptimize(model))
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (GRBwrite(model, "sudoku.lp"))	/*write to 'sudoku.lp'*/
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus))
		return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	if (optimstatus == GRB_OPTIMAL)/*If the problem was solved puts the optimal values to solutionInt*/
	{
		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, solution);
		if (error)
			return freeGurobi(error,env,model,solution,x,numRep,solutionInt);
		else{
			for (i = 0; i < N * N * N; i++) {
				solutionInt[i] = (int) solution[i];
			}}
	}
	else {
		free(solutionInt);
		solutionInt = NULL;}

	freeGurobi(error,env,model,solution,x,numRep,solutionInt);
	return solutionInt;
}

int checkValue(boards* boardG, int val, int currRow, int currCol) {
	int i,j;
	int colB = boardG->blockCol;
	int rowB = boardG->blockRow;
	int dim=colB * rowB;
	int blockCol = rowB*((currCol - 1) / rowB);
	int blockRow = colB*((currRow - 1) / colB);
	/*can we put val in currCol?*/
	for (i = 0; i < dim; i++) {
		if ((i+1) != currRow && ( boardG->boardMain[currCol - 1][i] ) == val)
			return 0;
	}
	/*can we put val in currRow?*/
	for (i = 0; i < dim; i++) {
			if ((i+1) != currCol  && ( boardG->boardMain[i][currRow - 1] ) == val)
				return 0;
		}
	/*can we put val in the block of the given cell?*/
	for (i = blockCol; i < blockCol + rowB; i++)
	{
		for (j = blockRow; j < blockRow + colB; j++)
		{
			if (((i+1) != currCol  || (j+1) != currRow) && (boardG->boardMain[i][j] == val))
				return 0;}}
	return 1;}

int oneSolutionForCell(boards* boardG, int row, int col) {
	int j,num = 0,c=0;
	int colB = boardG->blockCol;
	int rowB = boardG->blockRow;
	int dim=rowB*colB;
	for (j = 1; j <= dim; j++) {
		if (checkValue(boardG, j, row, col)) {
			c++;
			num = j;
		}
		if (c > 1)
			return 0;
	}return num;}

int countEmpty(boards* boardG) {
	int c = 0,i;
	int row = boardG->blockRow;
	int col = boardG->blockCol;
	int dimSq=row*row*col*col;
	for (i = 0; i < dimSq; i++)
	{
		if (boardG->blockMain[i] == 0) /*Is the cell empty?*/
			c++;
	}
	return c;
}

int* listEmpty(boards* boardG) {
	int j,numEmptCells,i=0,row,col,dimSq;
	int* list;
	row = boardG->blockRow;
	col = boardG->blockCol;
	dimSq=row*row*col*col;
	numEmptCells = countEmpty(boardG);
	list = (int*)calloc(numEmptCells, sizeof(int));
	memFuncError("calloc", list); /*memory error when trying to allocate space for list*/
	for (j = 0; j < dimSq; j++)
	{
		if (boardG->blockMain[j] == 0) {/*is the cell empty?*/
			list[i] = j;
			i++;}
	}
	return list;
}

int exhBackTSolver(boards* boardG) {
	int countSolutions=0;
	int indList=0;/*The current index of a cell in listEmptyIndex*/
	int index; /*current index on the board*/
	int flag = 0;	/*if flag=0 we re-track ,else if flag=1 we proceed with the next cell*/
	int tempVal = 1; /*temporary value used when going through the possible values of a specific cell*/
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int dim=row*col;
	listStrc* nodeRec = NULL;
	int cellsEmptyFill = countEmpty(boardG);	/*num of empty cells*/
	int* listEmptyIndex= listEmpty(boardG); 	/*list with indexes of empty cells in board*/
	nodeRec  = addfirst(nodeRec , -1); 	/*adding a dummy value to know where is the head of the list*/
	index = listEmptyIndex[indList];
	while (1) {
		if (indList == cellsEmptyFill) { 	/*no empty cells left*/
			countSolutions++;
			boardG->blockMain[index] = 0; /*re-track the cell with current index in board*/
			tempVal = nodeRec->x;
			indList-=1; 		/*Re-track*/
			if (indList >= 0) /*do we have to update current index?*/
				index = listEmptyIndex[indList];
			flag = 0;
			nodeRec  = removefirst(nodeRec);/*remove first node in the list*/
		}
		/*Loop through all the possible values of a cell and find valid once*/
		for (; tempVal <= dim; tempVal++) {
			if (checkValue(boardG, tempVal, (index % dim) + 1,(index / dim) + 1)) /*is this value is valid?*/
			{
				indList++;
				boardG->blockMain[index] = tempVal;
				nodeRec  = addfirst(nodeRec , tempVal + 1);	/*value added to our list*/
				tempVal = 1; 	/*moving to the next cell so tempVal must be 1 to go through all possible values*/
				flag = 1; 	/*go to next cell*/
				if (indList < cellsEmptyFill)
					index = listEmptyIndex[indList];
				break;
			}
			flag = 0; 	/* back-track*/
		}
		if (flag == 0) /* back-track*/
		{
			boardG->blockMain[index] = 0;
			indList--; 	/* re-track to the previous cell */
			tempVal = nodeRec->x; /* get from the list the value we stopped on previously*/
			if (indList >= 0)
				index = listEmptyIndex[indList];/* update the index*/
			nodeRec = removefirst(nodeRec);
		}
	if(nodeRec == NULL)/*Has the first node-dummy been removed? */
		break;
	}
	free(listEmptyIndex);
	return countSolutions; /*number of solutions found*/
}

possibleValuesList *newPossibleValuesList(boards* boardG, int row, int col) {
	int i;
	int rowB = boardG->blockRow;
	int colB = boardG->blockCol;
	int dim=rowB*colB;
	possibleValuesList* firstN = NULL;
	for (i = 1; i <= dim; i++)
	{
		if (checkValue(boardG, i, row, col) == 1) /*Is value valid*/
			addList(&firstN, i);
	}
	return firstN;
}

void emptyUser(boards* boardG) {
	int i;
	int row = boardG->blockRow;
	int col = boardG->blockCol;
	int dimSq = row*col*row*col;
	for (i = 0; i < dimSq; i++) /*loop throughout the whole user board to empty all it's cells*/
		boardG->blockMain[i] = 0;
}

void putOptimalToBoard(boards* boardG, int* solutionILP) {
	int i,j,k;
	int row = boardG->blockRow;
	int col = boardG->blockCol;
	int dim = row*col;
	int dimSq=dim*dim;
	for (i = 0; i < dim; i++)
	{
		for (j = 0; j < dim; j++)
		{
			for (k = 0; k < dim; k++)
			{
				if (solutionILP[i*dimSq + j*dim + k] == 1) {
					boardG->boardMain[i][j]=1+k;
					break;}
			}
		}
	}
}


void generateMode(boards* boardG, int num1, int num2) {
	int flag; /*if a cell has a valid value flag=1, 0 otherwise*/
	int flag2;/*if an empty cell has valid values then flag2=1, 0 otherwise*/
	int rand1, rand2, randIndex, iter, iter2;
	int* solutionILP;
	possibleValuesList* list;
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int dim = row * col;
	int dimSq=dim*dim;
	boards* boardOrig= createBoard(row, col);
	boardsCopy(boardOrig,boardG);
	for (iter = 0; iter < MAXTRIES; iter++)/*MAXTRIES iterations to try to create the board in the wanted way*/
	{
		flag = 0;
		for (iter2 = 0; (flag == 0) && (iter2 < num1 ); iter2++) /*Trying finding x random empty cells and filling them with valid values*/
		{
			flag2 = 0; /* at the start no empty cell found yet*/
			while(1) {
				rand1 = rand() % dim;
				rand2 = rand() % dim;
				if (boardG->boardMain[rand2][rand1] == 0)/*if cell empty*/
				{
					list = newPossibleValuesList(boardG, rand1 + 1, rand2 + 1);
					if (list == NULL ) /*if no possible valid values found for cell*/
					{
						flag = 1;
						break;
					}
					else /*if possible valid values found for cell*/
					{
						flag2 = 1;
						randIndex = rand()% sizeList(list);
						boardG->boardMain[rand2][rand1] = removeList(&list,randIndex);
						freeMemo(list);
					}
				}
				if(flag2!=0)/*when an empty cell is found*/
					break;
			}
		}
		if (iter2 != num1) /*Are there no num1 cells to fill?*/
			boardsCopy(boardG,boardOrig);
		 else /*else run ilp*/
		 {
			solutionILP = boardSolver(boardG);
			if (solutionILP == NULL )
				boardsCopy(boardG,boardOrig);
			 else /*ilp found a solution*/
				break;
		}
	}
	if (iter == MAXTRIES) {
		printf("Error: puzzle generator failed\n");
		free(boardOrig);
		free(solutionILP);
		return;
	}
	emptyUser(boardG);
	putOptimalToBoard(boardG, solutionILP);
	free(solutionILP);
	free(boardOrig);
	for (iter2 = 0; iter2 < dimSq - num2; iter2++)/*leave only num2 cells on board*/ {
		flag2 = 0;
		while(1) {
			rand1 = rand() % dim;
			rand2 = rand() % dim;
			if (boardG->boardMain[rand2][rand1] != 0)/*is cell not empty?*/ {
				flag2 = 1;
				boardG->boardMain[rand2][rand1] = 0;
			}
			if(flag2==1)
				break;
		}
	}
	printBoard(boardG, 1, 'e');
}

void LPbuildGRBoard(boards* boardG, double* x, char* numRep, int N) {
	int i,j,k;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 1; k <= N; k++)
			{
				numRep[j * N + i * N * N + k - 1] = GRB_CONTINUOUS;
				if (boardG->boardMain[i][j] == k)
					x[j * N + i * N * N + k - 1] = 1;

			}
		}}}
/*returns the first solution with score greater than thresh in specified cell*/
int LPgetSolForCellThresh(double* arr, int row, int col, int dim, float thresh) {
	int i, consTant;
	int dimSq=dim*dim;
	consTant=(col - 1) * dimSq + (row - 1) * dim;
	for (i = 0; i < dim; i++)
	{
		if (arr[consTant + i] >=thresh)
			return (i + 1);
	}
	return 0;
}
/*prints the solutions for the cell*/
int LPgetSolForCell(double* arr, int row, int col, int dim) {
	int i, consTant;
	int dimSq=dim*dim;
	int c=0;
	consTant=(col - 1) * dimSq + (row - 1) * dim;
	for (i = 0; i < dim; i++)
		{
			if (arr[consTant + i] > 0){
				if(c==0) printf("Guessed values for chosen cell:\n");
				printf("%d with the score: %f\n",i+1,arr[consTant + i]);
				c++;}
		}
	return 0;
}

int addGoal(GRBmodel* model , int N) {/*adding the weights for each variable xijk between 1 to N*/
	int error,error2 ,i, j, k;
	char *numRep;
	double* obj;

	numRep = (char*)calloc(N * N * N, sizeof(char));
	obj=(double*)calloc(N * N * N, sizeof(double));

	memFuncError("calloc", numRep);
	memFuncError("calloc", obj);
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			for (k = 0; k < N; k++)
			{
				obj[i * N * N + j * N + k]= rand() % N +1; /*the weight given for xijk*/
				numRep[i * N * N + j * N + k]=GRB_CONTINUOUS;
			}
		}
	}
	error = GRBaddvars(model, N*N*N, 0, NULL, NULL, NULL, obj, NULL, NULL, numRep, NULL);
	error2 = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
	free(obj);
	free(numRep);
	if(error)
		return error;
	return error2;
}

int addConstBetween0And1(GRBmodel* model, int N) {
	int error1,error2, i, j, k;
	double* val;
	int* ind;

	val = (double*)calloc(1, sizeof(double));
	ind = (int*)calloc(1, sizeof(double));
	memFuncError("calloc", val);
	memFuncError("calloc", ind);
	for (i = 0; i < N; i++)
		{
			for (j = 0; j < N; j++)
			{
				for (k = 0; k < N; k++)
				{
					ind[0]=  i * N * N + j * N + k;/*making sure every variable 0<=xijk<=1 */
					val[0] = 1.0;
					error1 = GRBaddconstr(model, 1, ind, val, GRB_LESS_EQUAL, 1.0, NULL);
					error2 = GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0.0, NULL);
					if (error1||error2) {
						free(ind);
						free(val);
						if(error1)
							return error1;
						return error2;
					}
				}

			}
		}
		free(ind);
		free(val);
		return error1;
}

double* LPboardSolver(boards* boardG) {
	int i,error=0;
	int optimstatus;
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int N = row * col;
	GRBmodel *model = NULL;
	GRBenv *env = NULL;
	double* x = (double*)calloc(N * N * N, sizeof(double));
	char* numRep = (char*)calloc(N * N * N, sizeof(char));
	double* solution = (double*)calloc(N * N * N, sizeof(double));
	double* solutionDouble = (double*)calloc(N * N * N, sizeof(double));
	memFuncError("calloc", numRep);
	memFuncError("calloc", solution);
	memFuncError("calloc", solutionDouble);
	memFuncError("calloc", x);
	LPbuildGRBoard(boardG, x, numRep, N);
	if(GRBloadenv(&env, "sudoku.log"))
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0))
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (GRBnewmodel(env, &model, "sudoku", N * N * N, NULL, x, NULL, numRep, NULL ))
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addConstModel(model, 'v', N))	/*give every cell a value*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addConstModel(model, 'r', N))	/*makes sure every valid value appears once in each row*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addConstModel(model, 'c', N))	/*makes sure every valid value appears once in each column*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addConstModelSubG(model, row, col, N)) /*makes sure every valid value appears once in each sub-grid*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addConstBetween0And1(model, N)) /*makes sure every variable 0<=xijk<=1*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (addGoal(model, N)) /*add the goal function*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (GRBoptimize(model))
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (GRBwrite(model, "sudoku.lp"))	/*write to 'sudoku.lp'*/
			return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus))
				return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
	if (optimstatus == GRB_OPTIMAL)/*If the problem was solved puts the optimal values to solutionDouble*/
		{
			error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N * N * N, solution);
			if (error)
				return LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
			else{
				for (i = 0; i < N * N * N; i++) {
					solutionDouble[i] = solution[i];
				}}
		}
		else {
			free(solutionDouble);
			solutionDouble = NULL;}

	LPfreeGurobi(error,env,model,solution,x,numRep,solutionDouble);
		return solutionDouble;
}
