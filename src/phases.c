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

	instr->kk     = (raw_instr >>  0) & 0xff;

	instr->nnn    = (raw_instr >>  0) & 0xfff;
}

void fill_ctrl_bits(struct instruction *instr, struct ctrl_bits *ctrl)
{
	// zero out struct
	ctrl->vfwrite_enable = 0;
	ctrl->reg_src = 0;
	ctrl->write_reg = 0;
	ctrl->i_src = 0;
	ctrl->mem_src = 0;
	ctrl->fb_write = 0;
	ctrl->mem_write = 0;
	ctrl->sp_src = 0;
	ctrl->sp_write = 0;
	ctrl->pc_src = 0;
	ctrl->alu_src = 0;
	ctrl->delay_hold = 0;
	ctrl->sound_hold = 0;
	ctrl->xpointer = 0;
	ctrl->alu_op = 0;

	switch (instr->opcode) {
	case 0x0:
		// TODO
		switch (instr->kk) {
		case 0xE0:
			// TODO
			break;
		case 0xEE:
			// TODO:
			break;
		}
		break;
	case 0x1:
		// TODO
		break;
	case 0x2:
		// TODO
		break;
	case 0x3:
		// TODO
		break;
	case 0x4:
		// TODO
		break;
	case 0x5:
		// TODO
		break;
	case 0x6:
		// TODO
		break;
	case 0x7:
		// TODO
		break;
	case 0x8: // vxvy format
		ctrl->pc_src = 3;
		ctrl->alu_res = 5;

		switch (instr->funct) {
		case 0x0: // AND
			ctrl->alu_op = 0;
			break;
		case 0x1: // OR
			ctrl->alu_op = 1;
			break;
		case 0x2: // ADD
			ctrl->alu_op = 2;
			break;
		case 0x3: // SUB
			ctrl->alu_op = 3;
			break;
		case 0x4: // XOR
			ctrl->alu_op = 4;
			break;
		case 0x5: // SHIFT R
			ctrl->alu_op = 5;
			break;
		case 0x6: // SHIFT L
			ctrl->alu_op = 6;
			break;
		}
		break;
	case 0x9:
		// TODO
		break;
	case 0xA:
		// TODO
		break;
	case 0xB:
		// TODO
		break;
	case 0xC:
		// TODO
		break;
	case 0xD:
		// TODO
		break;
	case 0xE:
		// TODO
		break;
	case 0xF:
		// TODO
		break;
	}
}
