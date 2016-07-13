// chip8Operations.c

#include "chip8Operations.h"
#include "arithmeticOperations.h"

/*****************************************************************************
 * Error catching operation, called when opcode that does not corespond to a *
 * operation or suboperation is called. Sets pc to 0 and prints error msg    *
 * ***************************************************************************/
int cpuNULL(chip8* myChip8)
{
	printf("ERROR instruction 0x%x at %d not implemented yet\n", 
			(*myChip8).opcode, (*myChip8).pc - 2);
	(*myChip8).pc = 0;
	return -1;
}

/********************************
 * OPcode: 00XX                 *
 * XX = SubOperation to execute *
 * 0xE0: clear screen           *
 * 0xEE: return from subroutine *
 * ******************************/
int cpuZero(chip8* myChip8)
{
	// clear screen
	if((*myChip8).opcode == 0x00E0){
		return 1;
		initializeScreen(myChip8);
	}
	// return from subroutine
	if((*myChip8).opcode == 0x00EE){
		--(*myChip8).sp;
		(*myChip8).pc = (*myChip8).stack[(*myChip8).sp];
		return 1;
	}
	cpuNULL(myChip8);
	return 0;
}

/**************************************
 * Opcode 1NNN - jump to position NNN *
 * sets pc to NNN                     *
 * ************************************/
int cpuJump(chip8* myChip8)
{
	// NNN = (*myChip8).opcoe & 0x0fff
	(*myChip8).pc = (*myChip8).opcode & 0x0fff;
	return 1;
}

/***************************************************************
 * Opcode - 2NNN with NNN being the place in memory to call    *
 * ************************************************************/
int cpuSubroutine(chip8* myChip8)
{
	// NNN = (*myChip8).opcode & 0x0fff
	// store stuff on stack
	(*myChip8).stack[(*myChip8).sp++] = (*myChip8).pc;
	// Set pc to new routine location
	(*myChip8).pc = (*myChip8).opcode & 0x0fff;
	return 1;
}

/********************************************************
 * Opcode 3XNN skips The next instruction if v[x] == NN *
 * ******************************************************/
int cpuSkipIfEqual(chip8* myChip8)
{
	// X = ((*myChip8).opcode & 0x0f00) >> 8
	// NN = (*myChip8).opcode & 0x00ff)
	if((*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] == ((*myChip8).opcode & 0x00ff)){
		(*myChip8).pc += 2;
	}
	return 1;
}

/***************************************************
 * Opcode 4XNN skip next instruction if v[x] != NN *
 * *************************************************/
int cpuSkipIfNotEqual(chip8* myChip8)
{
	if((*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] != ((*myChip8).opcode & 0x00ff)){
		(*myChip8).pc += 2;
	}
	return 1;
}

/**********************************************************
 * Opcode 5XY0 skip the next instruction if v[x] == v[y]  *
 * ********************************************************/
int cpuSkipIfRegistersEqual(chip8* myChip8)
{
	if((*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] ==
		(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4]){
		(*myChip8).pc += 2;
	}
	return 1;
}

/***********************************************
 * OPcode: 0x6XNN - sets a register to a value *
 * X is the register to set                    *
 * NN is the value to set the register to      *
 ***********************************************/
int cpuSetRegister(chip8* myChip8)
{
  // X = (*myChip8).opcode&0x0f00)>>8
  // NN = (*myChip8).opcode&0x00ff)
  // V[] is array of registers
  // V[X] = NN
  (*myChip8).v[((*myChip8).opcode&0x0f00)>>8] = (*myChip8).opcode & 0x00ff;
  return 1;
}

/*************************
 * Opcode 7XNN           *
 * X- register to add to *
 * NN- number to add     *
 * ***********************/
int cpuAdd(chip8* myChip8)
{
	// X = ((*myChip8).opcode & 0x0f00) >> 8
	// NN = (*myChip8).opcode & 0x00ff
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] += (*myChip8).opcode & 0x00ff;
	return 1;
}

/*************************************************************************************
 * Opcode -  8XYK with K being the arithmetic operation to use, X and Y being the two *
 * registers to use in the operation.                                                 *
 * ************************************************************************************/
int cpuArithmetic(chip8* myChip8)
{
	// X = (*myChip8).opcode & 0x0f00 >> 8	
	// Y = (*myChip8).opcode & 0x00f0 >> 4
	// K = (*myCHip8).opcode & 0x000f
	return Chip8Arithmetic[(*myChip8).opcode & 0x000f](myChip8);
}



/******************************************************
 * Opcode 9XY0 Skips next instruction if v[x] != v[y] *
 * ****************************************************/
int cpuSkipIfRegistersNotEqual(chip8* myChip8)
{
	if((*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] !=
			(*myChip8).v[((*myChip8).opcode & 0x00f0) >> 4]){
		(*myChip8).pc += 2;
	}
	return 1;
}

/***********************************************
 * OPcode: 0xANNN - sets register i to a value *
 * NNN is the value to set register i to       *
 ***********************************************/
int cpuSetIRegister(chip8* myChip8)
{
  // NNN = (*myChip8).opcode&0xf000
  (*myChip8).i = ((*myChip8).opcode & 0x0fff);
  return 1;
}

/***************************************
 * OPcode BNNN - sets pc to NNN + v[0] *
 * *************************************/
int cpuJumpPlusRegister(chip8* myChip8)
{
	(*myChip8).pc = ((*myChip8).opcode & 0x0fff) + (*myChip8).v[0];
	return 1;
}

/***************************************************
 * opcode CXNN - sets v[x] to a random number & NN *
 * *************************************************/
int cpuRand(chip8* myChip8)
{
 // X = ((*myChip8).opcode & 0x0f00) >> 8
 // NN = (*myChip8).opcode & 0x00ff
	(*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8] = (rand() % 0xff) & ((*myChip8).opcode & 0x00ff);
	return 1;
}

/*********************************************************************************************
 * OPcode: 0xDXYN - Draw sprite from the memory location stored in register I. If a pixel    *
 * is changed set VF to 1. XOR drawing. start sprites at position VX VY. N is the number of  *
 * 8bit rows that are to be drawn, adding 1 to VY every row.                                 *
 *********************************************************************************************/
int cpuDrawSprite(chip8* myChip8)
{
	// X = ((*myChip8).opcode & 0x0f00) >> 8
	// Y = ((*myChip8).opcode & 0x00f0) >> 4
	// N = (*myChip8).opcode & 0x000f
	int x = ((*myChip8).opcode & 0x0f00) >> 8;
	int y = ((*myChip8).opcode & 0x00f0) >> 4;
	int n = (*myChip8).opcode & 0x000f; 
	int count = 0; // characters printed on this line
	int lineCount = 0; // lines printed
	int i;

	(*myChip8).v[0xf] = 0;

	x = (*myChip8).v[x];
	y = (*myChip8).v[y];
	
	while((n - lineCount) != 0){
		i = (*myChip8).mem[(*myChip8).i + lineCount];
		for(count = 0; count < 8; count++){
			if(i - twoToThe(7 - count) >= 0){
				i -= twoToThe(7 - count);
				if( (*myChip8).screen[(x + count) % 64][(y + lineCount) % 32] == 1){
					(*myChip8).v[0xf] = 1;
					(*myChip8).screen[(x + count) % 64][(y + lineCount) % 32] = 0;
				}
				else{
					(*myChip8).screen[(x + count) % 64][(y + lineCount) % 32] = 1;
					(*myChip8).draw = 1;
				}
			}
		}
		lineCount++;
	}

	return 1;
}

/********************************************
 * opcode EXKK kk selects operation on v[x] *
 * ******************************************/
int cpuKeyInput(chip8* myChip8)
{
	int x = (*myChip8).v[((*myChip8).opcode & 0x0f00) >> 8];
	int k = (*myChip8).opcode & 0x00ff;
	if(k == 0xA1){
		if((*myChip8).input[x] != 1){
			(*myChip8).pc += 2;	
		}
		(*myChip8).input[x] = 0;
		return 1;
	}
	if(k == 0x9E){
		if((*myChip8).input[x] == 1){
			(*myChip8).pc += 2;
			(*myChip8).input[x] = 0;
		}
		return 1;
	}
	cpuNULL(myChip8);
	return 0;
}

/********************************************************
 * Opcode - FXKK with KK being the operation to perform *
 * ******************************************************/
int cpuF (chip8* myChip8)
{
	// X = ((*myChip8).opcode & 0x0f00) >> 8
	int x = ((*myChip8).opcode & 0x0f00) >> 8;
	// KK = (*myChip8).opcode & 0x00ff
	int k = (*myChip8).opcode & 0x00ff;
	
	if(k == 0x07){
		(*myChip8).v[x] = (*myChip8).delay;
		return 1;
	}
	if(k == 0x0A){
		k = getchar();
		(*myChip8).v[x] = decodeInput(k);
		return 1;
	}
	if(k == 0x15){
		(*myChip8).delay = (*myChip8).v[x];
		return 1;
	}
	if(k == 0x18){
		(*myChip8).sound = (*myChip8).v[x];
		return 1;
	}
	if(k == 0x1E){
		(*myChip8).i += (*myChip8).v[x];
		return 1;
	}
	if(k == 0x29){
		if((*myChip8).v[x] * 5 > 80){
			printf("error not a character 0-f\n");
			return 0;
		}
		//printf("adding character %x, from location %x\n", (*myChip8).v[x], (*myChip8).v[x] * 5);
		(*myChip8).i = (*myChip8).v[x] * 5;	
		return 1;
	}
	if(k == 0x33){
		(*myChip8).mem[(*myChip8).i] = (*myChip8).v[x] / 100;
		(*myChip8).mem[(*myChip8).i + 1] = ((*myChip8).v[x] / 10) % 10;
		(*myChip8).mem[(*myChip8).i + 2] = ((*myChip8).v[x] % 100) % 10;
		return 1;
	}
	if(k == 0x55){
		for(k = 0; k <= x; k++){
			(*myChip8).v[(*myChip8).i + k] = (*myChip8).v[k];
		}
	}
	if(k == 0x65){
		for(k = 0; k <= x; k++){
			(*myChip8).v[k] = (*myChip8).mem[(*myChip8).i + k];
			//printf("setting register%x to %x\n", k, (*myChip8).mem[(*myChip8).i + k]);
		}
		return 1;
	}
	
	cpuNULL(myChip8);
	return 0;
}



