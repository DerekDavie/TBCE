// chip8.h

#pragma once

#include "chip8Operations.h"
#include "arithmeticOperations.h"
#include "util.h"
#include "executeCycle.h"

int loadProgram(chip8*, char*);
int initializeScreen();
int initializeMyChip(chip8*);
int draw(chip8*);
int getInput(chip8*);

