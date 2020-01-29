#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "entities.h"

#define THREADCOUNT 4

void read_write(int fdfile, int fdfifo){
	int n;
	char buffer[50];
	while((n=read(fdfile, buffer, 50))>0){
		write(fdfifo, buffer, n);
	}
}

void *writeFifo(void *argum){
	writeFifoArgs *args = argum;
	char * fifoname = (*args).fifoname;
	int fdfile = (*args).fdfile;
	int fifofd;
	fifofd = open(fifoname, O_WRONLY);
	read_write(fdfile, fifofd);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t threads[THREADCOUNT];
	if(argc!=4){
		printf("Usage: %s <file1> <file2> <file3>\n", argv[0]);
		return -1;
	}
	int fd[3];
	//error processing
	if((fd[0] = open(argv[1], O_RDONLY, 0666)) == -1){
		printf("Error opening file %s\n", argv[1]);
		return 1;
	}
	if((fd[1] = open(argv[2], O_RDONLY, 0666)) == -1){
		printf("Error opening file %s\n", argv[2]);
		return 1;
	}
	if((fd[2] = open(argv[3], O_RDONLY, 0666)) == -1){
		printf("Error opening file %s\n", argv[3]);
		return 1;
	}
	//fifo creator
	mkfifo("fifoin0", 0666);
	mkfifo("fifoin1", 0666);
	mkfifo("fifoin2", 0666);
	mkfifo("fifoin3", 0666);
	//arguments
	writeFifoArgs * args0 = malloc(sizeof(writeFifoArgs));
	(*args0).fdfile = fd[0];
	(*args0).fifoname = "fifoin0";
	writeFifoArgs * args1 = malloc(sizeof(writeFifoArgs));
	(*args1).fdfile = fd[1];
	(*args1).fifoname = "fifoin1";
	writeFifoArgs * args2 = malloc(sizeof(writeFifoArgs));
	(*args2).fdfile = fd[2];
	(*args2).fifoname = "fifoin2";
    writeFifoArgs * args3 = malloc(sizeof(writeFifoArgs));
	(*args3).fdfile = 0;
	(*args3).fifoname = "fifoin3";
	//threads, put it in cycle maybe
	pthread_create(&threads[0], NULL, writeFifo, (void*) args0);
	pthread_create(&threads[1], NULL, writeFifo, (void*) args1);
	pthread_create(&threads[2], NULL, writeFifo, (void*) args2);
    pthread_create(&threads[3], NULL, writeFifo, (void*) args3);
	pthread_exit(NULL);
	exit(0);
}
