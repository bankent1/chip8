#include <stdio.h>
#include <stdint.h>

#include "phases.h"

#define FOUR_KB 4096
#define MEM_SIZE FOUR_KB
#define NUM_REGS 16

void zero_mem(uint8_t *mem, size_t size);

int main(int argc, char *argv[])
{
	printf("Hello, CHIP 8!\n");

	// init memory
	uint8_t mem[MEM_SIZE];
	zero_mem(mem, MEM_SIZE);

	// init reg_file and spec regs
	uint8_t reg_file[NUM_REGS];
	uint16_t I_reg = 0;
	// TODO: Delay and sound registers
	
	// TODO: Fetch Instruction
	// TODO: Decode Instruction
	// TODO: Execute Instruction

	return 0;
}

void zero_mem(uint8_t *mem, size_t size)
{
	for (int i = 0; i < size; i++) {
		mem[i] = 0;
	}
}
