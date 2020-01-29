#Copyright (C) 2019 Nuno Penim
#Simple example makefile for use in ctgbot project
#Can be fully adapted for all uses

CC=gcc
FLAGS_COMMON=-lpthread
FLAGS_EXOTIC=-luuid

OBJ1=utilitario1.c
OBJ2=utilitario2.c
OBJ3=utilitario3.c
OBJ4=utilitario4.c

SIM=simulador.c

OBJ1_EXEC=util1
OBJ2_EXEC=util2
OBJ3_EXEC=util3
OBJ4_EXEC=util4

SIM_EXEC=sim

all: utilitario1 utilitario2 simulador utilitario3

utilitario1: 
	$(CC) $(OBJ1) -o $(OBJ1_EXEC) $(FLAGS_COMMON) $(FLAGS_EXOTIC)

utilitario2:
	$(CC) $(OBJ2) -o $(OBJ2_EXEC) $(FLAGS_COMMON)

simulador:
	$(CC) $(SIM) -o $(SIM_EXEC) $(FLAGS_COMMON)

utilitario3:
	$(CC) $(OBJ3) -o $(OBJ3_EXEC) $(FLAGS_COMMON)

utilitario4:
	$(CC) $(OBJ4) -o $(OBJ4_EXEC) $(FLAGS_COMMON)

clean:
	rm -rf *.txt fifo* out* $(OBJ1_EXEC) $(OBJ2_EXEC) $(OBJ3_EXEC) $(SIM_EXEC) $(OBJ4_EXEC)

