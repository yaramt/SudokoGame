/*
 * possibleValuesList.h
 */

#ifndef POSSIBLEVALUESLIST_H_
#define POSSIBLEVALUESLIST_H_

/**
 * This structure is a list of all possible values for a cell
 * vFirst is the value of the first node in the list
 * nextNode is a possibleValuesList containing the next value and remaining possible values(excluding vFirst)
 */
typedef struct possibleValuesList
{
	struct possibleValuesList *nextNode;
	int vFirst;
}possibleValuesList;

/**
 * The function gets a pointer to a possibleValuesList structure, listNode. where it points to the first node of the list
 * The function returns the size of the list
 */
int sizeList(possibleValuesList *listNode);

/**
 * The function gets 2 parameters:
 * listNode= pointer to the first node of the list ,i= the index of the element we want to remove
 * The function will remove the element with the specified index and will return the value of the removed element
 */
int removeList(possibleValuesList **listNode, int i);

/**
 * The function gets 2 parameters:
 * listNode= pointer to the first node of the list, v=the value of the node we want to add to the list
 * The function adds a new node with the value v to the end of the list
 */
void addList(possibleValuesList **listNode, int v);

/**
 * The function gets parameter listNode=the first node of the list
 * The function frees the whole list from memory starting from listNode until the last node
 */
void freeMemo(possibleValuesList *listNode);

#endif /* POSSIBLEVALUESLIST_H_ */
