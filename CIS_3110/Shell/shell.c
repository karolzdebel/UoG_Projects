
/********************************************
shell.c -- Shell library implementation.
Last updated:  11:27 AM February-10-16

Name: Karol Zdebel
Student Number: 0892519
Contact: kzdebel@mail.uoguelph.ca
********************************************/

#ifndef __SHELL_H
#define __SHELL_H
#include "shell.h"
#endif

/*Global variable for child process id*/
pid_t curpid;	//Array of background processes

/*Create a copy of a string*/
static char *getCopy(char *buffer){
	char *copy;
	copy = malloc(sizeof(char)
		*(strlen(buffer)+1));
	strcpy(copy,buffer);
	return copy;
}

void execCmd(char *cmd,char **args,int outinType
	,char *redirName,int *status){

	int ret;

	/*Create child process and wait for
	  it to terminate*/
	curpid = fork();

	waitpid(curpid,status,0);
	if (curpid == -1){
		exit(errno);
	}

	if (curpid == 0){
		/*Redirect output or input if requested*/
		if (outinType == __REDIR_TFILE){
			stdout = freopen(redirName,"a+",stdout);
			free(redirName);
		}
		else if (outinType == __REDIR_FFILE){
			stdin = freopen(redirName,"r",stdin);
			free(redirName);
		}

		/*Execute command*/		
		ret = execvp(cmd,args);
		if (ret == -1){
			free(cmd);
			freeArgs(&args);
			exit(errno);
		}
	}
	kill(curpid,SIGKILL);

}

/*Executes command in the background*/
void execBackCmd(char *cmd,char **args,int outinType
	,char *redirName){

	int ret;

	sigset(SIGCHLD,(void (*)(int))killChild);
	curpid = fork();
	if (curpid == -1){
		exit(errno);
	}

	/*Execute the command asynchronously in the
	  child process*/
	if (curpid == 0){

		/*Redirect output or input if requested*/
		if (outinType == __REDIR_TFILE){
			stdout = freopen(redirName,"w",stdout);
			free(redirName);
		}

		else if (outinType == __REDIR_FFILE){
			stdin = freopen(redirName,"r",stdin);
			free(redirName);
		}

		ret = execvp(cmd,args);
		if (ret == -1){
			exit(errno);
		}

		/*Free memory if error returned*/
		free(cmd);
		freeArgs(&args);
		exit(errno);
	}
}

/*Executes two commands, the second one
uses the output of the first as input*/
void execPipeCmd(char *cmd,char **args
	,char *pipecmd,char **pipeargs){
	int fd[2],ret;
	int stdinCopy= dup(0);
	int stdoutCopy = dup(1);

	/*Create the pipe*/
	ret = pipe(fd);
	if (ret == -1){
		exit(errno);
	}

	/*Execute first command in first
	  child process*/
	curpid = fork();
	if (curpid == -1){
		exit(errno);
	}

	if (curpid == 0){

		/*Close pipe read*/
		close(fd[0]);

		/*Redicrect stdout to pipe write*/
		close(STDOUT_FILENO);
		ret = dup2(fd[1],STDOUT_FILENO);
		if (ret == -1){
			exit(errno);
		}

		/*Execute first command*/
		ret = execvp(cmd,args);
		if (ret == -1){
			free(cmd);
			freeArgs(&args);
			exit(errno);
		}		
	}

	curpid = fork();
	if (curpid == -1){
		exit(errno);
	}

	/*Execute other command in second child
	 using piped stdin*/
	if (curpid == 0){

		/*Close pipe write*/
		close(fd[1]);

		/*Redirect STDIN to pipe*/
		close(STDIN_FILENO);
		ret = dup2(fd[0],STDIN_FILENO);
		if (ret == -1){
			exit(errno);
		}

		/*Execute second command*/
		ret = execvp(pipecmd,pipeargs);
		if (ret == -1){
			exit(errno);
		}

		/*Free memory if error returned*/
		free(cmd);
		freeArgs(&args);
		exit(errno);
	}

	close(fd[1]);
	close(fd[0]);

	ret = dup2(stdoutCopy,STDOUT_FILENO);
	if (ret == -1){
		exit(errno);
	}

	ret = dup2(stdinCopy,STDIN_FILENO);
	if (ret == -1){
		exit(errno);
	}

	/*Wait for child processes to finish
	  before continuing*/
	wait(0);
	wait(0);
}

/*Temporarily void for testing, set to (char**) */
char **getDirs(){
	char **args,path[MAX_PATH];
	char **dirs=NULL,*token=NULL;
	int fd[2],numDirs=0,nread,ret;

	/*Set up the pipe*/
	ret = pipe(fd);
	if (ret == -1){
		exit(errno);
	}

	/*Create child process*/
	curpid = fork();
	if (curpid == -1){
		exit(errno);
	}

	/*Child executes get path command
	  and writes output to pipe*/
	if (curpid == 0){

		/*Close input*/
		close(fd[0]);

		/*Redirect stdout to pipe write*/
		ret = dup2(fd[1],STDOUT_FILENO);
		if (ret == -1){
			exit(errno);
		}

		args = malloc(sizeof(char*)*7);
		args[0] = malloc(sizeof(char)*20);
		args[1] = malloc(sizeof(char)*20);
		args[2] = NULL;
		strcpy(args[0],"getconf");
		strcpy(args[1],"PATH");
			execvp("getconf",args);
			exit(errno);
		}

	/*Close output*/
	close(fd[1]);

	/*Read piped output*/
	nread = read(fd[0],path,MAX_PATH);
	path[nread-1] = '\0';

	/*Wait for child process*/
	wait(0);
	kill(curpid,SIGKILL);

	close(fd[0]);
	close(fd[1]);

	/*Seperate the directories by ':'
	  and store them in an array*/
	token = strtok(path,":");

	while (token != NULL){
		numDirs++;

		/*Allocate memory for directory path
		  and store it*/
		if (dirs == NULL){
			dirs = malloc(sizeof(char*));
		}
		else{
			dirs = realloc(dirs,sizeof(char*)
				*(numDirs+1));
		}
		dirs[numDirs-1] = malloc(sizeof(char)
			*(strlen(token)+1));
		strcpy(dirs[numDirs-1],token);

		token = strtok(NULL,":");
	}
	dirs[numDirs] = NULL;
	return dirs;
}

/*Allocate memory and initialize Path struct*/
void initializePath(Path **path){
	*path = malloc(sizeof(Path));
	(*path)->next = NULL;
	(*path)->dir = NULL;
}

/*Free path and all its elements*/
void freePath(Path **path){
	if ((*path)->next){
		freePath(&((*path)->next));
	}

	free((*path)->dir);
	free((*path));
	(*path) = NULL;
}

/*Add directory to path list*/
void addDir(Path *path,char *dir){
	Path *temp;
	char *dirCopy = getCopy(dir);

	#ifdef DEBUG
	assert(path);
	#endif

	/*Insert into main data structure*/
	if (!path->dir){
		path->dir = malloc(sizeof(char)
			*(strlen(dir)+1));
		strcpy(path->dir,dirCopy);
	}

	/*Insert into first next*/
	else if (!path->next){
		initializePath(&(path->next));
		path->next->dir = malloc(sizeof(char)
			*(strlen(dir)+1));
		strcpy(path->next->dir,dirCopy);
	}

	/*Insert into last position*/
	else{
		temp = path->next;
		while (temp->next){
			temp = temp->next;
		}
		initializePath(&(temp->next));
		temp->next->dir = malloc(sizeof(char)
			*(strlen(dir)+1));
		strcpy(temp->next->dir,dirCopy);
	}

	free(dirCopy);
}

/*Gets the name of whereever output 
  or input is going to be redirected*/
char *getRedirName(char *buffer,int type){
	char *token,*name = NULL;
	char *delim,*input;
	int i;

	input = getCopy(buffer);
	delim = malloc(sizeof(char)*2);
	switch(type){
		case __REDIR_TFILE:
			delim[0] = '>';
			break;

		case __REDIR_FFILE:
			delim[0] = '<';
			break;

		case __PIPE_OUTPUT:
			delim[0] = '|';
			break;
	}
	delim[1] = '\0';

	/*Get the name*/
	token = strtok(input,delim);
	token = strtok(NULL,delim);

	/*Remove space or \n if one exists*/
	if (token[0] == ' '){
		for (i=0;i<strlen(token);i++){
			token[i] = token[i+1];
		}
	}
	for (i=0;i<strlen(token);i++){
		if (token[i] == '\n'){
			token[i] = '\0';
		}
	}

	/*Copy the name*/
	name = malloc(sizeof(char)
		*(strlen(token)+1));
	strcpy(name,token);

	if (name == NULL){
		#ifdef DEBUG
		printf("No name found, returning NULL\n");
		#endif
	}

	free(input);
	free(delim);

	return name;
}


/*Returns a value based on the type of 
  output or input redirection that is occuring*/
int getOutInType(char *input){
	int i, outin, outinCount = 0;
	char *command;

	command = getCopy(input);
	outin = __REDIR_NONE;
	/*Check for characters representing different
	  output and input combinations*/
	for (i=0;i<strlen(command)-1;i++){
		if (command[i] == ' '){
			/*Redirection of output to file*/
			if (command[i+1] == '>'){
				outin = __REDIR_TFILE;
				outinCount++;
			}
			/*Input that is redirected from a file*/
			else if (command[i+1] == '<'){
				outin = __REDIR_FFILE;
				outinCount++;
			}
			/*Pipe output*/
			else if (command[i+1] == '|'){
				outin = __PIPE_OUTPUT;
				outinCount++;
			}

		}
	}

	if (outinCount > 1){
		#ifdef DEBUG
		printf("outinCount > 1. Errro!\n");
		exit(1);
		#endif
	}
	free(command);

	return outin;
}

void killChild(int signal){
	wait(0);
	kill(curpid,SIGKILL);
}

/*Checks whether command is a background running
  command and returns a boolean as a result*/
bool isBackCmd(char *command){
	if (command[strlen(command)-1] == '&'){
		return true;
	}
	return false;
}

/*Removes the & from the background command
  and returns the actual command to be ran*/
char *getBackCmd(char *command){
	char *backcmd;
	backcmd = malloc(sizeof(char)
		*(strlen(command)+1));
	strcpy(backcmd,command);
	if (backcmd[strlen(backcmd)-1] == '&'){
		backcmd[strlen(backcmd)-1] = '\0';
	}
	return backcmd;
}

/*Free args array*/
void freeArgs(char ***args){
	int i=0;
	while ((*args)[i]){
		free((*args)[i]);
		i++;
	}
	free(*args);
}

bool isEmpty(char *string){
	int i;

	/*If a non white space character is found
	  return false,return true otherwise*/
	for (i=0;i<strlen(string);i++){
		if (string[i] != ' ' && string[i] != '\t'
			&& string[i] != '\n' && string[i] != '\r'){
			return true;
		}
	}
	return false;
}
char **getPipeArgs(char *buffer,char *name){
	char **tokens=NULL,**args=NULL,*token;
	char *input;
	int i,tokenCount=0,argNum;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	/*Remove end of line character*/
	token = strtok(input,"\n");

	/*Tokenize past the first command*/
	token = strtok(input,"|");

	/*Get all tokens seperated by a space*/
	while (token != NULL){
		tokenCount++;

		if (tokens == NULL){
			tokens = malloc(sizeof(char*));
		}
		else{
			tokens = realloc(tokens
				,sizeof(char*)*tokenCount);
		}
		
		tokens[tokenCount-1] = malloc(sizeof(char)
			*(strlen(token)+1));
		strcpy(tokens[tokenCount-1],token);

		token = strtok(NULL," ");
	}

	/*Store the name inside the args array*/
	args = malloc(sizeof(char*));
	args[0] = malloc(sizeof(char)
		*(strlen(name)+1));
	strcpy(args[0],name);

	/*Store all tokens that have the proper argument
	  syntax in the argument array*/
	argNum = 1;
	for (i=0;i<tokenCount;i++){
		if (tokens[i][0] == '-'){

			argNum++;
			args = realloc(args
				,sizeof(char*)*argNum);
			
			args[argNum-1] = malloc(sizeof(char)
				*(strlen(tokens[i])+1));
			strcpy(args[argNum-1],tokens[i]);
		}
	}
	/*If no arguments were found, create
	  an array with one empty string element*/
	if (args == NULL){
		args = malloc(sizeof(char*));
		args[0] = malloc(sizeof(char));
		args[0][0] = '\0';
		argNum++;
	}
	/*Null terminate the array*/
	args = realloc(args,sizeof(char*)
		*(argNum+1));
	args[argNum] = NULL;

	/*Free token array*/
	if (tokens != NULL){
		for (i=0;i<tokenCount;i++){
			free(tokens[i]);
		}
		free(tokens);
	}

	free(input);
	return args;
}
char **getArgs(char *buffer){

	char **tokens=NULL,*token;
	char *input;
	int tokenCount=0;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	/*Remove end of line character*/
	token = strtok(input,"\n");

	/*Get all the tokens seperated by a space*/
	token = strtok(input," ");

	/*Get all tokens seperated by a space*/
	while (token != NULL){
		tokenCount++;

		if (tokens == NULL){
			tokens = malloc(sizeof(char*));
		}
		else{
			tokens = realloc(tokens
				,sizeof(char*)*tokenCount);
		}	
		
		tokens[tokenCount-1] = malloc(sizeof(char)
			*(strlen(token)+1));
		strcpy(tokens[tokenCount-1],token);

		token = strtok(NULL," ");
		if (token != NULL && (token[0] == '|' 
			|| token[0] == '<' || token[0] == '>')){
			break;
		}
	}

	/*Null terminate the array*/
	tokens = realloc(tokens,sizeof(char*)
		*(tokenCount+1));
	tokens[tokenCount] = NULL;

	free(input);
	return tokens;
}
char *getPipeCmd(char *buffer){
	char *token,*input,*command;
	int i;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	token = strtok(input,"|");
	token = strtok(NULL," ");

	/*Remove beginning space if one exists*/
	if (token[0] == ' '){
		for (i=0;i<strlen(token);i++){
			token[i] = token[i+1];
		}
	}
	/*Remove new line char*/
	for (i=0;i<strlen(token);i++){
		if (token[i] == '\n'){
			token[i] = '\0';
		}
	}

	command = malloc(sizeof(char)
		*(strlen(token)+1));
	strcpy(command,token);

	free(input);

	return command;
}
char *getCmd(char *buffer){
	char *token,*input,*command;

	/*Copy the contents of buffer*/
	input = getCopy(buffer);

	/*Remove the new line*/
	token = strtok(input,"\n");	
	token = strtok(token," ");

	command = malloc(sizeof(char)
		*(strlen(token)+1));
	strcpy(command,token);

	free(input);

	return command;
}
