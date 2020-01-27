#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

const char *airArray[] = {"AMS", "ATL", "DXB", "GRU", "HKG", "ICN", "JNB", "LGW", "MOW", "TLS"};

int getAir(char str[]) {
	if(!strcmp(str, airArray[0])) {
		return 0;
	}
	if(!strcmp(str, airArray[1])) {
		return 1;
	}
	if(!strcmp(str, airArray[2])) {
		return 2;
	}
	if(!strcmp(str, airArray[3])) {
		return 3;
	}
	if(!strcmp(str, airArray[4])) {
		return 4;
	}
	if(!strcmp(str, airArray[5])) {
		return 5;
	}
	if(!strcmp(str, airArray[6])) {
		return 6;
	}
	if(!strcmp(str, airArray[7])) {
		return 7;
	}
	if(!strcmp(str, airArray[8])) {
		return 8;
	}
	if(!strcmp(str, airArray[9])) {
		return 9;
	}
	return -1;
}

typedef enum { FALSE, TRUE } boolean;

typedef struct _writeFifoArgs {
	int fdfile;
	char * fifoname;
} writeFifoArgs;

typedef struct _pack {
    char uuid[38];
    char peso[5];
    char airport[4];
    time_t entrada;
    time_t saida;
} package;
