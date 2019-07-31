#include <stdint.h>

#ifndef PHASES_H
#define PHASES_H

// helper struct for holding the different data from the component
struct instruction {
	// 4 bits
	uint8_t opcode;
	uint8_t vx;
	uint8_t vy;
	uint8_t funct;
	
	// 8 bits
	uint8_t imm8;

	// 12 bits
	uint16_t addr;
};

// structure for holding the control bits of the emulator
struct ctrl_bits {
	uint8_t alu_op;
	uint8_t bnegate;

	// TODO: Rest of ctrl bits
};

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
uint8_t fetch_instr(uint8_t *mem, size_t size, uint16_t addr);

/*
 * Decodes the given instruction and fills the given instr struct.
 * Memory for the instruction struct is expected to be alloc before calling.
 */
void decode_instr(uint16_t raw_instr, struct instruction *instr);

#endif
