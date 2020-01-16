#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include "entities.h"
#include <semaphore.h>

#define THREADCOUNT 4

void read_write(int fdfile, int fdfifo, sem_t *sem){
	int n;
	char buffer[50];
	while((n=read(fdfile, buffer, 50))>0){
		write(fdfifo, buffer, n);
        sem_post(sem);
	}
}

void *writeFifo(void *argum){
	writeFifoArgs *args = argum;
	char * fifoname = (*args).fifoname;
	int fdfile = (*args).fdfile;
    sem_t *sem = (*args).semaf;
	int fifofd;
	mkfifo(fifoname, 0666);
	fifofd = open(fifoname, O_WRONLY);
	read_write(fdfile, fifofd, sem);
	pthread_exit(NULL);
}

int main(int argc, char *argv[]){
	pthread_t threads[THREADCOUNT];
    sem_t *semfifo0;
    sem_unlink("/sem_fifo0");
    semfifo0 = sem_open("/sem_fifo0", O_CREAT, 0666, 0);
    sem_t *semfifo1;
    sem_unlink("/sem_fifo1");
    semfifo0 = sem_open("/sem_fifo1", O_CREAT, 0666, 0);
    sem_t *semfifo2;
    sem_unlink("/sem_fifo2");
    semfifo0 = sem_open("/sem_fifo2", O_CREAT, 0666, 0);
    sem_t *semfifo3;
    sem_unlink("/sem_fifo3");
    semfifo0 = sem_open("/sem_fifo3", O_CREAT, 0666, 0);
	if(argc!=4){
		printf("Usage: %s <file1> <file2> <file3>\n", argv[0]);
		return -1;
	}
	int fd[3];
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
	writeFifoArgs * args0 = malloc(sizeof(writeFifoArgs));
	(*args0).fdfile = fd[0];
	(*args0).fifoname = "fifo0";
    (*args0).semaf = semfifo0;
	writeFifoArgs * args1 = malloc(sizeof(writeFifoArgs));
	(*args1).fdfile = fd[1];
	(*args1).fifoname = "fifo1";
    (*args1).semaf = semfifo1;
	writeFifoArgs * args2 = malloc(sizeof(writeFifoArgs));
	(*args2).fdfile = fd[2];
	(*args2).fifoname = "fifo2";
    (*args2).semaf = semfifo2;
    writeFifoArgs * args3 = malloc(sizeof(writeFifoArgs));
	(*args3).fdfile = 0;
	(*args3).fifoname = "fifo3";
    (*args3).semaf = semfifo3;
	pthread_create(&threads[0], NULL, writeFifo, (void*) args0);
	pthread_create(&threads[1], NULL, writeFifo, (void*) args1);
	pthread_create(&threads[2], NULL, writeFifo, (void*) args2);
    pthread_create(&threads[3], NULL, writeFifo, (void*) args3);
	pthread_exit(NULL);
	exit(0);
}
