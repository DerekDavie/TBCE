CC = gcc

CFLAGS = 

OBJ = main.o executeCycle.o arithmeticOperations.o util.o chip8Operations.o

main: $(OBJ) 
	$(CC) $(OBJ) -lcurses -o chip8Emulator -I. 

main.o: main.c executeCycle.o util.o 
	$(CC) $(CFLAGS) -c main.c

executeCycle.o: executeCycle.c chip8Operations.o util.o
	$(CC) $(CFLAGS) -c executeCycle.c

chip8Operations.o: chip8Operations.c util.o arithmeticOperations.o
	$(CC) $(CFLAGS) -c chip8Operations.c

arithmeticOperations.o: arithmeticOperations.c util.o
	$(CC) $(CFLAGS) -c arithmeticOperations.c

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

clean:
	rm -rf *.o
