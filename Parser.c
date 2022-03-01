/*
 * Parser.c
 */
#include "Parser.h"
#include <string.h>
#include "Checkers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void setCommName(int* getVars,char* commStr,commands* cmd){
	char* tok;
	tok = strtok(commStr,  " \t\r\n");
	if(tok==NULL){	 /*N= nothing happens, go to the next loop*/
		cmd->commName='N';
	}
	else if(strcmp(tok, "solve")==0){/*S= solve*/
		cmd->commName='S';
	}
	else if(strcmp(tok, "edit")==0){/*E= Edit*/
		cmd->commName='E';
	}
	else if(strcmp(tok, "mark_errors")==0){/* m= mark errors */
		cmd->commName='m';
		*getVars = 1;
	}
	else if(strcmp(tok, "print_board")==0){/*p= print board*/
		cmd->commName='p';
	}
	else if(strcmp(tok, "guess")==0){		/* G=  guess */
		cmd->commName='G';
		*getVars = 0;
	}
	else if(strcmp(tok, "guess_hint")==0){		/* H=  guess hint */
		cmd->commName='H';
		*getVars = 2;
	}
	else if(strcmp(tok, "set")==0){/*s= set*/
		cmd->commName='s';
		*getVars = 3;
	}
	else if(strcmp(tok, "validate")==0){/* v= validate */
		cmd->commName='v';
	}
	else if(strcmp(tok, "generate")==0){/* g=  generate */
		cmd->commName='g';
		*getVars = 2;
	}
	else if(strcmp(tok, "undo")==0){/* u= undo */
		cmd->commName='u';
	}
	else if(strcmp(tok, "redo")==0){/* r= redo */
		cmd->commName='r';
	}
	else if(strcmp(tok, "save")==0){/* b= save */
		cmd->commName='b';
	}
	else if(strcmp(tok, "hint")==0){/* h= hint */
		cmd->commName='h';
		*getVars = 2;
	}
	else if(strcmp(tok, "num_solutions")==0){/* n= num_solutions */
		cmd->commName='n';
	}
	else if(strcmp(tok, "autofill")==0){/* a= autofill */
		cmd->commName='a';
	}
	else if(strcmp(tok, "reset")==0){/* R= reset */
		cmd->commName='R';
	}
	else if(strcmp(tok, "exit")==0){/* e= exit */
		cmd->commName='e';
	}
	else{
		cmd->commName='W';/* W= wrong command */
	}
}

void setCommPath(commands *cmd){
	char* tok;
	if(cmd->commName == 'E' || cmd->commName == 'S' || cmd->commName == 'b'){
		tok = strtok(NULL,  " \t\r\n");
		if(tok!=NULL){
			strcpy(cmd->path,tok);
		}
		else{
			if(cmd->commName != 'E'){
				cmd->commName = 'W';
			}
			cmd->path[0] = '\0';
		}
	}
}

void setCommVars(int varsNum, commands *cmd){
	int ind;
	char *tok;
	ind = varsNum;
	for(;ind>0; ind--){	  /*getting the variables of the command*/
		tok = strtok(NULL,  " \t\r\n");
		if(tok==NULL){	/*if we didn't get enough variables as we have excepted*/
			cmd->commName='W';
			break;
		}
		if(isStrANumber(tok) == 0){
			cmd->vars[varsNum-ind] = -1; /*if input isn't a number*/
		}
		else{
			cmd->vars[varsNum-ind] = strtol(tok, NULL, 10);
		}
	}
	tok = strtok(NULL,  " \t\r\n");
	if(tok!=NULL){	/*if we got too many variables*/
		cmd->vars[varsNum]=-2;
			}
}

void setCommThreshold(commands *comm){
	char *tok;
	tok = strtok(NULL,  " \t\r\n");
	comm->threshold=strtof(tok,NULL);
}

commands *commParse(char *commStr){
	int getVars = 0;
	commands *cmd = (commands*)malloc(sizeof(commands));
	memFuncError("malloc", cmd);
	setCommName(&getVars,commStr,cmd);
	cmd->numberVariables=getVars;
	if(cmd->commName=='G'){/*set threshold if the command is guess*/
			setCommThreshold(cmd);
		}
	setCommPath(cmd);
	setCommVars(getVars, cmd);
	return cmd;
}
