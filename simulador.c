#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "entities.h"

#define THREADSIN 4
#define QUEUESIZE 8192 //mudar
#define BUFFERSIZE 46

//threading
pthread_t threads_in[THREADSIN];
pthread_mutex_t queueMut;

//Queue operators
package dados[QUEUESIZE];
int dadosPointer = -1;

boolean push(package p) {
	if (dadosPointer+1 >= QUEUESIZE) {
		return FALSE;
	}
	else {
		dadosPointer++;
		dados[dadosPointer] = p;
		return TRUE;
	}
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
		pthread_mutex_lock(&queueMut);
		push(p);
		pthread_mutex_unlock(&queueMut);
	}
}

int main() {
	pthread_create(&threads_in[0], NULL, collector, (void *) 0);
	pthread_create(&threads_in[1], NULL, collector, (void *) 1);
	pthread_create(&threads_in[2], NULL, collector, (void *) 2);
	pthread_create(&threads_in[3], NULL, collector, (void *) 3);
	pthread_exit(NULL);
    return 0;
}
