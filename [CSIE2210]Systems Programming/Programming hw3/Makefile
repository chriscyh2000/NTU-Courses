all: simulatedThreads.o main.c scheduler.c
	gcc -Wall -o main main.c scheduler.c simulatedThreads.o
simulatedThreads.o: simulatedThreads.c
	gcc -c simulatedThreads.c
