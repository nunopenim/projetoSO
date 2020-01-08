#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "entities.h"
#include "aux_funcs.h"

#define THREADS 4

package *dados[128];
int dadosPointer = 0;

void *simThread(void *args) {
    simArgs *argum = args;
    char * fifo = (*argum).inFifoName;
    int fd = open(fifo, O_RDONLY);
    char buffer[45];
    while (dadosPointer<128){
        char *b1 = strtok(buffer, ",");
        char *b2 = strtok(NULL, ",");
        char *b3 = buffer;
        strcat(b1, "\0");
        strcat(b2, "\0");
        strcat(b3, "\0");
        package * p = malloc(sizeof(package));
        (*p).uuid = b1;
        (*p).peso = b2;
        (*p).airport = b3;
        dados[dadosPointer] = p;
        dadosPointer++;
    }
}

int main() {
    /*for (int i=0; i<10; i++){
        char something[5] = "out";
        char buff[2];
        sprintf(buff, "%d", i);
        strcat(something, buff);
        mkfifo(something, 0666);
    }*/
    simArgs * args = malloc(sizeof(simArgs));
    char name[] = "fifo0"; 
    (*args).inFifoName = name;
    *simThread((void*) args);
    
    printf("%s\n", );
    return 0;
}
