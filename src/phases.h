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
	uint8_t vfwrite_enable;

	// 0 - DT (delay timer)
	// 1 - Rand
	// 2 - mem
	// 3 - kk (imm)
	// 4 - key press
	// 5 - alu_res
	uint8_t reg_src;
	uint8_t write_reg;

	// 0 - I
	// 1 - I + ALU/NNN
	// 2 - I + 1
	uint8_t i_src;

	// 0 - Binary Coded Value
	// 1 - VX
	uint8_t mem_src;
	uint8_t mem_write;

	// frame buf write
	uint8_t fb_write;

	// ???
	uint8_t fb_row;

	// 0 - sp
	// 1 - sp + 1
	// 2 - sp - 1 
	uint8_t sp_src;

	uint8_t sp_write;

	// 0 - PC + 2
	// 1 - PC
	// 2 - PC + 4
	// 3 - NNN
	// 4 - v0 + nnn
	// 5 - stack
	uint8_t pc_src;

	// 0 - ST-1
	// 1 - VX
	uint8_t delay_hold;

	// 0 - ST-1
	// 1 - VX
	uint8_t sound_hold;

	// ???
	uint8_t xpointer;

	// 0 - VY
	// 1 - kk
	// 2 - literal 0 // not used?
	uint8_t alu_src;

	// 0 - AND
	// 1 - OR
	// 2 - ADD
	// 3 - SUB
	// 4 - XOR
	// 5 - shift right
	// 6 - shift left
	uint8_t alu_op;

	uint8_t not_alu_res;
};

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
int fetch_instr(uint8_t *mem, size_t size, uint16_t addr, uint16_t *instr);

/*
 * Decodes the given instruction and fills the given instr struct.
 * Memory for the instruction struct is expected to be alloc before calling.
 */
void decode_instr(uint16_t raw_instr, struct instruction *instr);

/*
 * Fills the initial control bits from the given instr struct. The resulting
 * ctrl bits are stored in the given ctrl_bits structs
 * 
 * Returns 1 on invalid instruction
 */
int fill_ctrl_bits(struct instruction *instr, struct ctrl_bits *ctrl);

/*
 * Sets the value of *alu_in1 to the alu input based on the instr given.
 * 
 * Return 1 if error occured, 0 on success.
 */
int get_aluin1(struct instruction *instr, uint8_t *regfile, size_t regfile_len, 
               uint16_t *alu_in1);

/*
 * Sets the value of *alu_in2 to the alu input based on the ctrl bits and instr
 * 
 * Return 1 on error, 0 on success.
 */
int get_aluin2(struct ctrl_bits *ctrl, struct instruction *instr,
                    uint8_t *regfile, size_t regfile_len, uint16_t *alu_in2);

/*
 * Executes the alu based on the alu op and inputs given. The result will be 
 * stored in *alu_res param and the carry out of the execution will be stored in
 * *carryout param.
 * 
 * Returns 1 if error occured while executing, 0 otherwise.
 */
int exec_alu(uint16_t alu_in1, uint16_t alu_in2, uint16_t *alu_res, 
             uint8_t *carryout, struct ctrl_bits *ctrl);
/*
 * Executes the mem phase of the chip 8 processor.
 *
 * Returns 1 on error, 0 on success. 
 */
int mem_phase(struct ctrl_bits *ctrl, struct instruction *instr, uint8_t *mem, 
              size_t memsize, size_t addr, uint8_t bin_char, uint8_t *regfile, 
              size_t regsize);
/*
 * Executes the write-back phase of the chip 8 processor.
 *
 * Returns 1 on error, 0 on success.
 */
int wbphase(struct ctrl_bits *ctrl, struct instruction *instr, uint8_t *mem,
            size_t addr, size_t memsize, uint8_t *regfile, size_t regsize, 
            uint16_t alu_res, uint8_t randnum);

#endif
