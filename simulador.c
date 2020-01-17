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
    return 0;
}
