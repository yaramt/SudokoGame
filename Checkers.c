/*
 * Checkers.c
 */
#include <stdio.h>
#include <stdlib.h>
int isStrANumber(char* str){
	int x = 0;
	while(str[x] != '\0'){
		if(str[x]>57 || str[x]<48)
			return 0;
		x++;}
	return 1;
}

int WritingFileFailed(int returnedV,FILE* file){
	if(returnedV>=0)
		return 0;
	printf("Error writing to the file");
	fclose(file);
	return 1;
}

int checkMeasurements(int col,int row,int val,int x1,int x2){
	if((col<x1+1 || col>x2)||(row<x1+1 || row>x2)|| (val<x1 || val>x2))
		return 0;
	return 1;
}

void memFuncError(char funcStr[], void *returnPnt){
	if(returnPnt == NULL){
		printf("Error: %s has failed\n", funcStr);
		exit(0);
	}
}

int checkPath(char* p){
	FILE* file;
	file = fopen(p,"r");
	if(file == NULL)
		return 0;
	fclose(file);
	return 1;
}
