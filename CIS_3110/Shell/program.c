
#ifndef __SHELL_H
#define __SHELL_H
#include "shell.h"
#endif

static void freeVars(int outinType,char **cmd,char ***args,
	char **backcmd,char **pipecmd,char ***pipeargs
	,char **redirName){

	if (outinType == __PIPE_OUTPUT){
		free(*pipecmd);
		freeArgs(pipeargs);
	}
	else if (outinType == __REDIR_FFILE
		|| outinType == __REDIR_TFILE){
		free(*redirName);
	}
	if (isBackCmd(*cmd)){
		free(*backcmd);
	}
	free(*cmd);
	freeArgs(args);
}

int main(int argv,char **argc){
	char input[MAX_INPUT],*redirName = NULL;
	char *cmd=NULL,*backcmd=NULL,*pipecmd=NULL;
	char **args=NULL,**pipeargs=NULL,**dirs;
	int status = 0,i=0,outinType;
	Path *path=NULL;

	initializePath(&path);
	/*Get all directories inside path*/
	dirs = getDirs();
	/*Store all directories inside path linked list*/
	while(dirs[i]){
		addDir(path,dirs[i]);
		free(dirs[i]);
		i++;
	}
	free(dirs);

	/*Initialize childpid to a non zero value*/

	while (1){
		printf("> ");
		while(!fgets(input,MAX_INPUT,stdin)){;}
		/*Empty input*/
		if (isEmpty(input) == 0){
			continue;
		}
		
		/*Get command and arguments*/
		cmd = getCmd(input);
		args = getArgs(input);
		outinType = getOutInType(input);

		/*Remove & from cmd*/
		if (isBackCmd(cmd)){
			backcmd = getBackCmd(cmd);
		}
		/*Get the name of which file redirection
		  is occuring to/from*/
		if (outinType == __REDIR_TFILE
				|| outinType == __REDIR_FFILE){
				redirName = getRedirName(input,outinType);
		}
		/*Get the second commands name
		and arguments*/
		else if(outinType == __PIPE_OUTPUT){
			pipecmd = getPipeCmd(input);
			pipeargs = getPipeArgs(input,pipecmd);
		}

		/*Exit*/
		if (strcmp(cmd,"exit") == 0){
			printf("Logout\n\n[Process Completed]\n");

			/*Free all memory*/
			freeVars(outinType,&cmd,&args,&backcmd
				,&pipecmd,&pipeargs,&redirName);
			freePath(&path);
			exit(1);
		}
		/*Run in background asynchronously*/
		else if (isBackCmd(cmd)){
			execBackCmd(backcmd,args,outinType,redirName);
		}
		/*Run two commands by piping first output
		 to the second commands input*/
		else if (outinType == __PIPE_OUTPUT){
			execPipeCmd(cmd,args,pipecmd,pipeargs);
		}
		/*Run the command synchronously*/
		else{
			/*Execute command*/			
			execCmd(cmd,args,outinType,redirName
				,&status);
			if (WEXITSTATUS(status)){
				printf("Abnormal command, return code: %d\n",WEXITSTATUS(status));
			}
		}

		/*Free all memory*/
		freeVars(outinType,&cmd,&args,&backcmd
			,&pipecmd,&pipeargs,&redirName);
	}
	freePath(&path);

	return 0;
}