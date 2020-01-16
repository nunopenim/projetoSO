#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

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
    sem_t *semaphore = (*argum).semaf;
    int leitura = read(fd, buffer, 45);
    while (leitura > 0){
        char newStr[45];
        strcpy(newStr, buffer);
        char *token;
        char * rest = newStr;
        package * p = malloc(sizeof(package));
        token = strtok_r(rest, ",", &rest); 
        (*p).uuid = token;
        token = strtok_r(rest, ",", &rest);
        (*p).peso = token;
        token = strtok_r(rest, ",", &rest); 
        (*p).airport = token;
        dados[dadosPointer] = p;
        dadosPointer++;
        for(int i = 0; i<45; i++) {
            buffer[i]='\0';
        }
        printf("%s\n", (*p).peso);
        sem_wait(semaphore);
        leitura = read(fd, buffer, 45);
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
    (*args).semaf = sem_open("/sem_fifo0", 0666, 0);
    *simThread((void*) args);
    return 0;
}
