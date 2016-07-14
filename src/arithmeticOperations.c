#include "headers/chip8Operations.h"

int (*Chip8Arithmetic[16])(chip8*) = 
{
  arithmeticSet,	arithmeticSetOr,		arithmeticSetAnd,		arithmeticSetXor,
  arithmeticAdd,	arithmeticSubtract,		arithmeticShiftRight,	arithmeticSubtractBackwards, 
  cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, cpuNULL, arithmeticShiftLeft 
};

/********************************************
 * Opcode 8XY0 set v[x] = v[y]              *
 * ******************************************/
int arithmeticSet(chip8* myChip8)
{
	// X = ((*myChip8).opcode & 0x0f00) >> 8
	// Y = ((*myChip8).opcode & 0x00f0) >> 4
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] = 
		(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	return 1;
}

/*****************************************
 * Opcode 8XY1 set v[x] = (v[y] or v[x]) *
 * ***************************************/
int arithmeticSetOr(chip8* myChip8)
{
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] =
		(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] |
		(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	return 1;
}

/******************************************
 * Opcode 8XY2 set v[x] = (v[y] and v[x]) *
 * ****************************************/
int arithmeticSetAnd(chip8* myChip8)
{
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] =
		(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] & 
		(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	return 1;
}

/******************************************
 * Opcode 8XY3 set v[x] = (v[y] xor v[x]) *
 * ****************************************/
int arithmeticSetXor(chip8* myChip8)
{
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] =
		(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] ^ 
		(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	return 1;
}

/**************************************************
 * Opcode 8XY4 v[x] += v[y]                       *
 * Don't think i have carry implemented correctly * 
 * ************************************************/
int arithmeticAdd(chip8* myChip8)
{
	int vx = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	int vy = (*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	
	// largest 8 bit unsigned int is 256
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] = (vx + vy) % 256;

	// set v[0xf] to 1 if there is a carry bit 0 otherwise
	if((vy + vy) > 255 )
		(*myChip8).v[0xf] = 1;
	else
		(*myChip8).v[0xf] = 0;

	return 1;
}

/**************************************************
 * Opcode 8XY5 v[x] -= v[y]                       *
 * ************************************************/
int arithmeticSubtract(chip8* myChip8)
{
	int vx = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	int vy = (*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] =(unsigned)(vx - vy);

	// detect if there is a borrow, if so set carry to 0 else set to 1
	if((vx - vy) < 0)
		(*myChip8).v[0xf] = 0;
	else
		(*myChip8).v[0xf] = 1;

	return 1;
}

/*************************************************************************
 * Opcode 8XY6 Shifts v[x] right by one. v[f] is set to the value of the *
 * least significat bit of v[x] before the shift                         *
 * ***********************************************************************/
int arithmeticShiftRight(chip8* myChip8)
{
	unsigned int vx = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	unsigned int temp = vx;
	int x = 0;	
	int leastSigBit = 0;

	// find least significant bit
	for(x = 7; x >= 0; x--){
		if(temp - twoToThe(x) >= 0){
			leastSigBit = x;
			temp -= twoToThe(x);
		}
	}
	// shift to the right 1	bit
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] = vx >> 1;

	// store the value of the least sig bit in v[f]
	(*myChip8).v[0xf] = twoToThe(leastSigBit);

	return 1;
}

/**********************************
 * Opcode 8XY7 v[x] = v[y] - v[x] *
 * ********************************/
int arithmeticSubtractBackwards(chip8* myChip8)
{
	int vx = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	int vy = (*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4];
	
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] =(unsigned)(vy - vx);

	// detect if there is a borrow, if so set carry to 0 else set to 1
	if((vy - vx) < 0)
		(*myChip8).v[0xf] = 0;
	else
		(*myChip8).v[0xf] = 1;

	return 1;
}

/******************************************************************************
 * Opcode 8XYE Shifts v[x] left one, v[f] is set to the value of the most sig *
 * bit                                                                        *
 * ****************************************************************************/
int arithmeticShiftLeft(chip8* myChip8)
{
	unsigned int vx = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	unsigned int temp = vx;
	int x = 0;	
	int MostSigBit = 0;

	// find most significant bit
	for(x = 7; x >= 0; x--){
		if(temp - twoToThe(x) >= 0){
			MostSigBit = x;
			x = -1;
		}
	}
	// shift to the left 1 bit
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] = vx << 1;

	// store the value of the most sig bit in v[f]
	(*myChip8).v[0xf] = twoToThe(MostSigBit);

	return 1;
}
