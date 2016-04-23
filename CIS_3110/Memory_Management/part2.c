#include "part2.h"

static void *copyProcess(void *data){
	Process *process = (Process*)data;
	Process *copy = malloc(sizeof(Process));

	copy->id = process->id;
	copy->size = process->size;
	copy->address = process->address;
	copy->loads = process->loads;

	return (void*)copy;
}

static void *copyEvent(void *data){
	Event *event = (Event*)data;
	Event *copy = malloc(sizeof(Event));

	copy->id = event->id;
	copy->processes = event->processes;
	copy->holes = event->holes;
	copy->memUse = event->memUse;
	copy->cumMemUse = event->cumMemUse;

	return (void*)copy;
}

static bool isEmpty(char *line){
	
	for (int i=0;i<strlen(line);i++){
		if (line[i] != ' ' && line[i] != '\t'){
			return false;
		}
	}
	return true;
}

static List *getProcesses(FILE *file){
	char id,*token,line[_SIZE_LINE];
	int size;
	Process *add;
	List *processes;

	createList(&processes);

	while (fgets(line,_SIZE_LINE,file)){

		/*Skip any empty lines*/
		if (isEmpty(line)){ continue; }

		/*Get id*/
		token = strtok(line," ");
		id = token[0];

		/*Get size*/
		token = strtok(NULL," ");
		size = (int)strtol(token,NULL,10);

		/*Store data inside process*/
		add = malloc(sizeof(Process));
		add->id = id;
		add->size = size;
		add->address = 0;
		add->loads = 0;

		push(processes,add,copyProcess);

		/*Free memory since push creates copy*/
		free(add);
	}

	return processes;
}

static int holeCount(int memory[]){
	int count=0;

	for (int i=1;i<_MEM_SIZE;i++){

		/*If gap is found between blocks or block and 
		end of memory than increaes hole counter*/
		if ( (memory[i]==_MEM_TAKEN || i==(_MEM_SIZE-1) )
			&& (memory[i-1]==_MEM_FREE) ){
			count++;
		}
	}
	return count;
}

static double memUsage(int memory[]){
	int count=0;

	for (int i=0;i<_MEM_SIZE;i++){
		if (memory[i]==_MEM_TAKEN){
			count++;
		}
	}

	return (((double)count)/((double)_MEM_SIZE))*100;
}

static int firstNextAdd(int memSize, int memory[]){
	int memCounter=0;

	for (int i=0;i<_MEM_SIZE;i++){
		
		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
		else{ memCounter = 0; }
		/*Return index since address has been found*/
		if (memCounter==memSize){
			return (i+1)-memSize;
		}
	}

	return _MEM_FULL;
}

static int worstAdd(int memSize, int memory[]){
	int max=_MEM_FULL;
	int maxSize=0;
	int memCounter=0;

	/*Find biggest hole*/
	for (int i=0;i<_MEM_SIZE;i++){
		
		/*Check if its a hole*/
		if ( (memCounter > 0 && memory[i] == _MEM_TAKEN)
			|| (memCounter > 0 && i == _MEM_SIZE-1) ){

			/*Check if hole is larger*/
			if (memCounter > maxSize){
				max = i-memCounter;
				maxSize = memCounter;
			}
			memCounter = 0;
		}

		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
	}

	/*Check if the largest hole is 
	large enough for process if not than 
	no hole can fit the process*/
	if (memSize > maxSize){
		return _MEM_FULL;
	}

	return max;
}

static int bestAdd(int memSize, int memory[]){
	int closestAdd=_MEM_FULL;
	int closestSize=_MEM_SIZE+1;
	int memCounter=0;

	/*Find hole size closest to memSize*/
	for (int i=0;i<_MEM_SIZE;i++){
		
		/*Check if its a hole*/
		if ((memory[i] == _MEM_TAKEN && memCounter > 0)
			|| (i==_MEM_SIZE-1 && memCounter > 0)){

			/*Check if hole is closer to size*/
			if (memCounter >= memSize 
				&& memCounter < closestSize){
				/*Store address*/
				closestSize = memCounter;
				closestAdd = i-memCounter;
			}
			memCounter = 0;
		}

		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
	}

	return closestAdd;
}

static int findNextAdd(int start, int memSize, int memory[]){
	int memCounter=0;

	/*Look from starting address till end*/
	for (int i=start;i<_MEM_SIZE;i++){

		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
		else{ memCounter = 0; }
		/*Return index since address has been found*/
		if (memCounter==memSize){
			return (i+1)-memSize;
		}
	}

	/*Loop back to address 0 and proceed 
	till where started*/
	memCounter = 0;
	for (int i=0;i<start;i++){

		/*Look for empty memory*/
		if (memory[i]==_MEM_FREE){
			memCounter++;
		}
		else{ memCounter = 0; }
		/*Return index since address has been found*/
		if (memCounter==memSize){
			return (i+1)-memSize;
		}
	}
	return _MEM_FULL;
}

static void storeMemory(int address,int size, int memory[]){
	for (int i=address;i<address+size;i++){
		memory[i] = 1;
	}
}

static void clearMemory(int address,int size, int memory[]){
	for (int i=address;i<address+size;i++){
		memory[i] = 0;
	}
}

static void createMemory(int memory[]){
	for (int i=0;i<_MEM_SIZE;i++){
		memory[i]=0;
	}
}

static double getCumulativeMem(List *events, double memUse){
	double total = 0;
	Event *event;
	for (int i=1;i<=events->count;i++){
		event = get(events,i,copyEvent);
		total += event->memUse;
		free(event);
	}
	total += memUse;

	return (total/((double)(events->count+1)));
}

static Event *createEvent(Process *process,List *events
		,int memory[], int processCount){

	Event *event = malloc(sizeof(Event));

	event->id = process->id;
	event->processes = processCount;
	event->holes = holeCount(memory);
	event->memUse = memUsage(memory);
	event->cumMemUse 
		= getCumulativeMem(events,memUsage(memory));

	return event;

}

static List *memSim(int memory[], List *waiting, int type){
	List *inMemory,*finished,*events;
	Process *top,*longest,*add;
	Event *event;
	int address=0;

	/*Cannot simulate empty list*/
	if (empty(waiting)){ return NULL; }

	/*Queue of processes in memory*/
	createList(&events);
	createList(&inMemory);
	createList(&finished);
	
	/*While not all processes are finished*/
	do{
		top = getTop(waiting,copyProcess);

		/*Get address based on the type of simulation*/
		switch(type){
			case _FIRST_FIT:
				address = firstNextAdd(top->size,memory);
				break;
			case _BEST_FIT:
				address = bestAdd(top->size,memory);
				break;
			case _NEXT_FIT:
				address = findNextAdd(address,top->size,memory);
				break;
			case _WORST_FIT:
				address = worstAdd(top->size,memory);
				break;
		}

		free(top);

		/*No space for process*/
		if (address == _MEM_FULL){
			/*Reset address to start*/
			address = 0;

			/*Remove longest stored process from memory*/
			longest = (Process*)pop(inMemory);
			clearMemory(longest->address,longest->size,memory);

			/*If 3 loads than remove*/
			if (longest->loads == 3){
				push(finished,longest,copyProcess);
				free(longest);
			}
			/*Otherwise store in waiting list*/
			else{
				push(waiting,longest,copyProcess);	
				free(longest);	
			}
		}
		/*Space found for process*/
		else{

			/*Add head of waiting list to memory*/
			add = (Process*)pop(waiting);
			add->loads++;
			add->address = address;
			storeMemory(add->address,add->size,memory);
			push(inMemory,add,copyProcess);

			/*Store event*/
			event = createEvent(add,events,memory,inMemory->count);
			push(events,event,copyEvent);
			free(event);

			free(add);
		}

	}while(!empty(waiting));

	destroyList(&finished,free);
	destroyList(&inMemory,free);

	return events;
}

static double holesAvg(List *events){
	double total=0;
	Event *event;

	for (int i=1;i<=events->count;i++){
		event = get(events,i,copyEvent);
		total+= event->holes;

		free(event);
	}

	return total/((double)events->count);
}

static double findCumMem(List *events){
	double mem = 0.0;
	Event *event = get(events,events->count,copyEvent); 
	
	mem = event->cumMemUse;
	free(event);
	return mem;
}

static double processesAvg(List *events){
	double total=0;
	Event *event;

	for (int i=1;i<=events->count;i++){
		event = get(events,i,copyEvent);
		total+= event->processes;

		free(event);
	}

	return total/((double)events->count);
}

static void printEvents(List *events){
	Event *event;

	for (int i=1;i<=events->count;i++){
		event = get(events,i,copyEvent);

		printf("%c loaded, #processes = %d"
			", $holes = %d, %%memusage = %.1lf"
			", cumulative%%mem = %.1lf\n"
			,event->id,event->processes
			,event->holes,event->memUse
			,event->cumMemUse);		

		free(event);
	}

	printf("\nTotal loads = %d"
		", average #processes = %.1lf"
		", average #holes = %.1lf"
		", cumulative %%mem = %.1lf\n"
		,events->count,processesAvg(events)
		,holesAvg(events),findCumMem(events));
}

static void simulate(List *processes,int type){
	int memory[_MEM_SIZE];
	List *events,*simProcesses;

	/*Create empty memory*/
	createMemory(memory);

	switch(type){
		case _NEXT_FIT:
			printf("\n\n-----Next Fit Results-----\n");
			break;
		case _BEST_FIT:
			printf("\n\n-----Best Fit Results-----\n");
			break;
		case _FIRST_FIT:
			printf("\n\n-----First Fit Results-----\n");
			break;
		case _WORST_FIT:
			printf("\n\n-----Worst Fit Results-----\n");
			break;
	}

	/*Simulate*/
	simProcesses = copyList(processes,copyProcess);

	events = memSim(memory,simProcesses,type);
	/*Nothing returned, don't proceed*/
	if (!events){
		fprintf(stdout,"Simulation returned no loads/events.\n");
		
		destroyList(&events,free);
		destroyList(&simProcesses,free);
		return;
	}
	
	printEvents(events);
	destroyList(&events,free);
	destroyList(&simProcesses,free);
}

int main(int argc, char **argv){
	FILE *file;
	List *processes;

	if (argc != 2){
		fprintf(stderr,_ERR0_MSG);
		return EXIT_FAILURE;
	}

	file = fopen(argv[1],"r");
	if (!file){
		fprintf(stderr,_ERR1_MSG);
		return EXIT_FAILURE;
	}

	processes = getProcesses(file);
	fclose(file);

	simulate(processes,_FIRST_FIT);
	simulate(processes,_BEST_FIT);
	simulate(processes,_NEXT_FIT);
	simulate(processes,_WORST_FIT);

	destroyList(&processes,free);

	return EXIT_SUCCESS;

}