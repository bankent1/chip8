/*
 * phases.c
 *
 * Author: Travis Banken
 *
 * Holds the code for the different phases and helper funtions for fetching, 
 * decoding, and executing instructions.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "phases.h"

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
int fetch_instr(uint8_t *mem, size_t size, uint16_t addr, uint16_t *instr)
{
	if (addr >= size || addr + 1 >= size) {
		fprintf(stderr, "fetch_instr(): addr [0x%x] too large\n", addr);
		return 1;
	}

	*instr = (mem[addr] << 8) | mem[addr + 1];
	return 0;
}

/*
 * Decodes the given instruction and fills the given instr struct.
 */
void decode_instr(uint16_t raw_instr, struct instruction *instr)
{
	instr->opcode = (raw_instr >> 12) & 0xf;
	instr->vx     = (raw_instr >>  8) & 0xf;
	instr->vy     = (raw_instr >>  4) & 0xf;
	instr->funct  = (raw_instr >>  0) & 0xf;

	instr->imm8   = (raw_instr >>  0) & 0xff;

	instr->addr   = (raw_instr >>  0) & 0xfff;
}

void fill_ctrl_bits(struct instruction *instr, struct crtl_bits *crtl)
{
	// TODO: Implement me!
}
