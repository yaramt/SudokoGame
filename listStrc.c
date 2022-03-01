/*
 * listStrc.c
 */
#include "listStrc.h"
#include <stdlib.h>
#include <stdio.h>
#include "Checkers.h"


listStrc* removefirst(listStrc* list){
	listStrc *node;
	if(list != NULL){
	node = list->prevNode;
	free(list);
	return node;
	}
	return NULL;
}

listStrc* addfirst(listStrc* list,int x){
	listStrc *node = (listStrc*)malloc(sizeof(listStrc));
	memFuncError("malloc", node);
	node->prevNode = list;
	node->x = x;
	return node;
}
