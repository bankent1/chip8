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
	uint8_t kk;

	// 12 bits
	uint16_t nnn;
};

// structure for holding the control bits of the emulator
struct ctrl_bits {
	/* uint8_t vfwrite_enable; */
	/* uint8_t reg_src; */
	/* uint8_t fb_row; */
	/* uint8_t write_reg; */
	/* uint8_t i_src; */
	/* uint8_t mem_src; */
	/* uint8_t fb_write; */
	/* uint8_t mem_write; */
	/* uint8_t sp_src; */
	/* uint8_t sp_write; */
	/* uint8_t pc_src; */
	/* uint8_t alu_src; */
	/* uint8_t delay_hold; */
	/* uint8_t sound_hold; */
	/* uint8_t xpointer; */
	/* uint8_t alu_op; */

	uint8_t vfwrite_enable;

	// 0 - DT (delay timer)
	// 1 - Rand
	// 2 - mem
	// 3 - kk (imm)
	// 4 - key press
	uint8_t reg_src;

	uint8_t fb_row;
	uint8_t write_reg;

	// 0 - I
	// 1 - ALU/NNN
	// 2 - I + 1
	uint8_t i_src;

	// 0 - Binary Coded Value
	// 1 - VX
	uint8_t mem_src;

	uint8_t fb_write;
	uint8_t mem_write;

	// 0 - sp
	// 1 - sp + 1
	// 2 - sp - 1 
	uint8_t sp_src;

	uint8_t sp_write;

	// 0 - stack
	// 1 - ALU
	// 2 - PC + 2
	// 3 - PC + 1
	// 4 - PC
	// 4 - NNN
	uint8_t pc_src;

	// 0 - ST-1
	// 1 - VX
	uint8_t delay_hold;

	// 0 - ST-1
	// 1 - VX
	uint8_t sound_hold;

	uint8_t xpointer;

	// 0 - VY
	// 1 - kk
	// 2 - literal 5
	uint8_t alu_src;

	// 0 - AND
	// 1 - OR
	// 2 - ADD
	// 3 - SUB
	// 4 - XOR
	// 5 - shift right
	// 6 - shift left
	uint8_t alu_op;
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
