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
	sprintf(fifoName, "fifoin%d", fifoNum);
	FILE* fifo = fopen(fifoName, "r");
	char c = fgetc(fifo);
	while(c != EOF) {
		int i = 0;
		bufferWipe(buffer, BUFFERSIZE);
		while(c != '\0' || c != '\n') {
			buffer[i] = c;
		}
		sem_wait(&outputSem);
		printf("%s", buffer);
		write(logfd, buffer, strlen(buffer));
		sem_post(&outputSem);
	}
}


int main() {
	if((logfd = open(logName, O_WRONLY|O_CREAT|O_TRUNC, 0664)) == -1){
		printf("Error opening/creating file %s\n", logName);
		return 1;
	}
	sem_init(&outputSem, 0, 1);
	for(int i = 0; i < THREADCOUNT; i++) {
		pthread_create(&threads[i], NULL, logger, (void *) i);
	}
	pthread_exit(NULL);
	exit(0);
}
