/*
 * main.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include <time.h>


#define VALMARKERRORS 1

int main() {
	printf("Sudoku- show me what you got\n------\n");
	srand((time(NULL))); /*seed*/
	startGame(NULL,'i',VALMARKERRORS);
	return 0;
}
