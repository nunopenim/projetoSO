#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "entities.h"
#include "uuid_gen.h"

#define STRSIZE 45

void write_file(int fd){
	int peso = (rand() % (2400 - 240 + 1)) + 240;
	int airport = rand()%10;
	if(peso<1000){
		char str[STRSIZE];
		sprintf(str, "%s,%d,%s\n", getuuid(), peso, airArray[airport]);
		write(fd, str, STRSIZE);
	}
	else{
		char str[STRSIZE+1];
		sprintf(str, "%s,%d,%s\n", getuuid(), peso, airArray[airport]);
		write(fd, str, STRSIZE+1);
	}
	
}

int main(int argc, char *argv[]){
	time_t t;
	srand((unsigned) time(&t));
	if (argc !=3){
		printf("Usage: %s <file_name> <n_lines>\n", argv[0]);
		return 1;
	}
	int fd;
	if((fd = open(argv[1], O_WRONLY|O_CREAT|O_TRUNC, 0664)) == -1){
		printf("Error opening/creating file %s\n", argv[1]);
		return 1;
	}
	int n_lines = atoi(argv[2]);
	for(int i=0; i<n_lines; i++){
		write_file(fd);
	}
	close(fd);
	return 0;
}
