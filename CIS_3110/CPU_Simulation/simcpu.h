#ifndef SIMCPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "list.h"

#define _FCFS 0				//First come first serve
#define _RR 1				//Round robin

typedef enum SimType{
	fcfs,
	rr
} SimType;
/*State during parsing*/
typedef enum ParseState{
	simulateprops,			//Parsing simulation props
	processprops,			//Parsing process props
	threadprops,			//Parsing thread props
	burstprops				//Parsing burst props
} ParseState;

/*State during simulation*/
typedef enum SimState{
	new,					//New process
	ready,					//Ready process
	running,				//Running process
	blocked,				//Blocked process
	terminated				//Terminated process
} SimState;

/*Event structure*/
typedef struct StateEvent {
	int time;				//Time an event occurred
	int threadNum;			//Thread number
	int processNum;			//Process number
	SimState from;			//State switched from
	SimState to;			//State switched to
} StateEvent;

/*Burst structure*/
typedef struct BurstData {
	int burstNum;			//Burst number
	int cpuTm;				//Number of CPU time
	int IOTm;				//Number of IO time
} Burst;

/*Thread data structure*/
typedef struct ThreadData {
	int threadNum;			//Thread number
	int processNum;			//Associated process number
	int burstCount;			//number of CPU bursts
	int arrivalTm;			//Arrival time
	int serviceTm;			//Serivce time
	int turnTm;				//Turnaround time
	int finishTm;			//Finish time
	int IOTm;				//IO Time used
	int readyTime;			//Time entered ready queue
	List *finished;			//List of finished 
	List *burst; 			//Queue of bursts
} Thread;

/*Simulation results*/
typedef struct SimulationRes {
	int totalTm;			//Total time
	double avgTurnTm;		//Average turnaround time
	double cpuUtil;			//CPU utilization
	List *thread;			//Thread results
	List *stateEvents;		//State changes that occured
} SimulationRes;

/*Simulate structure*/
typedef struct SimulationData {
	SimType type;			//Type of simulation
	int rrQuantum;			//Round robin time quantum
	int sameSwitchTm;		//Thread time switch in same proc
	int diffSwitchTm;		//Thread time switch in diff proc
	int processCount;		//Process count
	List *threads;			//Queue of threads
} Simulation;

#define SIMCPU_H
#endif