#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *airArray[] = {"AMS", "ATL", "DXB", "GRU", "HKG", "ICN", "JNB", "LGW", "MOW", "TLS"};

typedef enum { FALSE, TRUE } boolean;

typedef struct _writeFifoArgs {
	int fdfile;
	char * fifoname;
} writeFifoArgs;

typedef struct _pack {
    char uuid[38];
    char peso[5];
    char airport[4];
} package;

int getAirNumber(char * airCode) {
	if (!strcmp(airCode, "AMS")) {
		return 0;
	}
	else if (!strcmp(airCode, "ATL")) {
		return 1;
	}
	else if (!strcmp(airCode, "DXB")) {
		return 2;
	}
	else if (!strcmp(airCode, "GRU")) {
		return 3;
	}
	else if (!strcmp(airCode, "HKG")) {
		return 4;
	}
	else if (!strcmp(airCode, "ICN")) {
		return 5;
	}
	else if (!strcmp(airCode, "JNB")) {
		return 6;
	}
	else if (!strcmp(airCode, "LGW")) {
		return 7;
	}
	else if (!strcmp(airCode, "MOW")) {
		return 8;
	}
	else if (!strcmp(airCode, "TLS")) {
		return 9;
	}
	return -1;
}
