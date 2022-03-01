/*
 * Checkers.h
 */
#include <stdio.h>
#include <stdlib.h>
#ifndef CHECKERS_H_
#define CHECKERS_H_

/**
 * The function gets a pointer to a string str.
 * It checks whether str consists of numbers only and returns 1 if it does, 0 otherwise
 */
int isStrANumber(char* str);

/**
 * The function gets 2 parameters:
 * returnedV= the returned valued of the function that prints to the file
 * file= the file that we wrote to
 * It checks if writing to file failed. if it did prints message and close file.
 *It returns 1, closes file and prints a message for the user if writing failed, returns 0 otherwise
 */
int WritingFileFailed(int returnedV,FILE* file);

/**
 * The function gets 5 parameters:
 * col=the column of a cell, row= the row of a cell, val= the v value of a cell
 * (x1,x2) is a range where x1 is the beginning and x2 is the end.
 * The function checks if x1<row<=x2 and x1<col<=x2 and x1<=val<=x2
 * It returns 1 if all of the measurements are in the right range, 0 otherwise
 */
int checkMeasurements(int col,int row,int val,int x1,int x2);

/**
 * The function gets 2 parameters:
 * funcStr= a string representing the name of one of the memory functions,
 * returnPnt= pointer to the returned value of the function with the name functStr
 * The function prints an error and exits if the function failed
 */
void memFuncError(char funcStr[], void *returnPnt);

/**
 * The function gets a path p as a parameter
 * It returns 1 if the path is valid, 0 otherwise
 */
int checkPath(char* p);

#endif /* CHECKERS_H_ */
