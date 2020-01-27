#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#include "entities.h"

#define THREADSIN 4
#define THREADSOUT 10
#define QUEUESIZE 4096 //mudar
#define BUFFERSIZE 46

//threading
pthread_t threads_in[THREADSIN];
pthread_t threads_out[THREADSOUT];
pthread_t thread_dist;

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
	for(int i = 0; i<(sizeof(*dat)/sizeof(package)); i++) {
		clean(dat, i);
	}
	(*pointer) = 0;
}

package pop(package dat[], int *pointer) {
	package p;
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
		sleep(1);
	}
}

void *terminal(void *_args) {
	int air = (int) _args;
	package *array;
	int *pointer;
	sem_t *sem;
	if (air == 0) {
		array = out0;
		pointer = &out0P;
		sem = &o0;
	}
	else if (air == 1) {
		array = out1;
		pointer = &out1P;
		sem = &o1;
	}
	else if (air == 2) {
		array = out2;
		pointer = &out2P;
		sem = &o2;
	}
	else if (air == 3) {
		array = out3;
		pointer = &out3P;
		sem = &o3;
	}
	else if (air == 4) {
		array = out4;
		pointer = &out4P;
		sem = &o4;
	}
	else if (air == 5) {
		array = out5;
		pointer = &out5P;
		sem = &o5;
	}
	else if (air == 6) {
		array = out6;
		pointer = &out6P;
		sem = &o6;
	}
	else if (air == 7) {
		array = out7;
		pointer = &out7P;
		sem = &o7;
	}
	else if (air == 8) {
		array = out8;
		pointer = &out8P;
		sem = &o8;
	}
	else if (air == 9) {
		array = out9;
		pointer = &out9P;
		sem = &o9;
	}
	else {
		pthread_exit(NULL);
		return; //rebentar com a thread, poupar um segfault
	}
	while(1) {
		while (*pointer > -1) {
			package a = pop(array, pointer);
			char fifoString[50];
			a.saida = time(NULL);
			sprintf(fifoString,"%s,%s,%s,%ld,%ld",a.uuid, a.peso, a.airport, a.entrada, a.saida);
		}
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
			int dest = getAir(a.airport);
			if (dest == -1) {
				continue;
			}
			
		}
	}
}

int main() {
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
	sleep(1); //syncronism;
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
	for (int i = 0; i < THREADSIN; i++) {
		pthread_create(&threads_in[i], NULL, collector, (void *) i);
	}
	pthread_create(&thread_dist, NULL, distributor, NULL);
	pthread_exit(NULL);
	sem_destroy(&queueIn);
    exit(0);
}
