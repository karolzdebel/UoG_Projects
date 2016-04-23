#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define _BEST_FIT 1
#define _FIRST_FIT 2
#define _NEXT_FIT 3
#define _WORST_FIT 4
#define _MEM_FULL -1
#define _MEM_FREE 0
#define _MEM_TAKEN 1
#define _MEM_SIZE 128
#define _SIZE_LINE 128
#define _ERR0_MSG "Error: invalid number of arguments. Use following syntax: \"./holes [filename]\"\n"
#define _ERR1_MSG "Error: couldn't open file\n"

typedef struct Process{
	char id;
	int size;
	int address;
	int loads;
}Process;

//pid loaded, #processes = 5, #holes = 3, %memusage = 41, cumulative%mem = 40
typedef struct Event{
	char id;
	int processes;
	int holes;
	double memUse;
	double cumMemUse;
}Event;