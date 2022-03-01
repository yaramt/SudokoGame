
/*
 * Game.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Game.h"
#include "SolveMode.h"
#include "Parser.h"
#include "Checkers.h"
#include "doubleList.h"
#include "boardGame.h"
#include "boardStrc.h"
#include "Stack.h"

#define CMDSIZE 257 	/*max length of command is 256 characters and '\0'*/

void printLineHyphens(int num) {
	int i;
	for (i = 0; i < num; ++i)
		printf("-");
	printf("\n");
}

void printBoard(boards *boardG, int markErrors, char mode) {
	int col1,col2,row1;
	int rowB = boardG->blockRow;
	int colB = boardG->blockCol;
	int dim=colB*rowB;
	if(mode == 'e')/*edit mode*/
		markErrors = 1;
	printLineHyphens(4 * dim + rowB + 1);
	for (row1 = 0; row1 < dim; row1++) {
		printf("|");
		for (col1 = 0; col1 < rowB; col1++) {
			for (col2 = 0; col2 < colB; col2++)
			{
				printf(" ");
				if (1==boardG->boardFixed[row1][col2+col1 * colB])
					printf("%2d.", boardG->boardMain[row1][col2+col1 * colB]);
				else if (markErrors == 1 && boardG->boardError[row1][col2+col1 * colB ] == 1)
					printf("%2d*", boardG->boardMain[row1][col2+col1 * colB]);
			    else {
					if (boardG->boardMain[row1][col1 * colB + col2] == 0)/*empty cell*/
						printf("   ");
					else printf("%2d ",boardG->boardMain[row1][col1 * colB + col2]);
				     }
			}
			printf("|");
		}
		printf("\n");
		if (((row1+1) % rowB) == 0)
			printLineHyphens(4*dim +rowB+1);
	}
}

int getSolForCellIlp(int* arr, int row, int col, int dim) {
	int i, consTant;
	int dimSq=dim*dim;
	consTant=(col - 1) * dimSq + (row - 1) * dim;
	for (i = 0; i < dim; i++)
	{
		if (arr[consTant + i] == 1)
			return (i + 1);
	}
	return 0;
}

int isEmpty(boards* boardG) {
	int i;
	int row = boardG->blockRow;
	int col = boardG->blockCol;
	int dim = row*col;
	int dimSq=dim*dim;
	for (i = 0; i < dimSq; i++) {
		if (boardG->blockMain[i] != 0) /*is there a cell that is not empty?*/
			return 0;
	}
	return 1;
}

int isEmptyXCells(boards* boardG,int x){
		int i,c=0;
		int row = boardG->blockRow;
		int col = boardG->blockCol;
		int dim = row*col;
		int dimSq=dim*dim;
		for (i = 0; i < dimSq; i++) {
			if (boardG->blockMain[i] == 0) /*a cell that is empty?*/
				c++;
			if(c==x)
				return 1;
		}
		return 0;
}

int boardFull(boards* boardG) {
	int i,j;
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int dim=row*col;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (boardG->boardMain[i][j] == 0) {
				return 0;
			}
		}
	}
	return 1;
}

void markUserErrors(boards* boardG) {
	int i,j;
	int col = boardG->blockCol;
	int row = boardG->blockRow;
	int dim=row*col;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (boardG->boardMain[i][j] == 0|| checkValue(boardG, boardG->boardMain[i][j], j + 1, i + 1))
				boardG->boardError[i][j] = 0;
			 else boardG->boardError[i][j] = 1;
		}
	}
}

boards* pathLoading(char* path, char mode) {
	int row,col,i=0,v;
	char dot;
	boards* boardG;
	FILE* fp;
	if (mode == 'e' && path[0] == '\0')
		boardG = createBoard(3, 3);
	 else {
		fp = fopen(path, "r");
		memFuncError("fopen",fp);
		fscanf(fp, "%d %d", &row, &col);
		boardG = createBoard(row, col);
		while (fscanf(fp, "%d", &v) != EOF) {
			boardG->blockMain[i] = v;
			dot = fgetc(fp);
			if (dot == '.' && mode == 's') {
				boardG->blockFixed[i] = 1;
			}
			i++;}
		if(ferror(fp)){
			memFuncError("fscanf",NULL);
		}
		markUserErrors(boardG);
		fclose(fp);
	}
	return boardG;
}

void updateErrorBoardCell(boards* boardG, int row, int col) {
	int i,j;
	int rowB = boardG->blockRow;
	int colB = boardG->blockCol;
	int dim=rowB*colB;
	int b1 = ((col - 1) / rowB) * rowB;
	int b2 = ((row - 1) / colB) * colB;
	for (i = 0; i < dim; i++) {
		if (boardG->boardMain[i][row - 1] == 0 || checkValue(boardG, boardG->boardMain[i][row - 1], row, i + 1))
			boardG->boardError[i][row - 1] = 0;
		 else boardG->boardError[i][row - 1] = 1;
	}
	for (i = 0; i < dim; i++) {
		if (boardG->boardMain[col - 1][i] == 0 || checkValue(boardG, boardG->boardMain[col - 1][i], i + 1, col))
			boardG->boardError[col - 1][i] = 0;
		 else boardG->boardError[col - 1][i] = 1;
	}
	for (i = b1; i < b1 + rowB; i++) {
		for (j = b2; j < b2 + colB; j++) {
			if (boardG->boardMain[i][j] == 0 || checkValue(boardG, boardG->boardMain[i][j], j + 1, i + 1))
				boardG->boardError[i][j] = 0;
			 else boardG->boardError[i][j] = 1;
		}
	}
}

void markErrorVALUE(boardStrc* boardStr, int val) {
	if ((val != 0) && (val != 1))
		printf("Error: the value should be 0 or 1\n");
	else boardStr->markErrors = val;
}

int isWrong(boards* boardG) {
	int i,j;
	int row = boardG->blockRow;
	int col = boardG->blockCol;
	int dim=row*col;
	for (i = 0; i < dim; i++) {
		for (j = 0; j < dim; j++) {
			if (boardG->boardError[i][j] == 1)
				return 1;
		}
	}
	return 0;
}

void freeMemoAllGame(commands* command, boardStrc* boardStr, char* commandUser) {
	if(command != NULL){
		free(command);
	}
	if(commandUser != NULL){
		free(commandUser);
	}
	if(boardStr != NULL){
		freeGame(boardStr);
	}
}

int solve(commands* command, boardStrc* boardStr, char* commandUser,int markErrV){
	if(checkPath(command->path))
	{
		if(boardStr != NULL)
			markErrV = boardStr->markErrors;
		startGame(command->path,'s',markErrV);
		freeMemoAllGame(command,boardStr,commandUser);
		return 1;
	}
	else {
		printf("Error: File doesn't exist or cannot be opened\n");
		return 0;
	}
}

int edit(commands* command, boardStrc* boardStr, char* commandUser,int markErrV){
	if(command->path[0] == '\0' || checkPath(command->path))
	{
		if(boardStr != NULL)
			markErrV = boardStr->markErrors;
		startGame(command->path,'e',markErrV);
		freeMemoAllGame(command,boardStr,commandUser);
		return 1;
	}
	else
	{
		printf("Error: File cannot be opened\n");
		return 0;
	}
}

int set(boardStrc* game, commands* cmd, char mode,char* origcom) {
	int markErrorsVal;
	int x = cmd->vars[0];
	int y = cmd->vars[1];
	int set_value = cmd->vars[2];
	if (checkMeasurements(x, y, set_value, 0,game->board->blockCol * game->board->blockRow)) { /*in parser already checked if we get numbers as input and put -1 in not, which will fail here*/
		if (mode == 's' && game->board->boardFixed[y - 1][x - 1] == 1) {
			printf("Error: cell is fixed\n");
			return 0;
		} else {
			if (game->board->boardMain[y - 1][x - 1] != set_value) {
				game->board->boardMain[y - 1][x - 1] = set_value;
				updateErrorBoardCell(game->board, x, y);
				game->moveList = addgameList(game->moveList, game->board);
			}
		}
	} else {
		printf("Error: value not in range 1-%d\n",game->board->blockCol * game->board->blockRow);
		return 0;
	}
	printBoard(game->board, game->markErrors, mode);

	if (mode == 's' && boardFull(game->board)) { /*after set command check if board if full and finished*/
		if (isWrong(game->board)) {
			printf("Puzzle solution erroneous\n");
		} else {
			printf("Puzzle solved successfully\n");
			markErrorsVal = game->markErrors;
			freeMemoAllGame(cmd,game,origcom);
			startGame(NULL,'i',markErrorsVal);
			return 1;
		}
	}
	return 0;
}

void validate(boards *boardG) {
	int* solutionILP;
	if (isWrong(boardG))
	{
		printf("Error: board contains erroneous values\n");
		return;
	}
	solutionILP = boardSolver(boardG);
	if (solutionILP != NULL )
	{
		printf("Validation passed: board is solvable\n");
		free(solutionILP);
	}
	else printf("Validation failed: board is unsolvable\n");
}

void generate(boardStrc* boardStr, int x, int y) {
	int col = boardStr->board->blockCol;
	int row = boardStr->board->blockRow;
	int dim=row*col;
	int dimSq=dim*dim;
	if (checkMeasurements(x, y, 0, -1, dimSq) == 0 ) {
		printf("Error: value not in range 0-%d\n", dimSq);
	} else if (isEmptyXCells(boardStr->board,x) == 0) {/*There are no x empty cells*/
		printf("Error: board does not have %d empty cells\n",x);
	} else {
		generateMode(boardStr->board, x, y);
		if(y != 0)
			boardStr->moveList = addgameList(boardStr->moveList, boardStr->board);
	}
}

void reUnPrint(char* cmd,boards* fromBoard, boards* toBoard) {
	int i,j,fromV,toV;
	int row = fromBoard->blockCol;
	int col = fromBoard->blockRow;
	for (i = 0; i < row * col; i++) {
		for (j = 0; j < row * col; j++) {
			fromV = fromBoard->boardMain[i][j];
			toV = toBoard->boardMain[i][j];
			if (fromV != toV) {
				if (fromV != 0 && toV != 0) {
					printf("%s %d,%d: from %d to %d\n", cmd, j+1, i+1, fromV, toV);
				} else if (fromV == 0 && toV != 0) {
					printf("%s %d,%d: from _ to %d\n", cmd, j+1, i+1, toV);
				} else if (fromV != 0 && toV == 0) {
					printf("%s %d,%d: from %d to _\n", cmd, j+1, i+1, fromV);
				} else if (fromV == 0 && toV == 0) {
					printf("%s %d,%d: from _ to _\n", cmd, j + 1, i + 1);
				}
			}
		}
	}
}

void undo(char modeGame,boardStrc* game) {
	if (game->moveList->prev == NULL ) {
		printf("Error: no moves to undo\n");
		return;
	}
	printBoard(game->moveList->prev->board, game->markErrors,modeGame);
	reUnPrint("Undo",game->board, game->moveList->prev->board);
	boardsCopy(game->board, game->moveList->prev->board);
	game->moveList = game->moveList->prev;
}

void redo(char modeGame,boardStrc* game) {
	if (game->moveList->next == NULL ) {
		printf("Error: no moves to redo\n");
		return;
	}
	printBoard(game->moveList->next->board, game->markErrors,modeGame);
	reUnPrint("Redo",game->board, game->moveList->next->board);
	boardsCopy(game->board, game->moveList->next->board);
	game->moveList = game->moveList->next;
}

void saveBoards(char mode,boards* board, char* path) {
	int i,j;
	int* arr;
	int col = board->blockCol;
	int row = board->blockRow;
	FILE *ofp = NULL;
	if (mode == 'e') {/* so we don't execute the command */
		if (isWrong(board)) {
			printf("Error: board contains erroneous values\n");
			return;
		}
		arr = boardSolver(board);
		if (arr == NULL ) {
			printf("Error: board validation failed\n"); /*command is not executed in this case*/
			return;
		}
	}
	ofp = fopen(path, "w");
	if (ofp == NULL ) {
		printf("Error: File cannot be created or modified\n");
	} else {
		if(WritingFileFailed(fprintf(ofp, "%d %d\n", row, col),ofp)){
			return;
		}
		for (i = 0; i < row * col; i++) {
			for (j = 0; j < row * col; j++) {
				if(WritingFileFailed(fprintf(ofp, "%d", board->boardMain[i][j]),ofp)){
					return;
				}
				if ((mode == 'e' && board->boardMain[i][j] != 0)
						|| board->boardFixed[i][j] == 1) {
					if(WritingFileFailed(fprintf(ofp,"%c",'.'),ofp)){
						return;
					}
				}
				if (j != row * col - 1) {
					if(WritingFileFailed(fprintf(ofp,"%c",' '),ofp)){
						return;
					}
				}
			}
			if (i != row * col - 1) {
				if(WritingFileFailed(fprintf(ofp,"%c",'\n'),ofp)){
					return;
				}
			}
		}
		printf("Saved to: %s\n", path);
		fclose(ofp);
	}
}


int setGuess(boardStrc* boardStr, int x,int y,int set_value) {
		if (boardStr->board->boardFixed[y - 1][x - 1] == 1) /*if cell is fixed*/ {
			return 0;
		} else
		{
			if (boardStr->board->boardMain[y - 1][x - 1] != set_value)
			{
				boardStr->board->boardMain[y - 1][x - 1] = set_value;
				updateErrorBoardCell(boardStr->board, x, y);
				if (isWrong(boardStr->board)) {
					boardStr->board->boardMain[y - 1][x - 1] = 0;/*undo what you did*/
					updateErrorBoardCell(boardStr->board, x, y);
				}
				else boardStr->moveList = addgameList(boardStr->moveList, boardStr->board);
			}
		}
	return 0;
}

/*x is the threshold*/
void guess(commands* commParsed,float x, boardStrc* game,char* origComm){
	double* arr;
	int i,j;
	int row = game->board->blockRow;
	int col = game->board->blockCol;
	int mul = col*row;

	if(x>1 || x<0)/*threshold not in [0,1]*/
		printf("ERROR: threshold should be in the range [0,1]");
	else if (isWrong(game->board))
		printf("Error: board contains erroneous values\n");

	else{
		arr = LPboardSolver(game->board);
		if (arr!= NULL )
		{
			for(i=1;i<=mul;i++){
				for(j=1;j<=mul;j++){
					setGuess(game, i,j,LPgetSolForCellThresh(arr,i,j,mul,x));
				}}
			printBoard(game->board, game->markErrors, 's');
			if (boardFull(game->board)) {
					if (isWrong(game->board))
						printf("Puzzle solution erroneous\n");
					 else {
						printf("Puzzle solved successfully\n");
						freeMemoAllGame(commParsed,game,origComm);
						startGame(NULL,'i',game->markErrors);
					}}
		}
		else
			printf("Validation failed: board is unsolvable\n");
}
}

void guessHint(int x, int y, boards *board){

	double* arr;
	int row = board->blockRow;
	int col = board->blockCol;
	int mul = col*row;
	if (isWrong(board))
		printf("Error: board contains erroneous values\n");
	else if (board->boardFixed[y-1][x-1] == 1)
			printf("Error: cell is fixed\n");
	else if (board->boardMain[y-1][x-1] != 0)
			printf("Error: cell already contains a value\n");
	else if (checkMeasurements(x, y, 0, 0, mul) == 0)
		printf("Error: value not in range 1-%d\n", mul);
	 else {
			arr = LPboardSolver(board);
			if (arr!= NULL ) {
				LPgetSolForCell(arr, x, y, mul);
				free(arr);}
			 else
				printf("Validation failed: board is unsolvable\n");
		}
}

void hint(int x, int y, boards *board) {
	int row = board->blockRow;
	int col = board->blockCol;
	int mul = col*row;
	int* arr;
	if (checkMeasurements(x, y, 0, 0, mul) == 0) {
		printf("Error: value not in range 1-%d\n", mul);
	} else if (isWrong(board)) {
		printf("Error: board contains erroneous values\n");
	} else if (board->boardFixed[y-1][x-1] == 1) {
		printf("Error: cell is fixed\n");
	} else if (board->boardMain[y-1][x-1] != 0) {
		printf("Error: cell already contains a value\n");
	} else {
		arr = boardSolver(board);
		if (arr!= NULL ) {
			printf("Hint: set cell to %d\n", getSolForCellIlp(arr, x, y, mul));
			free(arr);
		} else {
			printf("Validation failed: board is unsolvable\n");
	}
	}
}

void solutionsNum(boards* board) {
	int num;
	if (isWrong(board)) { /*can't execute*/
		printf("Error: board contains erroneous values\n");
		return;
	}
	if (boardFull(board)) {
		num = 1;
	} else {
		num= exhBackTSolver(board);
}
	printf("Number of solutions: %d\n", num);
	if (num == 1) {		/*there is one solution*/
		printf("This is a good board!\n");
	} else if (num>1) {
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
}

int autofill(boardStrc* game, commands* parsedComm, char* userComm,char mode) {
	int row = game->board->blockRow;
	int col = game->board->blockCol;
	Stack* stack = NULL;
	int i;
	int j;
	int value;
	int markErrors;

	if (isWrong(game->board)) {
		printf("Error: board contains erroneous values\n");
		return 0; /*continue the game*/
	}
	for (i= 0; i<col*row; i++) {
		for (j= 0; j<col*row; j++) {
			if (game->board->boardMain[i][j] == 0) {
				value = oneSolutionForCell(game->board, j+1, i+1);
				if (value!= 0) { /*if there is one possible solution for cell*/
					stack = push(stack, j+1, i+1, value);
					printf("Cell <%d,%d> set to %d\n", j+1, i+1, value);
		}
		}
	}
	}
	if (stack== NULL ) { /*didn't find any value*/
		printBoard(game->board, game->markErrors, mode);
		return 0; /*continue the current game*/
	}
	while (stack!= NULL ) {
		game->board->boardMain[(stack->y)-1][(stack->x)-1] = stack->value; /*adding values to the boards*/
		stack = delStack(stack); /*delete value from the stack*/
	}
	markUserErrors(game->board);
	game->moveList = addgameList(game->moveList, game->board);
	printBoard(game->board, game->markErrors,mode);
	if (boardFull(game->board)) {
		printf("Puzzle solved successfully\n");		/*if the board has been solved*/
		markErrors = game->markErrors;
		freeMemoAllGame(parsedComm,game,userComm);
		startGame(NULL,'i',markErrors);
		return 1; /*exit the game*/
	}
	return 0; /*the game isn't over*/
}

void reset(boardStrc* game,char mode) {
	gameList* move = game->moveList;
	while (move->prev != NULL ) {   /*until we are in the first move*/
		move = move->prev;
	}
	game->moveList = move;
	boardsCopy(game->board, game->moveList->board);
	freeRightgameList(game->moveList->next);
	game->moveList->next = NULL;
	printBoard(game->board,game->markErrors,mode);
	printf("Board reset\n");
}

int gameExit(commands* commParsed, boardStrc* game, char* comm){
	printf("Exiting...\n");
	freeMemoAllGame(commParsed, game, comm);
	return 1; 	/*exit from the game*/
}

int runComm(commands* cmd,char mode, boardStrc* game, char* origComm,char* eof,int valMarkErrors){
	if(eof==NULL || cmd->commName=='e') { /*exit*/
		return gameExit(cmd,game,origComm);
	}
	else if(cmd->commName=='S') { /*solve*/
		return solve(cmd,game,origComm,valMarkErrors);
	}
	else if(cmd->commName=='E') { /*edit*/
		return edit(cmd,game,origComm,valMarkErrors);
	}
	else if(mode == 's' && cmd->commName=='m') {	/*mark_errors*/
		markErrorVALUE(game,cmd->vars[0]);
	}
	else if(mode != 'i' && cmd->commName=='p') {	/*print_board*/
		printBoard(game->board,game->markErrors,mode);
	}
	else if(mode != 'i' && cmd->commName=='s') { /*set*/
		return set(game, cmd, mode,origComm);
	}
	else if(mode != 'i' && cmd->commName=='v'){	/*validate*/
		validate(game->board);
	}
	else if(mode == 'e' && cmd->commName=='g'){	/*generate*/
		generate(game,cmd->vars[0],cmd->vars[1]);
	}
	else if(mode != 'i' && cmd->commName=='u'){	/*undo*/
		undo(mode,game);
	}
	else if(mode != 'i' && cmd->commName=='r'){	/*redo*/
		redo(mode,game);
	}
	else if(mode != 'i' && cmd->commName=='b'){	/*save*/
		saveBoards(mode,game->board,cmd->path);
	}
	else if(mode == 's' && cmd->commName=='h'){	/*hint*/
		hint(cmd->vars[0],cmd->vars[1],game->board);
	}
	else if(cmd->commName=='G' && mode== 's'){
		guess(cmd,cmd->threshold, game,origComm);		/*guess*/
	}
	else if(cmd->commName=='H' && mode== 's'){
		guessHint(cmd->vars[0],cmd->vars[1], game->board);		/*guess hint*/
	}
	else if(mode != 'i' && cmd->commName=='n'){	/*num_solutions*/
		solutionsNum(game->board);
	}
	else if(mode == 's' && cmd->commName=='a'){	 /*autofill*/
		return autofill(game,cmd,origComm,mode);
	}
	else if(mode != 'i' && cmd->commName=='R'){/*reset*/
		reset(game,mode);
	}
	else if(cmd->commName=='N'){}
	else{printf("ERROR: invalid command\n");}
	return 0;
}

int commandNotAvailableInCurrMode(commands* cmd,char mode){
	if(cmd->commName=='m' && mode != 's'){/*mark_errors*/
		printf("ERROR: command mark_errors is only available in solve mode\n");
		return 1;
	}
	if(cmd->commName=='p' && mode == 'i'){/*print_board*/
		printf("ERROR: command print_board is only available in solve and edit mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='s') { /*set*/
		printf("ERROR: command set is only available in solve and edit mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='v'){	/*validate*/
		printf("ERROR: command validate is only available in solve and edit mode\n");
		return 1;
	}
	if(mode != 'e' && cmd->commName=='g'){	/*generate*/
		printf("ERROR: command generate is only available in edit mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='u'){	/*undo*/
		printf("ERROR: command undo is only available in solve and edit mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='r'){	/*redo*/
		printf("ERROR: command redo is only available in solve and edit mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='b'){	/*save*/
		printf("ERROR: command save is only available in solve and edit mode\n");
		return 1;
	}
	if(mode != 's' && cmd->commName=='h'){	/*hint*/
		printf("ERROR: command hint is only available in solve mode\n");
		return 1;
	}
	if(cmd->commName=='G' && mode!= 's'){/*guess*/
		printf("ERROR: command guess is only available in solve mode\n");
		return 1;
	}
	if(cmd->commName=='H' && mode!= 's'){/*guess_hint*/
		printf("ERROR: command guess_hint is only available in solve mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='n'){	/*num_solutions*/
		printf("ERROR: command num_solutions is only available in solve and edit mode\n");
		return 1;
	}
	if(mode != 's' && cmd->commName=='a'){	 /*autofill*/
		printf("ERROR: command autofill is only available in solve mode\n");
		return 1;
	}
	if(mode == 'i' && cmd->commName=='R'){/*reset*/
		printf("ERROR: command reset is only available in solve and edit mode\n");
		return 1;
	}
	return 0;
}
int commandNameNotValid(commands* cmd){
	/*If the command name is none of the following:
	 exit,solve,edit,mark_errors,print_board,set,validate,generate,undo,redo,save,hint,guess,
	 * guess_hint,num_solutions,autofill,reset*/
	if(cmd->commName!='e' && cmd->commName!='S' && cmd->commName!='E'
			&& cmd->commName!='m' && cmd->commName!='p' && cmd->commName!='s'
			&& cmd->commName!='v'&& cmd->commName!='g' && cmd->commName!='u'&& cmd->commName!='r' && cmd->commName!='b'
			&& cmd->commName!='h' &&cmd->commName!='G'&& cmd->commName!='H' && cmd->commName!='n' && cmd->commName!='a'
			&& cmd->commName!='R' && cmd->commName!='N' && cmd->commName!='W') {
		printf("ERROR: invalid command name\n");
		return 1;
	}
	return 0;
}

int handleEof(commands* commParsed, boardStrc* game, char* origComm){
	char ch;
	if(feof(stdin)){
		gameExit(commParsed,game,origComm); /*got EOF	, so we exit the game*/
		return 1;
	}
	if(origComm[255]!= '\0'){
		while ((ch=getchar()) !='\n' ){ /*if command is too big, we clean the buffer*/
			if(ch==EOF){
				gameExit(commParsed,game,origComm);
				return 1;
		}
	}
	}
	return 0;
}

void startGame(char* path, char mode, int valMarkErrors) {
	boards* board = NULL;
	boardStrc* game = NULL;
	commands *cmd = NULL;
	char* eof;
	char *origComm = (char*)calloc(CMDSIZE, sizeof(char));
	memFuncError("calloc", origComm);
	if(mode != 'i'){ /*if it isn't Init mode*/
		board = pathLoading(path, mode);
		game = generateGame(board, valMarkErrors);
		printBoard(game->board, game->markErrors,mode);
	}
	while (1) {
		printf("Enter your command:\n");
		eof = fgets(origComm, CMDSIZE, stdin); /*get user's command*/
		cmd =  commParse(origComm);

		if(commandNameNotValid(cmd))/*if the command name is not valid*/
			continue;
		if(commandNotAvailableInCurrMode(cmd,mode)==1)/*if the command is not available in the current mode*/
			continue;
		if(cmd->commName=='W'){/*if we didn't get enough variables as we have excepted*/
			printf("ERROR: Wrong number of parameters\n");
			continue;
		}

		if(cmd->vars[cmd->numberVariables]==-2){/*if we to too many variables*/
					printf("ERROR: Wrong number of parameters\n");
					continue;
				}
		if(runComm(cmd,mode,game,origComm,eof,valMarkErrors) == 1)
			return;

		if(handleEof(cmd,game,origComm))/*check if we got EOF and need to clean the buffer*/
			return;

		origComm[255] = '\0'; /*create size for the next command*/
		free(cmd);
	}
	free(origComm);
	if(game!=NULL){
		freeGame(game);
	}
	return;
}
