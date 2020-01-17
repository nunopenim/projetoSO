#Copyright (C) 2019 Nuno Penim
#Simple example makefile for use in ctgbot project
#Can be fully adapted for all uses

CC=gcc
FLAGS_OBJ1=-lpthread -luuid
FLAGS_OBJ2=-lpthread
FLAGS_OBJ3=-lpthread -luuid
OBJ1=utilitario1.c
OBJ2=utilitario2.c
OBJ3=simulador.c
OBJ1_EXEC=util1
OBJ2_EXEC=util2
OBJ3_EXEC=sim

all: utilitario1 utilitario2

utilitario1: 
	$(CC) $(OBJ1) -o $(OBJ1_EXEC) $(FLAGS_OBJ1)

utilitario2:
	$(CC) $(OBJ2) -o $(OBJ2_EXEC) $(FLAGS_OBJ2)

simulador:
	$(CC) $(OBJ3) -o $(OBJ3_EXEC) $(FLAGS_OBJ3)

clean:
	rm -rf *.txt fifo* out* $(OBJ1_EXEC) $(OBJ2_EXEC) $(OBJ3_EXEC)

