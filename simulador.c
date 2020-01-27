#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>

#include "entities.h"

#define THREADSIN 4
#define QUEUESIZE 4096 //mudar
#define BUFFERSIZE 46

//threading
pthread_t threads_in[THREADSIN];
pthread_mutex_t queueMut;

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
		pthread_mutex_lock(&queueMut);
		push(p, dados, &dadosPointer);
		pthread_mutex_unlock(&queueMut);
	}
}

int main() {
	reset(dados, &dadosPointer);
	for (int i = 0; i < THREADSIN; i++) {
		pthread_create(&threads_in[i], NULL, collector, (void *) i);
	}
	pthread_exit(NULL);
    exit(0);
}
