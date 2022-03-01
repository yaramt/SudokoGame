/*
 * Stack.h
 */

#ifndef STACK_H_
#define STACK_H_


/**
 * a struct with nodes that have the value that need to be entered,
 *  x represents the cell's column and y is the row.
 */
typedef struct stack
{
	struct stack* prv;
	int value;
	int x;
	int y;
}Stack;

/**
 * The function gets the stack and deletes the top of it. It returns the new top of the stack.
 */
Stack* delStack(Stack* stack);

/**
 * The function adds a new node to the Stack. it gets the stack, the value of the new node,
 * and its column and row. it returns the stack's new top.
 */
Stack* push(Stack* stack,int x,int y,int value);

#endif /* STACK_H_ */
