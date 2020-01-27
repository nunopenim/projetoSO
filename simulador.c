#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

#include "entities.h"

#define THREADSIN 4
#define QUEUESIZE 4096 //mudar
#define BUFFERSIZE 46

//threading
pthread_t threads_in[THREADSIN];
sem_t queueIn;

//Queue operators
package dados[QUEUESIZE];
int dadosPointer = -1;

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
		printf("%s %s %s %ld\n", dados[dadosPointer].uuid, dados[dadosPointer].peso, dados[dadosPointer].airport, dados[dadosPointer].entrada);
		sem_post(&queueIn);
		sleep(0.5); //500ms
	}
}

int main() {
	reset(dados, &dadosPointer);
	sem_init(&queueIn, 0, 1);
	for (int i = 0; i < THREADSIN; i++) {
		pthread_create(&threads_in[i], NULL, collector, (void *) i);
	}
	pthread_exit(NULL);
	sem_destroy(&queueIn);
    exit(0);
}
