all: main.c stack.c
	gcc -Wall -lncurses -ltinfo main.c stack.c 