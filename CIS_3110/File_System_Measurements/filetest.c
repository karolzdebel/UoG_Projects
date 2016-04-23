#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _NANO 1000000000
#define _MICRO 1000000
#define _MILI 1000

#define _GBS 1000000000
#define _MBS 1000000
#define _KBS 1000
#define _BS 1

#define _ERR "Please run program using one of the following commands: \"./filetest fetch\" or \"./filetest block\"\n"

#define LONG 2814585000

static FILE *createFile(long int size, char *name){
	int i;
	FILE *file = fopen(name,"w+");

	for (i=0;i<size;i++){
		fputc(' ',file);
	}
	
	fseek(file,0,SEEK_SET);
	
	return file;
}

static double testBlockTime(long int size){
	char *buffer = malloc(sizeof(char)*size);
	double total,dubStart,dubEnd;
	FILE *fp;
	struct timeval start, end;

	fp = createFile(size,"test.tmp");
	if (!fp){
		fprintf(stderr,"Error when creating file.\n");
		exit(1);
	}

	gettimeofday(&start, NULL);
	fread(buffer, sizeof(char), size, fp);
	gettimeofday(&end, NULL);
    
    dubStart = start.tv_sec + (double)start.tv_usec/1000000;
    dubEnd = end.tv_sec + (double)end.tv_usec/1000000;
    total = dubEnd - dubStart;

	free(buffer);
	fclose(fp);
	remove("test.tmp");

	return total;
}

static void blockTest(){
	FILE *fp;
	long int i,j;
	double test,testAvg=0;
	
	printf("Processing small scale results\n");
	
	/*Small scale testing*/	
	fp = fopen("smallresults.txt","w+");
	for (i=1;i<=_KBS*4+1;i++){
		for (j=0;j<100;j++){
			testAvg += testBlockTime(i);		
		}
		test = testAvg/100;
		testAvg = 0;
		fprintf(fp,"%ld, %lf\n",i,test*_MICRO);
	}	
	fclose(fp);

	printf("\nProcessing medium scale results\n");
	/*Medium scale testing*/
	fp = fopen("mediumresults.txt","w+");
	for (i=_KBS;i<=_KBS*12;i+=_KBS){
		for (j=0;j<100;j++){
			testAvg += testBlockTime(i);
		}
		test = testAvg/100;
		testAvg = 0;
		fprintf(fp,"%ld, %lf\n",i/_KBS,test*_MICRO);
	}	

	fclose(fp);
	
	printf("\nProcessing large scale results\n");
	/*Large scale testing*/
	fp = fopen("largeresults.txt","w+");
	for (i=_MBS;i<=_MBS*100+1;i+=_MBS*10){
		test = testBlockTime(i);	
		fprintf(fp,"%ld, %lf\n",i/_MBS,test*_MILI);	
	}

	fclose(fp);

	printf("\nResults successfully written\n");
}

static double testFetchTime(int sizeRead, int sizeFile){
	char *buffer = malloc(sizeof(char)*sizeFile);
	double total,dubStart,dubEnd;
	FILE *fp;
	struct timeval start, end;


	fp = createFile(sizeFile,"test.tmp");

	gettimeofday(&start, NULL);
	fread(buffer, sizeof(char), sizeRead, fp);
	gettimeofday(&end, NULL);
    
    dubStart = start.tv_sec + (double)start.tv_usec/1000000;
    dubEnd = end.tv_sec + (double)end.tv_usec/1000000;
    total = dubEnd - dubStart;

    fclose(fp);
    remove("test.tmp");
	free(buffer);

	return total;
}

static void fetchTest(){
	
	int i,j;
	double totalAvg,total;
	FILE *fp = fopen("fetchresults.txt","w+");

	printf("Processing data.\n");
	for (i=0;i<_MBS;i+=_KBS){
		for (j=0;j<100;j++){
			totalAvg += testFetchTime(i,_KBS);
		}
		total = totalAvg/100;
		totalAvg = 0;
		fprintf(fp,"%d,%.3lf\n",i/_KBS,total*_MICRO);
	}

	printf("Data written to fetchresults.txt successfully\n");

	fclose(fp);
}

int main(int argc, char **argv){

	/*Invalid arguments*/
	if (argc < 2){
		fprintf(stderr,_ERR);
		return(EXIT_FAILURE);
	}

	/*Produce fetch results*/
	if (!strcmp(argv[1],"fetch")){
		fetchTest();
	}

	/*Produce block results*/
	else if (!strcmp(argv[1],"block")){
		blockTest();
	}

	/*Invalid arguments*/
	else{
		fprintf(stderr,_ERR);
		return(EXIT_FAILURE);
	}

	return EXIT_SUCCESS;
}