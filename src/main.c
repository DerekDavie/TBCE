/*************************************************************************************
 * Author: Derek Davie                                                               *
 * Last Updated: 6-13-16                                                             *
 * CHIP-8 emulator with direction from:                                              *
 * http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/ *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// you must link the curses library for drawing to work gcc -g sourcefile.c -lcurses
#include <curses.h>

#include "headers/chip8.h"


int main(int argc, char** argv)
{ 
	// used to set execution rate to 60hz
	clock_t prevtime;
	clock_t curtime;
	int delay = 0;

	// initialize graphics 
	WINDOW *scrn; // pointer to screen object for ncurse (not sure if required)
	scrn = initscr(); // initalized the screen
	clear(); // clears the screen
	timeout(3); // sets getch() to wait 50ms

	srand(time(NULL)); // need random generator for one of the opcodes implemented 
	
	chip8 myChip8;
	// initalize myChip8
	initializeMyChip(&myChip8);
	initializeScreen(&myChip8); 

	// load program into memory, exit if not loaded correctly
	if(loadProgram(&myChip8, "./ROMS/PONG") == 1){
      return 0;
    }
  
	noecho(); // curses function, turns off echo of keyboard input
	cbreak(); // curses, reports keystrokes without waiting for return key

	// should probably come up with a better end condition but loops until pc == 0
	while(myChip8.pc != 0){
		prevtime = clock(); // stores the time at start of execution cycle

		executeCycle(&myChip8); // execute one cycle

		// Checks if draw flag is set to 1, if not no changes need to be drawn
	    if(myChip8.draw != 0){
			draw(&myChip8);
			myChip8.draw = 0;
		}

	    // gets any changes to input
		getInput(&myChip8);

		// slows down execution to 60hz
		curtime = clock(); // gets clock at end of execution cycle

		// if execution time was greater than 17 ms (1sec/60) than continue if less
		// sleep so delay + execution time is equal to 17 ms	
		if(((curtime * 1000 / CLOCKS_PER_SEC) - (prevtime * 1000 / CLOCKS_PER_SEC)) < 17){
			delay = 17 - ((curtime * 1000 / CLOCKS_PER_SEC) - 
					(prevtime * 1000 / CLOCKS_PER_SEC)); 
			usleep(delay );
		}
	}  
}

/*****************************************************
 * Draws whole screen based on (*chip8).screen array *
 * ***************************************************/
int draw(chip8* myChip8)
{
	int x;
	int y;

	// loop through every x,y postion
	for(y = 0; y < 32; y++){
		for(x = 0; x < 64; x++){
			move(y, x); // move the cursor to position x, y
			delch(); // delect the character currently in this postion
			if((*myChip8).screen[x][y] == 1) // if pixel is set
				insch('#'); // draw a pixel
			else
				insch('-'); // draw a blank
		}	
	}
	move(33, 0);

	refresh(); // after the new screen is drawn, refresh the terminal so it show the changes
	return 1;
}

/**************************
 * Zeros out screen array *
 * ************************/
int initializeScreen(chip8* myChip8)
{
	int x;
	int y;
	// loop through every postion
	for(y = 0; y < 32; y++)
		for(x = 0;x < 64; x++)
			(*myChip8).screen[x][y] = 0; // set position to zero
	return 1;
}

/***************************************
 * Loads program from file filename and stores it in memory starting at 
 * position 0x200
 * ***************************************/
int loadProgram(chip8* myChip8, char* fileName)
{
	long int size;
	int i;
  
	char* buffer;

	if(fileName == NULL){ 
		printf("error no filename passed to loadProgram");
	}

	// open file with passed name
	FILE* fp = fopen(fileName, "rb");

	if(fp == NULL){
		printf("File Error reading, %s", fileName);
		return 1;
    }

	fseek(fp, 0, SEEK_END); // seek to end of file
  
	size = ftell(fp);	// returns how far into the file we are, because we are at end of file this
						// is size of file

	rewind(fp); // rewinds file to point to the begining

	buffer = malloc(sizeof(char) * size); // allocate memory for buffer

	if(buffer == NULL){
		printf("error allocating memory of size %ld\n", size);
		return 1;
    }

	fread(buffer, 1, size, fp); // read in program to buffer

	// transfers program data from the buffer to the userspace in memory starting at 0x200
	for(i = 0; i < size; ++i){
		(*myChip8).mem[0x200 + i] = buffer[i];
    }

	// free memory no longer being used and close open file
	free(buffer);
	fclose(fp);
	return 0;
}

/********************************************************
 * Sets all registers and memory to their inital values *
 * ******************************************************/
int initializeMyChip(chip8*  myChip8)
{

	// chip8 fontset from
	// http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
	unsigned char chip8_fontset[80] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

  // next 3 loops set all of the arrays to 0
  int i;
  for(i = 0; i < 16; ++i){
      (*myChip8).v[i] = 0; 
      (*myChip8).stack[i] = 0;
      (*myChip8).input[i] = 0;
      (*myChip8).mem[i] = 0;
    }
  for(i; i < 2048; ++i){
      (*myChip8).mem[i] = 0;
    }
  for(i; i < 4096; ++i){
      (*myChip8).mem[i] = 0;
    }

  // load the fontset into memory
  for(i = 0; i < 80; ++i){
      (*myChip8).mem[i] = chip8_fontset[i];
    }

  // set pc to 0x200 as this is where the start of the user program will be loaded
  (*myChip8).pc = 0x200;
  
  // set the rest of the variables to 0
  (*myChip8).i = 0;
  (*myChip8).sp = 0;
  (*myChip8).opcode = 0;
  (*myChip8).delay = 0;
  (*myChip8).sound = 0;
  (*myChip8).draw = 0;
}

/********************************************************
 * Runs each cycle and stores any keys that are pressed *
 * ******************************************************/
int getInput(chip8* myChip8)
{
	// gets character pressed and decoded it into it's coresponding position in the input array
	int c = decodeInput(getch());

	// if c == -1 that means we read in a charcter that is not mapped, ignore reading in anything
	// this cycle if it's not -1 go to the position in the input array and set it to 1
	if(c != -1){	
		(*myChip8).input[c] = 1;
	}

	return 1;
}

