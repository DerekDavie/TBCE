CC = gcc

CFLAGS = 

SRCPATH = ./src/

HEADERPATH = $(SRCPATH)headers/

OBJ = main.o executeCycle.o arithmeticOperations.o util.o chip8Operations.o

main: $(OBJ) 
	$(CC) $(OBJ) -lcurses -o chip8Emulator -I. 

main.o: $(SRCPATH)main.c executeCycle.o util.o 
	$(CC) $(CFLAGS) -c $(SRCPATH)main.c

executeCycle.o: $(SRCPATH)executeCycle.c chip8Operations.o util.o
	$(CC) $(CFLAGS) -c $(SRCPATH)executeCycle.c

chip8Operations.o: $(SRCPATH)chip8Operations.c util.o arithmeticOperations.o
	$(CC) $(CFLAGS) -c $(SRCPATH)chip8Operations.c

arithmeticOperations.o: $(SRCPATH)arithmeticOperations.c util.o
	$(CC) $(CFLAGS) -c $(SRCPATH)arithmeticOperations.c

util.o: $(SRCPATH)util.c
	$(CC) $(CFLAGS) -c $(SRCPATH)util.c

clean:
	rm -rf *.o
