#pragma once
// chip8Operations.h

#include <stdio.h> 
#include "util.h"

#define MEMORYSIZE 4096

typedef struct chip8 // contains all memory, storage, and other variables for a chip8 system
{
  unsigned char v[16]; // 16 registers
  unsigned short stack[16]; // stack of size 16
  unsigned char sp; // stack pointer
  unsigned short i;
  unsigned short pc;
  unsigned short opcode;
  unsigned char delay;
  unsigned char sound;
  unsigned char input[16];
  unsigned char screen[64][32];
  unsigned char mem[MEMORYSIZE];
  unsigned char draw; // set to 1 if screen needs to be updated
}chip8;

// uses chip8 struct so has to be included after struct is defined
#include "arithmeticOperations.h"

int (*Chip8Table[16])(chip8*);
int cpuNULL(chip8*);
int cpuZero(chip8*);
int cpuJump(chip8*);
int cpuSubroutine(chip8*);
int cpuSkipIfEqual(chip8*);
int cpuSkipIfNotEqual(chip8*);
int cpuSkipIfRegistersEqual(chip8*);
int cpuSkipIfRegistersNotEqual(chip8*);
int cpuSetRegister(chip8*);
int cpuAdd(chip8*);
int cpuArithmetic(chip8*);
int cpuSetIRegister(chip8*);
int cpuJumpPlusRegister(chip8*);
int cpuRand(chip8*);
int cpuDrawSprite(chip8*);
int cpuKeyInput(chip8*);
int cpuF(chip8*);

