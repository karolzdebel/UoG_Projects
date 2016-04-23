
/********************************************
shell.h -- Shell library.
Last updated:  11:27 AM February-10-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********************************************/

#define _XOPEN_SOURCE 500
#define _POSIX_SOURCE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>    
#include <signal.h>
#include <assert.h>

#define DEBUG
#define MAX_INPUT 1000
#define MAX_PATH 1000
#define MAX_PNUM 100
#define __EXIT 0
#define __NOARG 1
#define __ARG 2
#define __REDIR_NONE 0
#define __REDIR_TFILE 1 //Output is redirected to file
#define __REDIR_FFILE 2 //Input is redirected from a file
#define __PIPE_OUTPUT 3 //Pipe output to another command

/*Path linked list*/
typedef struct Path{
	char *dir;
	struct Path *next;
}Path; 

/************************FUNCTION PROTOTYPES*************************/
/*********************************************************************
 * FUNCTION NAME: execCmd
 * PURPOSE: Executes command synchronously.
 * ARGUMENTS: . Command being run (char *cmd).
 *			  . Arguments being run alongside command(char **args).
 *			  . Type of input or ouput redirection(int outinType).
 *			  . Where the input or output is being
 *			    redirected(char *redirName).
 *			  . Status of the execution in the child(int *status).
 *********************************************************************/
void execCmd(char *cmd,char **args,int outinType
	,char *redirName,int *status);
/*********************************************************************
 * FUNCTION NAME: execBackCmd
 * PURPOSE: Execute command asynchronously.
 * ARGUMENTS: . Command being run(char *cmd).
 *			  . Arguments being run alongside command(char **args).
 *			  . Type of input or output redirection(int outinType).
 *			  . Where the input or output is being
 * 				redirected(char *redirName).
 *********************************************************************/
void execBackCmd(char *cmd,char **args,int outinType
	,char *redirName);
/*********************************************************************
 * FUNCTION NAME: execPipeCmd
 * PURPOSE: Execute one level of piping between two commands.
 * ARGUMENTS: . First command being run producing output (char *cmd).
 *			  . Arguments being run alongside first command(char **args).
 *			  . Second command being run, which uses output from
 *				first command.
 *			  . Arguments running alongside second
 *				command(char **pipeargs).
 *********************************************************************/
void execPipeCmd(char *cmd,char **args
	,char *pipecmd,char **pipeargs);
/*********************************************************************
 * FUNCTION NAME: getDirs
 * PURPOSE: . Gets all the directories present in unix path variable.
 * RETURNS: . Array of strings where each string is a directory
 * 			   in the path variable.
 *********************************************************************/
char **getDirs();
/*********************************************************************
 * FUNCTION NAME: addDir
 * PURPOSE: Add directory to path structure.
 * ARGUMENTS: . Path structure the directory is being added
 * 				to (Path *path).
 *			  . Directory being added to the structure (char *dir).
 * RETURNS: Boolean value based on whether EOF was found.
 *********************************************************************/
void addDir(Path *path,char *dir);
/*********************************************************************
 * FUNCTION NAME: freePath
 * PURPOSE: Free's all the memory stored in the path struct(Path *path).
 * ARGUMENTS: . Path to be free'd (Path *path).
 *********************************************************************/
void freePath(Path **path);
/*********************************************************************
 * FUNCTION NAME: initializePath
 * PURPOSE: Initialize path structure.
 * ARGUMENTS: . Path to be initialized(Path **path).
 *********************************************************************/
void initializePath(Path **path);
/*********************************************************************
 * FUNCTION NAME: getPipeArgs
 * PURPOSE: Get arguments from string.
 * ARGUMENTS: . User input stored in a stirng(char *buffer).
 *			  . Name of the command (char *name).
 * RETURNS: Arguments stored in a string array.
 *********************************************************************/
char **getPipeArgs(char *buffer,char *name);
/*********************************************************************
 * FUNCTION NAME: getPipeCmd
 * PURPOSE: Gets the command from string.
 * ARGUMENTS: . User input stored in a string(char *buffer).
 * RETURNS: Command(char *).
 *********************************************************************/
char *getPipeCmd(char *buffer);
/*********************************************************************
 * FUNCTION NAME: getRedirName
 * PURPOSE: Gets the name of where the output/input is being redirected.
 * ARGUMENTS: . User input stored in a string(char *input).
 *			  . Type of redirection occurring(int type).
 * RETURNS: Where input or output is being redirected(char *).
 *********************************************************************/
char *getRedirName(char *input,int type);
/*********************************************************************
 * FUNCTION NAME: getOutInType
 * PURPOSE: Gets the type of redirection that is occurring.
 * ARGUMENTS: . Command being run (char * command).
 * RETURNS: Type of redirection(__REDIR_NONE, __REDIR_TFILE
 *		    , __REDIR_FFILE, __PIPE_OUTPUT)
 *********************************************************************/
int getOutInType(char *command);
/*********************************************************************
 * FUNCTION NAME: killChild
 * PURPOSE: Signal handler that kills all background processes.
 * ARGUMENTS: . The signal(int signal).
 *********************************************************************/
void killChild(int signal);
/*********************************************************************
 * FUNCTION NAME: getBackCmd
 * PURPOSE: Get the command.
 * ARGUMENTS: . Command stored in string(char *command).
 * RETURNS: Command(char *).
 *********************************************************************/
char *getBackCmd(char *command);
/*********************************************************************
 * FUNCTION NAME: isBackCmd
 * PURPOSE: Check whether command is a background command.
 * ARGUMENTS: . Command to be checked(char *command).
 * RETURNS: Boolean value based on whether the command
 *			is a back command.
 *********************************************************************/
bool isBackCmd(char *command);
/*********************************************************************
 * FUNCTION NAME: getCmd
 * PURPOSE: Get command from string.
 * ARGUMENTS: . String in which the command is found(char *buffer).
 * RETURNS: Command(char *).
 *********************************************************************/
char *getCmd(char *buffer);
/*********************************************************************
 * FUNCTION NAME: getArgs
 * PURPOSE: Gets the arguments from string.
 * ARGUMENTS: . String that stores the arugments and
 *              command(char *buffer).
 * RETURNS: Arguments stored in string array(char **).
 *********************************************************************/
char **getArgs(char *buffer);
/*********************************************************************
 * FUNCTION NAME: isEmpty
 * PURPOSE: Checks whether the string given is an empty command.
 * ARGUMENTS: . String to be checked(char *string).
 * RETURNS: Boolean value based on whether the string is empty.
 *********************************************************************/
bool isEmpty(char *string);
/*********************************************************************
 * FUNCTION NAME: freeArgs
 * PURPOSE: Free argument string array.
 * ARGUMENTS: . Arguments to be free'd(char ***args).
 *********************************************************************/
void freeArgs(char ***args);
/***********************FUNCTION PROTOTYPES***************************/