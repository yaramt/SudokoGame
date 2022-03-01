/*
 * Parser.h
 */


#ifndef PARSER_H_
#define PARSER_H_

/**
 * Parser
 * This model gets commands from the user - it manages the user's side.
 */

/**
 * This command struct has the name of the command, the path (for "edit"/"save"/"solve" command)
 * and all the variables of the command. threshold relevant only when the command is guess
 */
typedef struct commands
{
	char commName;
	char path[256];
	int vars[4];
	float threshold;
	int numberVariables;
}commands;

/*this is a function in stdlib I added it's signature so no warnings while compiling occur*/
float strtof (const char *string, char **tailptr);
/**
 * The function gets the expected amount of variables we seem to get, the user's command and the command object
 * and it sets the command name.
 */
void setCommName(int* getVars, char* commStr, commands* comm);

/**
 * The function gets the command's object that we have built and if the command
 * is "edit"/"save"/"solve": it sets the command's path.
 */
void setCommPath(commands *comm);

/**
 * The function gets the variables number that we should get and the command's object we that we build,
 * and it sets all the variables that are needed for the command.
 */
void setCommVars(int varsNum, commands *comm);
/**
 * The function gets thecommand's object that we build, this function is called when the command is guess
 * and it sets the threshold of comm to the user's requested value
 */
void setCommThreshold(commands *comm);

/**
 * The function gets the user's command and builds an object of the "commands" struct,includes important
 * information about it. it returns this objects.
 */
commands *commParse(char *commStr);

#endif /* PARSER_H_ */
