#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *airArray[] = {"AMS", "ATL", "DXB", "GRU", "HKG", "ICN", "JNB", "LGW", "MOW", "TLS"};

typedef struct _writeFifoArgs {
	int fdfile;
	char * fifoname;
} writeFifoArgs;

typedef struct _pack {
    char * uuid;
    char * peso;
    char * airport;
} package;

typedef struct _simArgs {
    char * inFifoName;
} simArgs;
