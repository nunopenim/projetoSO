#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void bufferWipe(char * buffer, int size) {
	for(int i = 0; i < size; i++){
		buffer[i] = '\0';
	}
}
