#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <unistd.h>

#include "entities.h"
//#include "aux_funcs.h"

#define THREADSIN 4
#define THREADSOUT 10
#define QUEUESIZE 1024
#define SHAREDMEM 196
#define BUFFERSIZE 46

//threading
pthread_t threads_in[THREADSIN];
pthread_t threads_out[THREADSOUT];
pthread_t thread_dist;

//memoria partilhada
package * memoriaPartilhada[SHAREDMEM];
int memPointer = 0;
sem_t * memSem;

void cleanMemory() {
	for(int i = 0; i < SHAREDMEM; i++) {
		(*memoriaPartilhada)[i].uuid[0] = '\0';
		(*memoriaPartilhada)[i].peso[0] = '\0';
		(*memoriaPartilhada)[i].airport[0] = '\0';
		(*memoriaPartilhada)[i].entrada = 0;
		(*memoriaPartilhada)[i].saida = 0;
	}
	memPointer = 0;
}

void addToMemory(package p) {
	if(memPointer >= SHAREDMEM) {
		memPointer = 0;
	}
	(*memoriaPartilhada)[memPointer] = p;
	memPointer++;
}

//semaforos
sem_t queueIn;
sem_t o0;
sem_t o1;
sem_t o2;
sem_t o3;
sem_t o4;
sem_t o5;
sem_t o6;
sem_t o7;
sem_t o8;
sem_t o9;

//Queue operators
package dados[QUEUESIZE];
int dadosPointer = -1;
package out0[QUEUESIZE];
package out1[QUEUESIZE];
package out2[QUEUESIZE];
package out3[QUEUESIZE];
package out4[QUEUESIZE];
package out5[QUEUESIZE];
package out6[QUEUESIZE];
package out7[QUEUESIZE];
package out8[QUEUESIZE];
package out9[QUEUESIZE];
int out0P = -1;
int out1P = -1;
int out2P = -1;
int out3P = -1;
int out4P = -1;
int out5P = -1;
int out6P = -1;
int out7P = -1;
int out8P = -1;
int out9P = -1;

boolean push(package p, package dat[], int *pointer) {
	if ((*pointer)+1 >= QUEUESIZE) {
		return FALSE;
	}
	else {
		(*pointer)++;
		dat[(*pointer)] = p;
		return TRUE;
	}
}

void clean(package dat[], int index) {
	dat[index].uuid[0] = '\0';
	dat[index].peso[0] = '\0';
	dat[index].airport[0] = '\0';
}

void reset(package dat[], int *pointer) {
	for(int i = 0; i<QUEUESIZE; i++) {
		clean(dat, i);
	}
	(*pointer) = -1;
}

package pop(package dat[], int *pointer) {
	package p;
	while((*pointer)==-1){ //wait cycle
	}
	if ((*pointer) > -1) {
		p = dat[(*pointer)];
		clean(dat, (*pointer));
		(*pointer)--;
	}
	return p;
}

//Programa
void *collector(void *_args) {
	int fifoNum = (int) _args;
	char stringName[9];
	sprintf(stringName, "fifoin%d", fifoNum);
	FILE* fifo = fopen(stringName, "r");
	char c = fgetc(fifo);
	while(c != EOF) {
		package p;
		int i = 0;
		while(c != ',') {
			p.uuid[i] = c;
			i++;
			c = fgetc(fifo); 
		}
		p.uuid[37] = '\0';
		c = fgetc(fifo);
		i = 0;
		while(c != ',') {
			p.peso[i] = c;
			i++;
			c = fgetc(fifo);
		}
		p.peso[i] = '\0';
		c = fgetc(fifo);
		i = 0;
		while(c != '\0' && c != '\n') {
			p.airport[i] = c;
			i++;
			c = fgetc(fifo);
		}
		p.airport[4] = '\0';
		p.entrada = time(NULL);
		sem_wait(&queueIn);
		push(p, dados, &dadosPointer);
		sem_post(&queueIn);
		usleep(100*1000);
	}
}

void *terminal(void *_args) {
	int air = (int) _args;
	package *array;
	int *pointer;
	sem_t *sem;
	char fifoname[10] = "fifoout";
	int fifofd;
	if (air == 0) {
		array = out0;
		pointer = &out0P;
		sem = &o0;
		strcat(fifoname, "0");
	}
	else if (air == 1) {
		array = out1;
		pointer = &out1P;
		sem = &o1;
		strcat(fifoname, "1");
	}
	else if (air == 2) {
		array = out2;
		pointer = &out2P;
		sem = &o2;
		strcat(fifoname, "2");
	}
	else if (air == 3) {
		array = out3;
		pointer = &out3P;
		sem = &o3;
		strcat(fifoname, "3");
	}
	else if (air == 4) {
		array = out4;
		pointer = &out4P;
		sem = &o4;
		strcat(fifoname, "4");
	}
	else if (air == 5) {
		array = out5;
		pointer = &out5P;
		sem = &o5;
		strcat(fifoname, "5");
	}
	else if (air == 6) {
		array = out6;
		pointer = &out6P;
		sem = &o6;
		strcat(fifoname, "6");
	}
	else if (air == 7) {
		array = out7;
		pointer = &out7P;
		sem = &o7;
		strcat(fifoname, "7");
	}
	else if (air == 8) {
		array = out8;
		pointer = &out8P;
		sem = &o8;
		strcat(fifoname, "8");
	}
	else if (air == 9) {
		array = out9;
		pointer = &out9P;
		sem = &o9;
		strcat(fifoname, "9");
	}
	else {
		pthread_exit(NULL); //rebentar com a thread, poupar um segfault 
	}
	fifofd = open(fifoname, O_WRONLY);
	while(1) {
		while (*pointer > -1) {
			sem_wait(sem);
			package a = pop(array, pointer);
			sem_post(sem);
			if (atoi(a.peso) < 1000) {
				char fifoString[67];
				a.saida = time(NULL);
				fifoString[66] = '\n';
				sprintf(fifoString,"%s,%s,%s,%ld,%ld",a.uuid, a.peso, a.airport, a.entrada, a.saida);
				write(fifofd, fifoString, 67);
			}
			else {
				char fifoString[68];
				a.saida = time(NULL);
				sprintf(fifoString,"%s,%s,%s,%ld,%ld\n",a.uuid, a.peso, a.airport, a.entrada, a.saida);
				write(fifofd, fifoString, 68);
			}
		}
		usleep(10000);
	}
	
}

void *distributor() {
	for (int i = 0; i < THREADSOUT; i++) {
		pthread_create(&threads_out[i], NULL, terminal, (void *) i);
	}
	while(1) {
		while(dadosPointer > -1) {
			sem_wait(&queueIn);
			package a = pop(dados, &dadosPointer);
			sem_post(&queueIn);
			sem_wait(memSem);
			addToMemory(a);
			sem_post(memSem);
			int dest = getAir(a.airport);
			if (dest == -1) {
				continue;
			}
			else if (dest == 0) {
				usleep(100*1000);
				sem_wait(&o0);
				push(a, out0, &out0P);
				sem_post(&o0);
			}
			else if (dest == 1) {
				usleep(100*1000);
				sem_wait(&o1);
				push(a, out1, &out1P);
				sem_post(&o1);
			}
			else if (dest == 2) {
				usleep(100*1000);
				sem_wait(&o2);
				push(a, out2, &out2P);
				sem_post(&o2);
			}
			else if (dest == 3) {
				usleep(100*1000);
				sem_wait(&o3);
				push(a, out3, &out3P);
				sem_post(&o3);
			}
			else if (dest == 4) {
				usleep(100*1000);
				sem_wait(&o4);
				push(a, out4, &out4P);
				sem_post(&o4);
			}
			else if (dest == 5) {
				usleep(100*1000);
				sem_wait(&o5);
				push(a, out5, &out5P);
				sem_post(&o5);
			}
			else if (dest == 6) {
				usleep(100*1000);
				sem_wait(&o6);
				push(a, out6, &out6P);
				sem_post(&o6);
			}
			else if (dest == 7) {
				usleep(100*1000);
				sem_wait(&o7);
				push(a, out7, &out7P);
				sem_post(&o7);
			}
			else if (dest == 8) {
				usleep(100*1000);
				sem_wait(&o8);
				push(a, out8, &out8P);
				sem_post(&o8);
			}
			else if (dest == 9) {
				usleep(100*1000);
				sem_wait(&o9);
				push(a, out9, &out9P);
				sem_post(&o9);
			}
		}
		if (dadosPointer == -1) {
			sleep(2); //cpu save
		}
	}
}

int main() {
	key_t datakey, mutexkey;
	mutexkey = 1234;
	datakey = 4321;
	int shmid, mutid;
	//pthread_mutex_init(mutexMem, NULL);
	if ((shmid = shmget(datakey, SHAREDMEM * sizeof(package), IPC_CREAT | 0666)) < 0){
		perror("shmget");
		return 1;
	}
	if ((mutid = shmget(mutexkey, sizeof(sem_t), IPC_CREAT | 0666)) < 0){
		perror("shmget");
		return 1;
	}
	*memoriaPartilhada = shmat(shmid, NULL, 0);
	memSem = shmat(mutid, NULL, 0);
	cleanMemory();
	//resets data structures
	reset(dados, &dadosPointer);
	reset(out0, &out0P);
	reset(out1, &out1P);
	reset(out2, &out2P);
	reset(out3, &out3P);
	reset(out4, &out4P);
	reset(out5, &out5P);
	reset(out6, &out6P);
	reset(out7, &out7P);
	reset(out8, &out8P);
	reset(out9, &out9P);
	//syncronism
	sleep(1);
	//semaphores
	sem_init(&queueIn, 0, 1);
	sem_init(&o0, 0, 1);
	sem_init(&o1, 0, 1);
	sem_init(&o2, 0, 1);
	sem_init(&o3, 0, 1);
	sem_init(&o4, 0, 1);
	sem_init(&o5, 0, 1);
	sem_init(&o6, 0, 1);
	sem_init(&o7, 0, 1);
	sem_init(&o8, 0, 1);
	sem_init(&o9, 0, 1);
	//fifos
	mkfifo("fifoout0", 0666);
	mkfifo("fifoout1", 0666);
	mkfifo("fifoout2", 0666);
	mkfifo("fifoout3", 0666);
	mkfifo("fifoout4", 0666);
	mkfifo("fifoout5", 0666);
	mkfifo("fifoout6", 0666);
	mkfifo("fifoout7", 0666);
	mkfifo("fifoout8", 0666);
	mkfifo("fifoout9", 0666);
	for (int i = 0; i < THREADSIN; i++) {
		pthread_create(&threads_in[i], NULL, collector, (void *) i);
	}
	pthread_create(&thread_dist, NULL, distributor, NULL);
	pthread_exit(NULL);
	sem_destroy(&queueIn);
    exit(0);
}
