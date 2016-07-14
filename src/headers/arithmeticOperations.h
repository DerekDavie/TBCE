//arithmeticOperations.h
#pragma once

#include "chip8Operations.h"

int (*Chip8Arithmetic[16])(chip8*);

int arithmeticSet(chip8*);
int arithmeticSetOr(chip8*);
int arithmeticSetAnd(chip8*);
int arithmeticSetXor(chip8*);
int arithmeticAdd(chip8*);
int arithmeticSubtract(chip8*);
int arithmeticShiftRight(chip8*);
int arithmeticSubtractBackwards(chip8*);
int arithmeticShiftLeft(chip8*);


