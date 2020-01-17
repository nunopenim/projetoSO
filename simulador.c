#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "entities.h"
#include "aux_funcs.h"

#define THREADSIN 4
#define QUEUESIZE 128
#define BUFFERSIZE 46

package dados[QUEUESIZE];

void *simThread(void *_args) {
	int fd = (int) _args;
	char buffer[BUFFERSIZE];
	char prev_ec[BUFFERSIZE] = ""; //error correction
	while(read(fd, buffer, BUFFERSIZE) > 0) {
		char fullStr[BUFFERSIZE];
		strcpy(fullStr, buffer);
		char * remaining = fullStr;
		char * temp_uuid = strtok_r(remaining, ",", &remaining);
		char uuid[BUFFERSIZE];
		strcpy(uuid, prev_ec);
		strcat(uuid, temp_uuid);
		char * peso = strtok_r(remaining, ",", &remaining);
		char * aeroporto = strtok_r(remaining, "\n", &remaining);
		if (remaining==NULL) {
			strcpy(prev_ec, "");
		}
		else{
			strcpy(prev_ec, remaining);
		}
		printf("%s\n", uuid);
		printf("%s\n", peso);
		printf("%s\n", aeroporto);
		strcpy(buffer, "");
		strcpy(fullStr, "");
	}
}

int main() {
	pthread_t threads_in[THREADSIN];
	int fifoFd[THREADSIN];
	fifoFd[0] = open("fifoin0", O_RDONLY);
	fifoFd[1] = open("fifoin1", O_RDONLY);
	fifoFd[2] = open("fifoin2", O_RDONLY);
	fifoFd[3] = open("fifoin3", O_RDONLY);
	pthread_create(&threads_in[0], NULL, simThread, (void *) fifoFd[0]);
	pthread_exit(NULL);
    return 0;
}
