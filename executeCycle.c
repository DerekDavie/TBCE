#include "chip8Operations.h"

int (*Chip8Table[16])(chip8*) =
{
  cpuZero,				cpuJump,					cpuSubroutine,		cpuSkipIfEqual, 
  cpuSkipIfNotEqual,	cpuSkipIfRegistersEqual,	cpuSetRegister,		cpuAdd,
  cpuArithmetic,		cpuSkipIfRegistersNotEqual,	cpuSetIRegister,	cpuJumpPlusRegister, 
  cpuRand,				cpuDrawSprite,				cpuKeyInput,		cpuF,
};

/********************************************************************************
 * Executes one cyle while either incrementing pc or changing if the operation  *
 * requrires so                                                                 *
 * ******************************************************************************/
int executeCycle(chip8* myChip8)
{
	// Each opcode is broken up into 8 bits and stored in seperate memory location
	// this combines them into one variable opcode
	(*myChip8).opcode = (*myChip8).mem[(*myChip8).pc + 1] |
						((*myChip8).mem[(*myChip8).pc] << 8);

	// incrment pc, we do this by 2 because each opcode takes up 2 memory location
	(*myChip8).pc += 2;

	// decrement dellay and sound each cycle
	(*myChip8).delay--;
	(*myChip8).sound--;	
  
	// uses an array of funciton pointers to call the correct operation based on the fire
	// 4 bits of the opcode
	Chip8Table[((*myChip8).opcode&0xf000)>>12](myChip8);

}




