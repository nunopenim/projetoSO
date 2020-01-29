#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h> 

#include "aux_funcs.h"

#define THREADCOUNT 10
#define BUFFERSIZE 150

//threading
pthread_t threads[THREADCOUNT];

//semaforos
sem_t outputSem;

//globais
const char logName[] = "log.txt";
int logfd;

void *logger(void *_args) {
	int fifoNum = (int) _args;
	char fifoName[9];
	char buffer[BUFFERSIZE];
	sprintf(fifoName, "fifoout%d", fifoNum);
	int fifofd = open(fifoName, O_RDONLY);
	int n;
	while ((n = read(fifofd, buffer, BUFFERSIZE)) > 0){
		sem_wait(&outputSem);
		write(1, buffer, n);
		write(logfd, buffer, n);
		sem_post(&outputSem);
	}
}


int main() {
	sem_init(&outputSem, 0, 1);
	if((logfd = open(logName, O_WRONLY|O_CREAT|O_TRUNC, 0664)) == -1){
		printf("Error opening/creating file %s\n", logName);
		return 1;
	}
	for(int i = 0; i < THREADCOUNT; i++) {
		pthread_create(&threads[i], NULL, logger, (void *) i);
	}
	pthread_exit(NULL);
	exit(0);
}
