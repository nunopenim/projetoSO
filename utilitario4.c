#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#include "entities.h"

#define SHAREDMEM 196

//memoria partilhada
package * memoriaPartilhada[SHAREDMEM];
sem_t * memSem;
int main() {
	key_t memkey, mutexkey;
	int memid, mutexid;
	memkey = 4321;
	mutexkey = 1234;
	if ((memid = shmget(memkey, SHAREDMEM * sizeof(package), 0666)) < 0){
		perror("shmget");
		return 1;
	}
	if ((mutexid = shmget(mutexkey, sizeof(sem_t), 0666)) < 0){
		perror("shmget");
		return 1;
	}
	*memoriaPartilhada = shmat(memid, NULL, 0);
	memSem = shmat(mutexid, NULL, 0);
	for (int i = 0; i < SHAREDMEM; i++) {
		if((*memoriaPartilhada)[i].entrada != 0) {
			printf("%s,%ld\n", (*memoriaPartilhada)[i].uuid, (*memoriaPartilhada)[i].entrada);
		}
	}
	return 0;
}
