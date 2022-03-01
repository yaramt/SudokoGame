/*
 * possibleValuesList.c
 */
#include <stdio.h>
#include "possibleValuesList.h"
#include <stdlib.h>
#include "Checkers.h"

int sizeList(possibleValuesList *listNode){
	int c = 1;
	possibleValuesList *temp= listNode;
	if(listNode==NULL)
		return 0;
	while(temp->nextNode!=0){
		temp = temp->nextNode;
		c++;
	}
	return c;
}

int removeList(possibleValuesList **listNode, int i){
	int removedInt;
	int j;
	possibleValuesList *removedNode;
	possibleValuesList *temp = *listNode;

	if(i == 0)
	{
		removedNode = *listNode;
		removedInt = removedNode->vFirst;
		*listNode = (*listNode)->nextNode;
		free(removedNode);
		return removedInt;
	}
	else
	{
		for(j=0; j<i-1; j++)
			temp = temp->nextNode;
		removedNode = temp->nextNode;
		removedInt =removedNode->vFirst;
		temp->nextNode = removedNode->nextNode;
		free(removedNode);
		return removedInt;
	}
}

void addList(possibleValuesList **listNode, int v){
	possibleValuesList *temp = *listNode;
	possibleValuesList *addedNode=(possibleValuesList*)malloc(sizeof(possibleValuesList)); /*creating the node to be added*/
	memFuncError("malloc", addedNode);
	addedNode->nextNode = NULL;
	addedNode->vFirst = v;
	if(*listNode == NULL){
		*listNode = addedNode;
		return;
	}
	while((temp->nextNode)!=NULL)
		temp = temp->nextNode;
	temp->nextNode = addedNode;
}

void freeMemo(possibleValuesList *listNode){
	if(listNode != NULL){
		freeMemo(listNode->nextNode);
		free(listNode);
	}
}
