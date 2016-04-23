#include "list.h"
#include "simcpu.h"

static bool isEmpty(char *line){
	for (int i=0;i<strlen(line);i++){
		if (line[i] != ' ' && line[i] != '\t'){
			return false;
		}
	}
	return true;
}

static char *getCopy(char *line){
	char *copy = malloc(sizeof(char)
		*(strlen(line)+1));
	strcpy(copy,line);

	return copy;
}

static void setStateEvent(StateEvent *event, int time
	,int threadNum, int processNum, SimState from, SimState to){

	event->time = time;
	event->threadNum = threadNum;
	event->processNum = processNum;
	event->from = from;
	event->to = to;
}

 static void initBurst(Burst *burst){
	burst->burstNum = 0;
	burst->cpuTm = 0;
	burst->IOTm = 0;
}

static void initThread(Thread *thread){
	thread->threadNum =0;
	thread->burstCount=0;
	thread->arrivalTm=0;
	thread->serviceTm=0;
	thread->turnTm=0;
	thread->finishTm=0;
	thread->IOTm=0;
	thread->burst = NULL;
	thread->finished = NULL;
}

static void setThreadFileData(Thread *thread, int threadNum
	, int processNum, int arrivalTm, int burstCount){

	thread->processNum = processNum;
	thread->threadNum = threadNum;
	thread->burstCount = burstCount;
	thread->arrivalTm = arrivalTm;
}

static void setBurst(Burst *burst,int cpuTm, int IOTm
	,int burstNum){

	burst->burstNum = burstNum;
	burst->cpuTm = cpuTm;
	burst->IOTm = IOTm;
}

static void setSimulationData(Simulation *sim, int processCount
	,int sameSwitchTm, int diffSwitchTm){

	sim->processCount = processCount;
	sim->sameSwitchTm = sameSwitchTm;
	sim->diffSwitchTm = diffSwitchTm;
}

static Burst *getBurst(char *line){
	char *token=NULL,*lineCopy;
	int burstNum=0,cpuTm=0,IOTm=0;
	Burst *burst=malloc(sizeof(Burst));

	initBurst(burst);
	lineCopy = getCopy(line);

	/*Get burst number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstNum = strtol(token,NULL,10);

	/*CPU time of burst*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	cpuTm = strtol(token,NULL,10);

	/*IO time of burst*/
	token = strtok(NULL," ");
	/*Check if IO time is present*/
	if (token && !isEmpty(lineCopy)){
		IOTm = strtol(token,NULL,10);		
	}

	/*Store thread properties*/
	setBurst(burst,cpuTm,IOTm,burstNum);
	
	free(lineCopy);
	return burst;
}

static Thread *getThread(char *line, int processNum){
	char *token=NULL,*lineCopy;
	int threadNum, arrivalTm, burstCount;
	Thread *thread=malloc(sizeof(Thread));

	initThread(thread);
	lineCopy = getCopy(line);

	/*Get thread number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	threadNum = strtol(token,NULL,10);

	/*Arrival time of thread*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	arrivalTm = strtol(token,NULL,10);

	/*Get the number of CPU bursts*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	burstCount = strtol(token,NULL,10);

	/*Store thread properties*/
	setThreadFileData(thread,threadNum,processNum
		,arrivalTm,burstCount);
	
	free(lineCopy);
	return thread;
}

static void getProcessInfo(char *line
	, int *processNum, int *threadCount){
	char *token=NULL,*lineCopy=NULL;

	lineCopy = getCopy(line);

	/*Get process number*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	*processNum = strtol(token,NULL,10);

	/*Get number of threads in process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	*threadCount = strtol(token,NULL,10);

	free(lineCopy);
}

static void initSimulation(Simulation *sim){
	sim->processCount = 0;
	sim->sameSwitchTm = 0;
	sim->diffSwitchTm = 0;
	sim->threads = NULL;
}

static Simulation *getSimulation(char *line){
	char *token=NULL,*lineCopy;
	int processCount,sameSwitchTm,diffSwitchTm;
	Simulation *simulation=malloc(sizeof(Simulation));

	initSimulation(simulation);
	lineCopy = getCopy(line);

	/*Get process count*/
	token = strtok(lineCopy," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	processCount = strtol(token,NULL,10);

	/*Get time to switch to thread in same process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	sameSwitchTm = strtol(token,NULL,10);

	/*Get time to switch to thread in different process*/
	token = strtok(NULL," ");
	if (!token){
		fprintf(stderr,"Error: invalid format\n");
		exit(1);
	}
	diffSwitchTm = strtol(token,NULL,10);

	/*Store simulator properties*/
	setSimulationData(simulation,processCount
		,sameSwitchTm,diffSwitchTm);
	
	free(lineCopy);
	return simulation;
}

static char *getLine(){
	char *line=NULL, curChar=' ';
	int charNum=0;

	while (curChar != '\n' && curChar != EOF){
		curChar = fgetc(stdin);
		if (feof(stdin) || curChar == '\n'){ break; }

		/*Allocate memory for line*/
		if (!line){ line = malloc(sizeof(char)*2); }
		else{
			line = realloc(line,sizeof(char)
				*(strlen(line)+2));
		}

		line[charNum] = curChar;
		line[charNum+1] = '\0';
		charNum++;
	}

	return line;
}

static Simulation *fillSimProps(){
	char *line=NULL;
	int burstNum=0,threadCounter=0,processCounter=0;
	int processNum=0;
	bool finished = false;
	Thread *curThread=NULL;
	Burst *curBurst=NULL;
	ParseState state=simulateprops;
	Simulation *sim;

	/*Get all the data from the file and store it
	inside data structures contained inside a 
	Simulation structure*/
	do{
		line = getLine();
		if (!line){ break; }

		/*Finished storing all thread properties*/	
		if (state == burstprops && burstNum == 0){
			if (threadCounter > 0){				
				state = threadprops;
				threadCounter--;
			}
			else if (processCounter > 0){
				state = processprops;
				processCounter--;
			}
		}
		/*Finished storing all thread properties*/	
		else if (state == threadprops && threadCounter == 0){
			if (processCounter > 0){				
				state = processprops;
				processCounter--;
			}
		}
		else if (state == processprops){
			if (processCounter == 0){
				finished = true;
			}
		}

		switch(state){

			case simulateprops:
				sim = getSimulation(line);
				state = processprops;
				processCounter = sim->processCount;

				if (sim->processCount > 0){
					/*Create process list*/
					createList(&(sim->threads));
				}

				break;

			case processprops:

				getProcessInfo(line,&processNum,&threadCounter);
				if (threadCounter > 0){
					state = threadprops;
				}
				break;

			case threadprops:

				curThread = getThread(line,processNum);
				burstNum = curThread->burstCount;
				push(sim->threads,curThread);

				/*Create burst list*/
				if (!curThread->burst){
					createList(&(curThread->burst));
				}
				if (!curThread->finished){
					createList(&(curThread->finished));
				}

				/*Get next bursts properties*/
				if (burstNum > 0){
					state = burstprops;
				}

				break;

			case burstprops:

				/*Get next burst*/
				curBurst = getBurst(line);

				/*Add burst to queue*/
				push(curThread->burst,curBurst);

				burstNum--;
				if (burstNum == 0){
					state = threadprops;	
					threadCounter--;
				}

				break;
		}

		free(line);

	}while (!finished);
 
	return sim;
}

static void freeThreads(void *data){
	Thread *thread = (Thread*)data;

	/*Free threads burst list*/
	if (thread->burst){
		freeHard(&thread->burst,free);
	}
	if (thread->finished){
		freeHard(&thread->finished,free);
	}
	/*Free node data(process)*/
	free(data);
}

static void freeSimulationRes(SimulationRes **sim){
	if ((*sim)->thread){
		freeHard(&((*sim)->thread),freeThreads);
	}
	if ((*sim)->stateEvents){
		freeHard(&((*sim)->stateEvents),free);
	}
	free(*sim);
	*sim = NULL;
}

static void freeSimulation(Simulation **sim){
	if ((*sim)->threads){
		freeHard(&((*sim)->threads),freeThreads);
	}
	free(*sim);
	*sim = NULL;
}

int threadCmp(const void *a,const void *b){
	Thread *thread1 = *((Thread**)a);
	Thread *thread2 = *((Thread**)b);
	return (thread1->readyTime - thread2->readyTime);
}

static void sortThreads(List *threads){
	int threadNum;
	Node *node=NULL;
	Thread **threadarr=NULL;

	threadNum = threads->count;
	threadarr = malloc(sizeof(Thread*)*threadNum);

	/*Add threads from all processes to array*/
	for (int i=1;i<=threadNum;i++){
		node = getNode(threads,i);
		threadarr[i-1] = getData(node);
	}

	/*Empty list to allow for restorage*/
	for (int i=1;i<=threadNum;i++){
		removeSoft(threads,1);
	}
	
	/*Sort the array of thread nodes*/
	qsort(threadarr,threadNum,sizeof(Thread*),threadCmp);

	/*Push sorted thread nodes into queue*/
	for (int i=0;i<threadNum;i++){
		push(threads,threadarr[i]);
	}

	free(threadarr);
}

static void printThreadRes(void *data){
	Thread *thread = (Thread*)data;

	printf("Thread %d of Process %d:\n"
		,thread->threadNum,thread->processNum );
	printf(" arrival time: %d\n",thread->arrivalTm);
	printf(" serivce time: %d units, I/O time: %d units"
		,thread->serviceTm,thread->IOTm);
	printf(", turnaround time: %d units, finish time: %d units\n"
		,thread->turnTm,thread->finishTm);
}

static void printSimState(SimState state){
	switch(state){
		case new:
			printf("new");
			break;
		case ready:
			printf("ready");
			break;
		case running:
			printf("running");
			break;
		case blocked:
			printf("blocked");
			break;
		case terminated:
			printf("terminated");
			break;	
	}
}

static void printStateEvent(void *data){
	StateEvent *event = (StateEvent*)data;

	printf("At time %d: Thread %d of Process %d moves from "
		,event->time,event->threadNum,event->processNum);
	printSimState(event->from);
	printf(" to ");
	printSimState(event->to);
	printf("\n");
}

static void printResults(SimulationRes *res
	, bool detInfo, bool verbMode, int timeQuant){

	if (timeQuant == 0){
		printf("\nFCFS Scheduling\n");
	}
	else{
		printf("Round Robin Scheduling(quantum = %d time units)\n"
			,timeQuant);
	}
	printf("Total Time required is: %d units\n",res->totalTm);
	printf("Average Turnaround Time is %.2f time units\n"
		,res->avgTurnTm);
	printf("CPU Utilization is %.2f%%\n",res->cpuUtil);

	if (detInfo){
		printf("\n");
		printList(*res->thread,printThreadRes);
	}
	if (verbMode){
		printf("\n");
		printList(*res->stateEvents,printStateEvent);
	}
}


static void initSimulationRes(SimulationRes *res){
	res->totalTm = 0;
	res->avgTurnTm = 0;
	res->cpuUtil = 0;
	res->stateEvents = NULL;
	res->thread = NULL;
}

static void updateReadyQueue(List *arrival,List *ready
	, int curTime){
	Thread *curThread;
	int arrivalSize,done[arrival->count+1];

	arrivalSize = arrival->count;

	for (int i=0;i<=arrivalSize;i++){
		done[i] = 0;
	}

	for (int i=1;i<=arrivalSize;i++){
		curThread = getData(getNode(arrival,i));

		if (curThread->arrivalTm == curTime){
			done[i-1] = 1;
		}
	}

	for (int i=1;i<=arrivalSize;i++){
		if (done[i-1]){
			push(ready,getData(getNode(arrival,i)));
			curThread = getData(getNode(arrival,i));
			curThread->readyTime = curTime;
		}
	}
	for (int i=arrivalSize;i>=1;i--){
		if (done[i-1]){
			removeSoft(arrival,i);
		}
	}
}

static void simIO(List *waitingList,List *readyQueue
	,List *finished, List *stateEvents,int curTime){

	int removed[waitingList->count];
	int waitSize;
	Thread *curThread;
	List *curBurstList;
	Burst *curBurst;
	StateEvent *curStateEvent;
	SimState from,to;

	waitSize = waitingList->count;

	/*Initialize all removed positions to false*/
	for (int i=0;i<waitSize;i++){
		removed[i] = 0;
	}

	/*Simulate I/O for each thread on waiting list*/
	for (int i=1;i<=waitSize;i++){

		curThread = getData(getNode(waitingList,i));
		curBurstList = curThread->burst;
		curBurst = getData(getHead(curBurstList));

		/*Remove thread from waiting list*/
		if (curBurst->IOTm == 0){

			/*Remove burst if no cpu time left*/
			if (curBurst->cpuTm == 0){
				push(curThread->finished,pop(curBurstList));

				/*Check if thread is complete*/
				if (curBurstList->count == 0){

					/*Create state event, set, and add to list*/
					curStateEvent = malloc(sizeof(StateEvent));
					from = blocked;
					to = terminated;
					setStateEvent(curStateEvent,curTime
						,curThread->threadNum
						,curThread->processNum,from,to);
					push(stateEvents,curStateEvent);

					push(finished,curThread);
					removed[i-1] = 1;
				}
				/*Add thread to ready queue if it has cpu time left
				or more bursts exists*/
				else{

					/*Create state event, set, and add to list*/
					curStateEvent = malloc(sizeof(StateEvent));
					from = blocked;
					to = ready;
					setStateEvent(curStateEvent,curTime
						,curThread->threadNum
						,curThread->processNum,from,to);
					push(stateEvents,curStateEvent);

					curThread->readyTime = curTime;
					push(readyQueue,curThread);
					removed[i-1] = 1;
				}
			}
		}
		else{
			/*Simulate I/O time*/
			curBurst->IOTm--;
			curThread->IOTm++;
		}

	}

	/*Remove everything that is done*/
	for (int i=1;i<=waitSize;i++){
		if (removed[i-1] == 1){
			curThread = getData(getNode(waitingList,i));
			curThread->finishTm = curTime;
			curThread->turnTm = curTime-curThread->arrivalTm;

			removeSoft(waitingList,i);
		}
	}
}

static SimulationRes *runSim(Simulation *data){
	int curTime=0,prevProcNum=0,curProcNum=0,switchCounter=0;
	int noUtil=0,readySize=0,turnTm=0,curRunTime=0;
	bool newThread = true,contSwitch=false;
	List *waitingList=NULL;
	List *doneIOList=NULL;
	List *curBurstList;
	List *finished;
	List *stateEvents;
	List *arrivalList;
	List *readyQueue;
	Burst *curBurst;
	Thread *curThread,*checkThread,*doneThread;
	SimulationRes *res;
	StateEvent *curStateEvent;
	SimState to,from;

	/*Initialize lists*/
	createList(&waitingList);
	createList(&doneIOList);
	createList(&finished);
	createList(&stateEvents);
	createList(&readyQueue);
	arrivalList = data->threads;


	/*Simulate while threads not done running*/
	while (!empty(readyQueue) || !empty(waitingList)
		|| !empty(arrivalList)){

		/*Check for any arriving threads*/
		updateReadyQueue(arrivalList,readyQueue,curTime);
		
		/*Add events that made it to the ready queue
		from the previous turn. This is being done here
		to prevent a thread from getting an I/O turn
		and a cpu turn in the same time unit*/
		readySize = doneIOList->count;
		for (int i=1;i<=readySize;i++){
			/*Update time thread made it to ready queue*/
			doneThread = pop(doneIOList);
			doneThread->readyTime = curTime;
			push(readyQueue,doneThread);
		}

		/*Rearrange ready list according to arrival time*/
		if (data->type == fcfs){
			sortThreads(readyQueue);
		}

		if (contSwitch){
			checkThread = getData(getHead(readyQueue));
			prevProcNum = checkThread->processNum;

			push(readyQueue,pop(readyQueue));
			checkThread = getData(getHead(readyQueue));

			/*Check type of context switch*/
			if (prevProcNum != checkThread->processNum){
				switchCounter = data->diffSwitchTm;
			}
			else{
				switchCounter = data->sameSwitchTm;
			}
			contSwitch = false;
			newThread = true;
		}

		/*Simulate I/O*/
		simIO(waitingList,doneIOList,finished,stateEvents,curTime);

		/*Simulate events if any exist and no switch is occurring*/
		if (!empty(readyQueue) && switchCounter == 0){

			/*Get top of event queue*/
			curThread = getData(getHead(readyQueue));
			curBurstList = curThread->burst;
			curBurst = getData(getHead(curBurstList));
			
			/*Thread went from ready to running*/
			if (newThread){
				/*Create state event, set, and add to list*/
				curStateEvent = malloc(sizeof(StateEvent));
				from = ready;
				to = running;
				setStateEvent(curStateEvent,curTime
					,curThread->threadNum
					,curThread->processNum,from,to);
				push(stateEvents,curStateEvent);
				newThread = false;
			}
			
			/*Check whether current burst is done but make
			sure no context switch is occurring*/
			if (curBurst->cpuTm == 0){
				/*Check if I/O is needed*/
				if (curBurst->IOTm > 0){
					/*Add current thread waiting list
					and remove from events queue*/

					/*Create state event, set, and add to list*/
					curStateEvent = malloc(sizeof(StateEvent));
					from = running;
					to = blocked;
					setStateEvent(curStateEvent,curTime
						,curThread->threadNum
						,curThread->processNum,from,to);
					push(stateEvents,curStateEvent);

					prevProcNum = curThread->processNum;
					push(waitingList,pop(readyQueue));
					curRunTime = 0;
					newThread = true;

					/*Check if switched same proc or different*/
					if (!empty(readyQueue)){
						checkThread = getTop(readyQueue);
						curProcNum = checkThread->processNum;
						if (curProcNum != prevProcNum){
							switchCounter = data->diffSwitchTm;
						}
						else{
							switchCounter = data->sameSwitchTm;
						}
					}
				}

				/*Remove burst*/
				else{
					push(curThread->finished,pop(curBurstList));
					/*Move to next thread if done*/
					if (curBurstList->count == 0){

						curThread->finishTm = curTime;
						curThread->turnTm = 
							curTime-curThread->arrivalTm;
						prevProcNum = curThread->processNum;
					
						push(finished,pop(readyQueue));
						curRunTime = 0;
						newThread = true;

						/*Check if switched same proc or different*/
						if (!empty(readyQueue)){
							checkThread = getTop(readyQueue);
							curProcNum = checkThread->processNum;
							if (curProcNum != prevProcNum){
								switchCounter = data->diffSwitchTm;
							}
							else{
								switchCounter = data->sameSwitchTm;
							}
						}
					}
				}
			}
			/*If no switch ocurred simulate cpu time*/
			if (switchCounter == 0){
				if (!empty(readyQueue)){
					curRunTime++;
					curBurst->cpuTm--;
					curThread->serviceTm++;

					/*Check for round robin time quantum*/
					if (switchCounter == 0 && data->type == rr && curTime != 0){
						if ((double)data->rrQuantum/(double)curRunTime == 1){
							contSwitch = true;
						}
					}
				}
			}
		}
		
		if (switchCounter > 0){
			noUtil++;
		}
		/*Context switch is occurring*/
		if (switchCounter > 0){
			switchCounter--;
		}

		/*Increase time if not finished*/
		if (!empty(readyQueue) || !empty(waitingList)
			|| !empty(arrivalList)){
			curTime++;
		}	
	}

	/*Store results*/
	res = malloc(sizeof(SimulationRes));
	initSimulationRes(res);

	res->stateEvents = stateEvents;
	res->thread = finished;
    res->totalTm = curTime;
	res->cpuUtil = ((double)(res->totalTm-noUtil)/res->totalTm)*100;
	for (int i=1;i<=res->thread->count;i++){
		curThread = getData(getNode(res->thread,i));
		turnTm += curThread->turnTm;
	}
	res->avgTurnTm = turnTm/res->thread->count;


	/*Free lists*/
	freeSoft(&readyQueue);
	freeSoft(&waitingList);
	freeSoft(&doneIOList);

	return res;
}

int main (int argc, char **argv){
	int timeQuant=0;
	bool detInfo=false,verbMode=false;
	Simulation *sim;
	SimulationRes *res;

	for (int i=0;i<argc;i++){
		if (!strcmp(argv[i],"-d")){
			detInfo = true;
		}
		else if (!strcmp(argv[i],"-v")){
			verbMode = true;
		}
		else if (!strcmp(argv[i],"-r")){
			if (argv[i+1]){
				timeQuant = strtol(argv[i+1],NULL,10);
			}
			else{
				fprintf(stderr,"Error: round robin enabled but time quantum not specified.\n");
				return(EXIT_FAILURE);
			}
		}
	}
	/*Get data ready for simulation*/
	sim = fillSimProps();
	if (timeQuant > 0){
		sim->type = rr;
		sim->rrQuantum = timeQuant;
	}
	else{
		sim->type = fcfs;
	}

	/*Run simulation and print results*/
	res = runSim(sim);
	printResults(res,detInfo,verbMode,timeQuant);

	/*Free memory*/
	freeSimulation(&sim);
	freeSimulationRes(&res);

	return 0;
}