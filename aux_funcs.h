#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//fills string with end of line chars
void bufferWipe(char * buffer, int size) {
	for(int i = 0; i < size; i++){
		buffer[i] = '\0';
	}
}

//multiple line breaks
void bufferLineBreaker(char * buffer, int size) {
	for(int i = 0; i < size; i++){
		buffer[i] = '\n';
	}
}
