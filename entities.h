#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *airArray[] = {"AMS", "ATL", "DXB", "GRU", "HKG", "ICN", "JNB", "LGW", "MOW", "TLS"};

typedef struct _writeFifoArgs {
	int fdfile;
	char * fifoname;
} writeFifoArgs;

typedef struct _pack {
    char uuid[37];
    char peso[5];
    char airport[4];
} package;
