/*
 * doubleList.c
 */

#include "Checkers.h"
#include "boardGame.h"
#include <stdlib.h>
#include <stdio.h>
#include "doubleList.h"


void freegameList(gameList *freeNode){
	gameList* currNode = freeNode;
	if(freeNode==NULL){
		return;
	}
	while((currNode->prev)!=NULL){
		currNode= currNode->prev;
	}
	freeRightgameList(currNode);
}

gameList* addgameList(gameList *node,boards* boards){
	gameList *newNode=(gameList*)malloc(sizeof(gameList));
	gameList *currNode = node;
	memFuncError("malloc", newNode);
	newNode->board = createBoard(boards->blockRow,boards->blockCol);
	boardsCopy(newNode->board, boards);
	newNode->prev= currNode;
	newNode->next = NULL;
	if (currNode==NULL){	/*if list is empty*/
		return newNode; }
	freeRightgameList(node->next);	/*if we should delete old moves*/
	currNode->next = newNode;
	return newNode;
}

void freeRightgameList(gameList *freeNode){
	if(freeNode!=NULL){
		freeRightgameList(freeNode->next);
		boardsFree(freeNode->board);
		free(freeNode);
}
}


