/*
 * chip8.c
 *
 * Author: Travis Banken
 *
 * Main file for the chip 8 processor
 */

#include <stdio.h>
#include <stdint.h>

#include "chip8.h"
#include "phases.h"
#include "dbgutils.h"
#include "chip8-error.h"

// memsize = 4KB
#define MEM_SIZE 4096
#define NUM_REGS 16

#define DATA_START 0x200
#define INSTR_START 0x80

int main(int argc, char *argv[])
{
	printf("Hello, CHIP 8!\n");

	// init memory
	uint8_t mem[MEM_SIZE];
	zero_mem(mem, MEM_SIZE);

	// init reg_file and spec regs
	uint8_t reg_file[NUM_REGS];
	uint16_t I_reg = 0;

	FILE *program = fopen("roms/jason.ch8", "r");
	if (program == NULL) {
		perror("fopen");
		return 1;
	}

	if (load_program(mem, program) != 0) {
		EXIT_ERROR("load_program");
		return 1;
	}

	return 0;
}

void zero_mem(uint8_t *mem, size_t size)
{
	for (int i = 0; i < size; i++) {
		mem[i] = 0;
	}
}

int exec_program(uint8_t *mem, size_t memsize, uint8_t *regfile, size_t regsize)
{
	for (int i = INSTR_START; i < DATA_START; i++) {
		
	}
	
	return 0;
}

int load_program(uint8_t *mem, FILE *bin_prog)
{
	uint8_t *memptr = mem + INSTR_START;
	if (fread(memptr, 1, (DATA_START - INSTR_START), bin_prog) == 0) {
		if (ferror(bin_prog) != 0) {
			PRINT_ERROR("load_program", "Error on fread");
			return 1;
		}
	}

	return 0;
}