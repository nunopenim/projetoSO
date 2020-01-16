#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

const char *airArray[] = {"AMS", "ATL", "DXB", "GRU", "HKG", "ICN", "JNB", "LGW", "MOW", "TLS"};

typedef struct _writeFifoArgs {
	int fdfile;
	char * fifoname;
    sem_t *semaf;
} writeFifoArgs;

typedef struct _pack {
    char * uuid;
    char * peso;
    char * airport;
} package;

typedef struct _simArgs {
    char * inFifoName;
    sem_t *semaf;
} simArgs;
