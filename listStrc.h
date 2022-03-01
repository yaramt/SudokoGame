/*
 * listStrc.h
 */

#ifndef LISTSTRC_H_
#define LISTSTRC_H_

/**
 * The listStrc class is a structure that has two elements:
 * x=  an integer representing the value inserted to the beginning of the list
 *prevNode= is a listStrc representing the node that has been added to the list previously
 */
typedef struct listStrc
{
	struct listStrc* prevNode;
	int x;
}listStrc;


/**
 * The function gets as a parameter: listStrc list.
 * It removes the first element of the list and returns the list after the adjustment
 */
listStrc* removefirst(listStrc* list);

/**
 * The function gets 2 parameters:
 * list= the list that we will add to the beginning of it a new value x (x will become the first element of the list)
 * It returns the list after the adjustment
 */
listStrc* addfirst(listStrc* list,int x);

#endif /* LISTSTRC_H_ */
