/*
 * phases.c
 *
 * Author: Travis Banken
 *
 * Holds the code for the different phases and helper functions for fetching, 
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
		fprintf(stderr, "Error [fetch_instr]: addr [0x%x] too large\n", addr);
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


// fills initial control bits
int fill_ctrl_bits(struct instruction *instr, struct ctrl_bits *ctrl)
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
	ctrl->not_alu_res = 0;

	switch (instr->opcode) {
	case 0x0:
		// TODO
		switch (instr->kk) {
		case 0xE0: // clear screen
			// TODO
			break;
		case 0xEE: // RET from subroutine
			// TODO:
			break;
		default:
			fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction");
			return 1;
		}
		break;
	case 0x1: // jump to nnn
		ctrl->pc_src = 3;
		break;
	case 0x2: // call subroutine at nnn
		// TODO
		break;
	case 0x3: // skp nxt instr if vx == kk
		ctrl->pc_src = 2; // TODO: requre pc to check if eq
		ctrl->not_alu_res = 1;
		ctrl->alu_op = 4;
		ctrl->alu_src = 1;
		break;
	case 0x4: // skp nxt isntr if vx != kk
		ctrl->pc_src = 2; // TODO: require pc to check if eq
		ctrl->alu_op = 4;
		ctrl->alu_src = 1;
		break;
	case 0x5: // skp nxt instr if vx == vy
		ctrl->pc_src = 2; // TODO: requre pc to check if eq
		ctrl->not_alu_res = 1;
		ctrl->alu_op = 4;
		ctrl->alu_src = 0;
		break;
	case 0x6: // set vx = kk
		ctrl->write_reg = 1;
		ctrl->reg_src = 3;
		break;
	case 0x7: // vx += kk
		ctrl->write_reg = 1;
		ctrl->reg_src = 5;
		ctrl->alu_src = 1;
		ctrl->alu_op = 2;
		break;
	case 0x8: // vxvy format
		ctrl->reg_src = 5;
		ctrl->write_reg = 1;
		ctrl->alu_src = 0;

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
		default:
			fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction");
			return 1;
		}
		break;
	case 0x9: // skp nxt instr if vx != vy
		ctrl->pc_src = 2; // TODO: require pc to check if eq
		ctrl->alu_op = 4;
		ctrl->alu_src = 0;
		break;
	case 0xA: // set I to nnn
		ctrl->i_src = 1;
		break;
	case 0xB: // jump to nnn + v0
		ctrl->pc_src = 4; // TODO: do nnn + v0
		break;
	case 0xC:
		ctrl->write_reg = 1;
		ctrl->reg_src = 1; // TODO: rand && nnn
		break;
	case 0xD: // draw at coor (vx, vy) with width 8px and height N px
		// TODO: May need revisions
		ctrl->fb_write = 1;
		ctrl->fb_row = 1; // ????
		break;
	case 0xE: // KeyOp
		// TODO
		switch(instr->kk) {
		case 0x9E: // skp instr if key in vx is pressed
			ctrl->pc_src = 2;
			// TODO: other bits ???
			break;
		case 0xA1: // skp instr if key in vx is not pressed
			ctrl->pc_src = 2;
			// TODO: other bits ???
			break;
		default:
			fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction");
			return 1;
		}
		break;
	case 0xF:
		// TODO
		switch(instr->kk) {
		case 0x07: // store current delay timer in vx
			ctrl->write_reg = 1;
			ctrl->reg_src = 0;
			break;
		case 0x0A: // wait for key press and store in vx
			ctrl->write_reg = 1;
			ctrl->reg_src = 4;
			break;
		case 0x15: // set value of delay to vx
			// TODO
			ctrl->delay_hold = 1;
			break;
		case 0x18: // set value of sound timer to vx
			ctrl->sound_hold = 1;
			break;
		case 0x1E: // add the value of vx to I
			ctrl->i_src = 1;
			break;
		case 0x29: // Sets I to the location of the sprite for the character 
				   // in VX. Characters 0-F (in hexadecimal) are represented 
				   // by a 4x5 font.
			// TODO
			break;
		case 0x33: // store decimal rep vx, storing most-sigfig at I, mid-sigfig
				   // at I + 1, least-sigfig at I + 2
			// TODO
			break;
		case 0x55: // store v0 to vx in mem starting at addr I
			// TODO
			break;
		case 0x65: // fill v0 to vx from mem starting at addr I
			// TODO
			break;
		default:
			fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction");
			return 1;
		}
		break;
	default:
		fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction");
		return 1;
	}
	return 0;
}

int get_aluin1(struct instruction *instr, uint8_t *regfile, size_t regfile_len, 
               uint16_t *alu_in1)
{
	// check for out of bounds error
	if (instr->vx >= regfile_len) {
		fprintf(stderr, "Error [get_aluin1]: reg index is [%u], but regfile has \
		                 len [%u]", instr->vx, regfile_len);
		return 1;
	}

	*alu_in1 = regfile[instr->vx];
	return 0;
}

int get_aluin2(struct ctrl_bits *ctrl, struct instruction *instr,
                    uint8_t *regfile, size_t regfile_len, uint16_t *alu_in2)
{
	uint8_t index = 111; // force error if not set
	if (ctrl->alu_src == 0)
		index = instr->vy;
	else if (ctrl->alu_src == 1)
		index = instr->kk;

	// check for out of bounds error
	if (index >= regfile_len) {
		fprintf(stderr, "Error [get_aluin2]: reg index is [%u], but regdile has \
		                 len [%u]", index, regfile_len);
		return 1;
	}

	*alu_in2 = regfile[index];
	return 0;
}

// execute alu based on alu_op ctrl bit
int exec_alu(uint16_t alu_in1, uint16_t alu_in2, uint16_t *alu_res, 
             uint8_t *carryout, struct ctrl_bits *ctrl)
{
	*carryout = 0;
	switch (ctrl->alu_op) {
	case 0: // AND
		*alu_res = alu_in1 & alu_in2;
		break;
	case 1: // OR
		*alu_res = alu_in1 | alu_in2;
		break;
	case 2: // ADD
		uint32_t res32 = (uint32_t)(alu_in1 + alu_in2);
		*carryout = (res32 >> 16) & 0x1;
		*alu_res = (uint16_t)res32;
		break;
	case 3: // SUB
		uint32_t res32 = (uint32_t)(alu_in1 + ~alu_in2 + 1);
		*carryout = (res32 >> 16) & 0x1;
		*alu_res = (uint16_t)res32;
		break;
	case 4: // XOR
		*alu_res = alu_in1 ^ alu_in2;
		break;
	case 5: // shift right
		*alu_res = alu_in1 >> 1;
		*carryout = alu_in1 & 0x1;
		break;
	case 6: // shift left
		*alu_res = alu_in1 << 1;
		*carryout = (alu_in1 >> 15) & 0x1;
		break;
	default:
		fprintf(stderr, "Error [exec_alu]: Unknown alu op");
		return 1;
	}
	return 0;
}