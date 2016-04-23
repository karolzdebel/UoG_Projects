#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

typedef enum{
	eating,
	thinking,
}state;

pthread_mutex_t *forkLock;	//Locks for every fork critical section
pthread_mutex_t foodLock;	//Lock for food critical section
int philNum;				//Total number of philosophers
int *food;					//Array of food that every philosopher still has to eat
state *states;				//State of each philosopher

/*Unlock forks and leave them for another 
philosopher to use*/
void leaveForks(int a, int b){
	pthread_mutex_unlock(&(forkLock[a]));
	pthread_mutex_unlock(&(forkLock[b]));
}

/*Take a fork and make sure no other
philosopher can use it*/
void takeForks(int a,int b){
	pthread_mutex_lock(&(forkLock[a]));
	pthread_mutex_lock(&(forkLock[b]));
}

void think(int num){
	printf("Philosopher %d thinking\n",num+1);
}

/*Decrease a particular philosophers food and total*/
void eat(int num){
	pthread_mutex_lock(&foodLock);
	printf("Philosopher %d eating\n",num+1);
	food[num]--;
	pthread_mutex_unlock(&foodLock);
}

void *dine(void *num){
	int phil = *((int*)num);

	while (food[phil] > 0){

		int left = phil+1;
		int right = phil;

		/*Wrap to the other side of array if index is -1*/
		if (left == philNum){
			left = 0;
		} 

		/*Eat if the forks to your left and right are available*/
		if (states[left] == thinking && states[right] == thinking){
			
			/*Print state prior to eating*/
			think(phil);

			states[phil] = eating;
			takeForks(left,right);
			
			/*Print eating state*/
			eat(phil);

			/*Leave forks for another philosopher 
			to use and return to thinking*/
			leaveForks(left,right);

			states[phil] = thinking;

		}
	}

	return NULL;
}

int main(int argc, char **argv){
	int foodIn;
	pthread_t *threads;

	/*Check for valid number of arguments*/
	if (argc != 3){
		fprintf(stderr,"Error, invalid number of arguments\n");
		return EXIT_FAILURE;
	}

	/*Check for valid number input*/
	for (int j=1;j<3;j++){
		for (int i=0;i<strlen(argv[j]);i++){

			if (!isdigit(argv[j][i])){
				fprintf(stderr,"Error, both arguments must be numbers\n");
				return EXIT_FAILURE;
			}
		}
	}

	/*Get input*/
	philNum = strtol(argv[1],NULL,10);
	foodIn = strtol(argv[2],NULL,10);

	/*Make sure number of philosophers is greater than 2*/
	if (philNum <= 2){
		fprintf(stderr,"Number of philosophers must be greater than 2\n");
		return EXIT_FAILURE;
	}

	/*Create lock for every fork and food for every philosopher*/
	forkLock = malloc(sizeof(pthread_mutex_t)*philNum);
	food = malloc(sizeof(int)*philNum);
	states = malloc(sizeof(state)*philNum);

	/*Initialize all mutexes and food number*/
	pthread_mutex_init(&foodLock, NULL);
	for (int i=0;i<philNum;i++){
		pthread_mutex_init(&(forkLock[i]), NULL);
		states[i] = thinking; 
		food[i] = foodIn;
	}

	/*Create threads*/
	threads = malloc(sizeof(pthread_t)*philNum);
	int nums[philNum];
	for (int i=0;i<philNum;i++){
		nums[i] = i;
		pthread_create (&(threads[i]), NULL, dine, (void*)(&(nums[i])));
	}
	for (int i=0;i<philNum;i++){
		pthread_join(threads[i], NULL);
	}


	/*Free memory*/
	free(forkLock);
	free(threads);
	free(states);

	return EXIT_SUCCESS;
}