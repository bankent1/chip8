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
#include "chip8-error.h"

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
int fetch_instr(uint8_t *mem, size_t size, uint16_t addr, uint16_t *instr)
{
    if (addr >= size || addr + 1 >= size) {
        PRINT_ERROR("fetch_instr", "addr [0x%x] too large\n", addr);
        return 1;
    }

    *instr = (mem[addr] << 8) | mem[addr + 1];
    return CHIP8_SUCCESS;
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
            PRINT_ERROR("fill_instr_bits", "Unkown instruction\n");
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
            PRINT_ERROR("fill_instr_bits", "Unknown instruction\n");
            return CHIP8_ERROR;
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
            PRINT_ERROR("fill_instr_bits", "Unknown instruction\n");
            return CHIP8_ERROR;
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
            // fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction\n");
            PRINT_ERROR("fill_instr_bits", "Unknown instruction");
            return CHIP8_ERROR;
        }
        break;
    default:
        // fprintf(stderr, "Error [fill_instr_bits]: Unknown instruction\n");
        PRINT_ERROR("fill_ctrl_bits", "Unknown instruction");
        return CHIP8_ERROR;
    }
    return CHIP8_SUCCESS;
}

int get_aluin1(struct instruction *instr, uint8_t *regfile, size_t regfile_len, 
               uint16_t *alu_in1)
{
    // check for out of bounds error
    if (instr->vx >= regfile_len) {
        // fprintf(stderr, "Error [get_aluin1]: reg index is [%u], but regfile has \
        //                  len [%lu]\n", instr->vx, regfile_len);
        PRINT_ERROR("get_aluin1", "reg index is [%u], but regfile has len [%lu]\n", instr->vx, regfile_len);
        return CHIP8_ERROR;
    }

    *alu_in1 = regfile[instr->vx];
    return CHIP8_SUCCESS;
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
        // fprintf(stderr, "Error [get_aluin2]: reg index is [%u], but regdile has \
        //                  len [%lu]\n", index, regfile_len);
        PRINT_ERROR("get_aluin2", "reg index is [%u], but regfile has len [%lu]\n", index, regfile_len);
        return CHIP8_ERROR;
    }

    *alu_in2 = regfile[index];
    return CHIP8_SUCCESS;
}

// execute alu based on alu_op ctrl bit
int exec_alu(uint16_t alu_in1, uint16_t alu_in2, uint16_t *alu_res, 
             uint8_t *carryout, struct ctrl_bits *ctrl)
{
    *carryout = 0;
    uint32_t res32 = 0;
    switch (ctrl->alu_op) {
    case 0: // AND
        *alu_res = alu_in1 & alu_in2;
        break;
    case 1: // OR
        *alu_res = alu_in1 | alu_in2;
        break;
    case 2: // ADD
        res32 = (uint32_t)(alu_in1 + alu_in2);
        *carryout = (res32 >> 16) & 0x1;
        *alu_res = (uint16_t)res32;
        break;
    case 3: // SUB
        res32 = (uint32_t)(alu_in1 + ~alu_in2 + 1);
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
        // fprintf(stderr, "Error [exec_alu]: Unknown alu op\n");
        PRINT_ERROR("exec_alu", "Unknown alu op\n");
        return CHIP8_ERROR;
    }
    *alu_res = ctrl->not_alu_res == 1 ? !*alu_res : *alu_res;
    return CHIP8_SUCCESS;
}

int mem_phase(struct ctrl_bits *ctrl, struct instruction *instr, uint8_t *mem, 
              size_t memsize, uint16_t *i_reg, uint8_t *regfile, size_t regsize)
{
    if (ctrl->mem_write == 1) {
        // array out of bounds check
        if (addr >= memsize) {
            // fprintf(stderr, "Error [mem_phase]: Address out of mem bounds\n");
            PRINT_ERROR("mem_phase", "Address out of mem bounds\n");
            return CHIP8_ERROR;
        }

        switch(ctrl->mem_src) {
        case 0: // Binary Coded Value
            if (instr->vx >= regsize) {
                PRINT_ERROR("mem_phase", "VX value %d not valid!\n", instr->vx);
                return CHIP8_ERROR;
            }

            // convert VX value to text and store encoding at mem[I_reg]
            uint8_t encoded_num[5];
            encode(encoded_num, regfile[instr->vx]);
            for (int i = 0; i < 5; i++) {
                mem[*i_reg + i] = encoded_num[i];
            }
            break;
        case 1: // VX
            // store vals in regs v0-vx into mem starting at I reg
            size_t addr = *i_reg;
            if (addr >= memsize) {
                // fprintf(stderr, "Error [mem_phase]: Regfile access out of bounds\n");
                PRINT_ERROR("mem_phase", "Mem access out of bounds\n");
                return CHIP8_ERROR;
            }

            if (instr->vx >= regsize) {
                PRINT_ERROR("mem_phase", "VX value %d not valid!\n", instr->vx);
                return CHIP8_ERROR;
            }
            
            for (int i = 0; i <= instr->vx; i++) {
                mem[addr] = regfile[i]; 
                addr++;
            }
            break;
        default:
            // fprintf(stderr, "Error [mem_phase]: Unknown mem_src given\n");
            PRINT_ERROR("mem_phase", "Unknown mem_src given\n");
            return CHIP8_ERROR;
        }
    }
    return CHIP8_SUCCESS;
}

int wbphase(struct ctrl_bits *ctrl, struct instruction *instr, uint8_t *mem,
            uint16_t *i_reg, size_t memsize, uint8_t *regfile, size_t regsize, 
            uint16_t alu_res, uint8_t randnum)
{
    if (ctrl->write_reg) {
        if (instr->vx) {
            fprintf(stderr, "Error [wbphase]: Regfile out of bounds");
            return 1;
        }
        switch (ctrl->reg_src) {
        case 0: // DT (delay timer)
            // TODO
            break;
        case 1: // Rand
            regfile[instr->vx] = randnum;
            break;
        case 2: // Mem
            // write into regs V0-VX starting at addr stored in I
            size_t addr = *i_reg;
            for (int i = 0; i <= instr->vx; i++) {
                if (addr >= memsize) {
                    fprintf(stderr, "Error [wbphase]: Mem out of bounds");
                    return 1;
                }
                regfile[i] = mem[addr];
                addr++;
            }
            break;
        case 3: // kk
            regfile[instr->vx] = instr->kk;
            break;
        case 4: // key press
            // TODO
            break;
        case 5: // alu_res
            regfile[instr->vx] = (uint16_t) alu_res;
            break;
        default:
            fprintf(stderr, "Error [wbphase]: Unknown reg src");
            return 1;
        }
    }

    return 0;
}

// *** HELPER FUNCTIONS ***
static void encode(uint8_t *buf, uint8_t num)
{
    // encode num into 5 bytes to be drawn on a screen
    switch(num) {
    case 0x00:
        buf[0] = 0xF0;
        buf[1] = 0x90;
        buf[2] = 0x90;
        buf[3] = 0x90;
        buf[4] = 0xF0;
        break;
    case 0x01:
        buf[0] = 0x20;
        buf[1] = 0x60;
        buf[2] = 0x20;
        buf[3] = 0x20;
        buf[4] = 0x70;
        break;
    case 0x02:
        buf[0] = 0xF0;
        buf[1] = 0x10;
        buf[2] = 0xF0;
        buf[3] = 0x80;
        buf[4] = 0xF0;
        break;
    case 0x03:
        buf[0] = 0xF0;
        buf[1] = 0x10;
        buf[2] = 0xF0;
        buf[3] = 0x10;
        buf[4] = 0xF0;
        break;
    case 0x04:
        buf[0] = 0x90;
        buf[1] = 0x90;
        buf[2] = 0xF0;
        buf[3] = 0x10;
        buf[4] = 0x10;
        break;
    case 0x05:
        buf[0] = 0xF0;
        buf[1] = 0x80;
        buf[2] = 0xF0;
        buf[3] = 0x10;
        buf[4] = 0xF0;
        break;
    case 0x06:
        buf[0] = 0xF0;
        buf[1] = 0x80;
        buf[2] = 0xF0;
        buf[3] = 0x90;
        buf[4] = 0xF0;
        break;
    case 0x07:
        buf[0] = 0xF0;
        buf[1] = 0x10;
        buf[2] = 0x20;
        buf[3] = 0x40;
        buf[4] = 0x40;
        break;
    case 0x08:
        buf[0] = 0xF0;
        buf[1] = 0x90;
        buf[2] = 0xF0;
        buf[3] = 0x90;
        buf[4] = 0xF0;
        break;
    case 0x09:
        buf[0] = 0xF0;
        buf[1] = 0x90;
        buf[2] = 0xF0;
        buf[3] = 0x10;
        buf[4] = 0xF0;
        break;
    case 0x0a:
        buf[0] = 0xF0;
        buf[1] = 0x90;
        buf[2] = 0xF0;
        buf[3] = 0x90;
        buf[4] = 0x90;
        break;
    case 0x0b:
        buf[0] = 0xE0;
        buf[1] = 0x90;
        buf[2] = 0xE0;
        buf[3] = 0x90;
        buf[4] = 0xE0;
        break;
    case 0x0c:
        buf[0] = 0xF0;
        buf[1] = 0x80;
        buf[2] = 0x80;
        buf[3] = 0x80;
        buf[4] = 0xF0;
        break;
    case 0x0d:
        buf[0] = 0xE0;
        buf[1] = 0x90;
        buf[2] = 0x90;
        buf[3] = 0x90;
        buf[4] = 0xE0;
        break;
    case 0x0e:
        buf[0] = 0xF0;
        buf[1] = 0x80;
        buf[2] = 0xF0;
        buf[3] = 0x80;
        buf[4] = 0xF0;
        break;
    case 0x0f:
        buf[0] = 0xF0;
        buf[1] = 0x80;
        buf[2] = 0xF0;
        buf[3] = 0x80;
        buf[4] = 0x80;
        break;
    }
}