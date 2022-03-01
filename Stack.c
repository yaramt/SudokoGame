/*
 * Stack.c
 */

#include "Stack.h"
#include "Checkers.h"
#include <stdio.h>
#include <stdlib.h>


Stack* delStack(Stack* stack){
	Stack* newTop;
	if(stack == NULL){		/*if stack is empty*/
		return NULL;
	}
	newTop = stack->prv;
	free(stack);
	return newTop;
}

Stack* push(Stack* stack,int x,int y,int value){
	Stack* newNode = (Stack*)malloc(sizeof(Stack));
	memFuncError("malloc", newNode);

	newNode->value = value;
	newNode->x = x;
	newNode->y = y;
	newNode->prv = stack;
	return newNode;
}
