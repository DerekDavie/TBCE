// chip8.h

#pragma once

#include "chip8Operations.h"
#include "arithmeticOperations.h"
#include "util.h"
#include "executeCycle.h"

// path to ROM folder relative to main.c 
#define ROMPATH /../ROM

int loadProgram(chip8*, char*);
int initializeScreen();
int initializeMyChip(chip8*);
int draw(chip8*);
int getInput(chip8*);

