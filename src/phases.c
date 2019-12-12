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
#include "chip8.h"

static void encode(uint8_t *buf, uint8_t num);

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
int fetch_instr(struct chip8_state *state, uint16_t *instr)
{
    LOG("Fetching Instruction...\n");
    if (state->pc >= CHIP8_MEM_SIZE || state->pc + 1 >= CHIP8_MEM_SIZE) {
        PRINT_ERROR("addr [0x%x] too large\n", state->pc)
        return 1;
    }

    *instr = (state->mem[state->pc] << 8) | state->mem[state->pc + 1];
    LOG("Instruction -- 0x%04x\n", *instr);
    return CHIP8_SUCCESS;
}

/*
 * Decodes the given instruction and fills the given instr struct.
 */
void decode_instr(uint16_t raw_instr, struct instruction *instr)
{
    LOG("Decoding Instruction...\n");
    instr->opcode = (raw_instr >> 12) & 0xf;
    instr->vx     = (raw_instr >>  8) & 0xf;
    instr->vy     = (raw_instr >>  4) & 0xf;
    instr->funct  = (raw_instr >>  0) & 0xf;

    instr->kk     = (raw_instr >>  0) & 0xff;

    instr->nnn    = (raw_instr >>  0) & 0xfff;
    LOG("opcode->0x%01x\n", instr->opcode);
    LOG("vx->    0x%01x\n", instr->vx);
    LOG("vy->    0x%01x\n", instr->vy);
    LOG("funct-> 0x%01x\n", instr->funct);
    LOG("kk->    0x%02x\n", instr->kk);
    LOG("nnn->   0x%03x\n", instr->nnn);
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
    ctrl->vf_write = 0;

    switch (instr->opcode) {
    case 0x0:
        // TODO
        switch (instr->kk) {
        case 0xE0: // clear screen
            // TODO (maybe)
            ctrl->fb_write = 2;
            break;
        case 0xEE: // RET from subroutine
            // TODO:
            break;
        default:
            PRINT_ERROR("Unkown instruction\n")
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
        ctrl->vf_write = 1;

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
            PRINT_ERROR("Unknown instruction\n")
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
            PRINT_ERROR("Unknown instruction\n")
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
            PRINT_ERROR("Unknown instruction")
            return CHIP8_ERROR;
        }
        break;
    default:
        PRINT_ERROR("Unknown instruction")
        return CHIP8_ERROR;
    }
    LOG("*** Control Bits ***\n");
    LOG("vfwrite_enable -> %d\n", ctrl->vfwrite_enable);
    LOG("reg_src        -> %d\n", ctrl->reg_src);
    LOG("write_reg      -> %d\n", ctrl->write_reg);
    LOG("i_src          -> %d\n", ctrl->i_src);
    LOG("mem_src        -> %d\n", ctrl->mem_src);
    LOG("mem_write      -> %d\n", ctrl->mem_write);
    LOG("fb_write       -> %d\n", ctrl->fb_write);
    LOG("fb_row         -> %d\n", ctrl->fb_row);
    LOG("sp_src         -> %d\n", ctrl->sp_src);
    LOG("sp_write       -> %d\n", ctrl->sp_write);
    LOG("pc_src         -> %d\n", ctrl->pc_src);
    LOG("delay_hold     -> %d\n", ctrl->delay_hold);
    LOG("sound_hold     -> %d\n", ctrl->sound_hold);
    LOG("xpointer       -> %d\n", ctrl->xpointer);
    LOG("alu_src        -> %d\n", ctrl->alu_src);
    LOG("alu_op         -> %d\n", ctrl->alu_op);
    LOG("not_alu_res    -> %d\n", ctrl->not_alu_res);
    LOG("vf_write       -> %d\n", ctrl->vf_write);
    LOG("********************\n");
    return CHIP8_SUCCESS;
}

// int get_aluin1(struct instruction *instr, uint8_t *regfile, size_t regfile_len, 
//                uint16_t *alu_in1)
int get_aluin1(struct chip8_state *state, uint16_t *alu_in1)
{
    // unpack state
    struct instruction *instr = state->instr;
    uint8_t *regfile = state->regfile;

    // check for out of bounds error
    if (instr->vx >= CHIP8_NUM_REGS) {
        PRINT_ERROR("reg index is [%u], but regfile has len [%u]\n", instr->vx, CHIP8_NUM_REGS)
        return CHIP8_ERROR;
    }

    *alu_in1 = regfile[instr->vx];
    LOG("Alu input 1 is %d\n", *alu_in1);
    return CHIP8_SUCCESS;
}

int get_aluin2(struct chip8_state *state, uint16_t *alu_in2)
{
    // unpack state
    struct instruction *instr = state->instr;
    struct ctrl_bits *ctrl = state->ctrl;
    uint8_t *regfile = state->regfile;

    uint8_t index = 111; // force error if not set
    if (ctrl->alu_src == 0) {
        index = instr->vy;
    } else if (ctrl->alu_src == 1) {
        index = instr->kk;
    } else {
        PRINT_ERROR("PANIC!!\n");
    }

    // check for out of bounds error
    if (index >= CHIP8_NUM_REGS) {
        PRINT_ERROR("reg index is [%u], but regfile has len [%u]\n", index, CHIP8_NUM_REGS)
        return CHIP8_ERROR;
    }

    *alu_in2 = regfile[index];
    LOG("Alu input 2 is %d\n", *alu_in2);
    return CHIP8_SUCCESS;
}

int exec_alu(uint16_t alu_in1, uint16_t alu_in2, struct chip8_state *state)
{
    // unpack state
    uint16_t *alu_res = &(state->alu_res);
    uint8_t *carryout = &(state->carry_out);
    struct ctrl_bits *ctrl = state->ctrl;

    *carryout = 0;
    uint32_t res32 = 0;
    switch (ctrl->alu_op) {
    case 0: // AND
        LOG("Performing an AND op on %d and %d\n", alu_in1, alu_in2);
        *alu_res = alu_in1 & alu_in2;
        break;
    case 1: // OR
        LOG("Performing an OR op on %d and %d\n", alu_in1, alu_in2);
        *alu_res = alu_in1 | alu_in2;
        break;
    case 2: // ADD
        LOG("Performing an ADD op on %d and %d\n", alu_in1, alu_in2);
        res32 = (uint32_t)(alu_in1 + alu_in2);
        *carryout = (res32 >> 16) & 0x1;
        *alu_res = (uint16_t)res32;
        break;
    case 3: // SUB
        LOG("Performing an SUB op on %d and %d\n", alu_in1, alu_in2);
        res32 = (uint32_t)(alu_in1 + ~alu_in2 + 1);
        *carryout = (res32 >> 16) & 0x1;
        *alu_res = (uint16_t)res32;
        break;
    case 4: // XOR
        LOG("Performing an XOR op on %d and %d\n", alu_in1, alu_in2);
        *alu_res = alu_in1 ^ alu_in2;
        break;
    case 5: // shift right
        LOG("Performing an SHIFT R op on %d\n", alu_in1);
        *alu_res = alu_in1 >> 1;
        *carryout = alu_in1 & 0x1;
        break;
    case 6: // shift left
        LOG("Performing an SHIFT L op on %d\n", alu_in1);
        *alu_res = alu_in1 << 1;
        *carryout = (alu_in1 >> 15) & 0x1;
        break;
    default:
        PRINT_ERROR("Unknown alu op\n")
        return CHIP8_ERROR;
    }
    *alu_res = ctrl->not_alu_res == 1 ? !*alu_res : *alu_res;
    return CHIP8_SUCCESS;
}

int mem_phase(struct chip8_state *state)
{
    LOG("Entering mem phase\n");
    // unpack state
    struct ctrl_bits *ctrl = state->ctrl;
    struct instruction *instr = state->instr;
    uint8_t *mem = state->mem;
    uint16_t *i_reg = state->I_reg;
    uint8_t *regfile = state->regfile;


    if (ctrl->mem_write == 1) {
        LOG("Mem write is on!\n");
        switch(ctrl->mem_src) {
        case 0: // Binary Coded Value
            if (instr->vx >= CHIP8_NUM_REGS) {
                PRINT_ERROR("VX value %d not valid!\n", instr->vx)
                return CHIP8_ERROR;
            }
            LOG("Store binary coded value for %d into 0x%04x\n", regfile[instr->vx], *i_reg);

            // convert VX value to text and store encoding at mem[I_reg]
            uint8_t encoded_num[5];
            encode(encoded_num, regfile[instr->vx]);
            for (int i = 0; i < 5; i++) {
                mem[*i_reg + i] = encoded_num[i];
            }
            break;
        case 1: // VX
             ; // make gcc happy :(
            // store vals in regs v0-vx into mem starting at I reg
            size_t addr = *i_reg;
            if (addr >= CHIP8_MEM_SIZE) {
                PRINT_ERROR("Mem access out of bounds\n")
                return CHIP8_ERROR;
            }

            if (instr->vx >= CHIP8_NUM_REGS) {
                PRINT_ERROR("VX value %d not valid!\n", instr->vx)
                return CHIP8_ERROR;
            }
            LOG("Dumping regs until V%X into mem 0x%04x\n", instr->vx, addr);
            
            for (int i = 0; i <= instr->vx; i++) {
                mem[addr] = regfile[i]; 
                addr++;
            }
            break;
        default:
            PRINT_ERROR("Unknown mem_src given\n")
            return CHIP8_ERROR;
        }
    }
    return CHIP8_SUCCESS;
}

int wbphase(struct chip8_state *state, uint8_t randnum)
{
    LOG("Entering WB phase\n");
    // unpack state
    struct ctrl_bits *ctrl = state->ctrl;
    struct instruction *instr = state->instr;
    uint8_t *mem = state->mem;
    uint16_t *i_reg = state->I_reg;
    uint8_t *regfile = state->regfile;
    uint16_t alu_res = state->alu_res;


    if (ctrl->write_reg) {
        LOG("Write reg is turned on!\n");
        if (instr->vx >= CHIP8_NUM_REGS) {
            PRINT_ERROR("Regfile out of bounds on reg %d\n", instr->vx);
            return CHIP8_ERROR;
        }
        switch (ctrl->reg_src) {
        case 0: // DT (delay timer)
            // TODO
            LOG("Delay timer -- not implemented yet :(");
            break;
        case 1: // Rand
            LOG("Writing rand number to V%X\n", instr->vx);
            regfile[instr->vx] = randnum;
            break;
        case 2: // Mem
             ; // make gcc happy :(
            // write into regs V0-VX starting at addr stored in I
            uint16_t addr = *i_reg;
            LOG("Writing into V0-V%X from addr 0x%04x\n", instr->vx, addr);
            for (int i = 0; i <= instr->vx; i++) {
                if (addr >= CHIP8_MEM_SIZE) {
                    PRINT_ERROR("Mem 0x%04x out of bounds\n", addr);
                    return CHIP8_ERROR;
                }
                regfile[i] = mem[addr];
                addr++;
            }
            break;
        case 3: // kk
            LOG("Writing kk = %d into V%X\n", instr->kk, instr->vx);
            regfile[instr->vx] = instr->kk;
            break;
        case 4: // key press
            // TODO
            LOG("Writing key press value -- NOT IMPLEMENTED!\n");
            break;
        case 5: // alu_res
            LOG("Writing alu res %d into V%X\n", alu_res, instr->vx);
            regfile[instr->vx] = (uint16_t) alu_res;
            break;
        default:
            PRINT_ERROR("Unknown reg src\n");
            return 1;
        }
    }

    // carry out write
    if (ctrl->vf_write == 1) {
        LOG("Carry out is on! Writing %d into VF\n", state->carry_out);
        regfile[VF] = state->carry_out;
    }

    return CHIP8_SUCCESS;
}

int get_nextpc(struct chip8_state *state)
{
    LOG("Getting next PC\n");
    switch (state->ctrl->pc_src) {
    case 0: // PC + 2
        LOG("PC = PC + 2\n");
        state->pc += 2;
        break;
    case 1: // PC
        LOG("PC = PC\n");
        break;
    case 2: // PC + 4
        LOG("PC = PC + 4\n");
        state->pc += 4;
        break;
    case 3: // NNN
        LOG("PC = NNN = 0x%04x\n", state->instr->nnn);
        state->pc = state->instr->nnn;
        break;
    case 4: // v0 + nnn
        state->pc = state->instr->nnn + state->regfile[V0];
        LOG("PC = V0 + NNN = \n", state->pc);
        break;
    case 5: // stack
        LOG("PC = STACK -- NOT IMPLEMENTED\n");
        // TODO ???
        PRINT_ERROR("pc from stack not implemented\n");
        return CHIP8_ERROR;
        break;
    default:
        PRINT_ERROR("Unknown pc bit %d\n", state->ctrl->pc_src);
        return CHIP8_ERROR;
    }
    return CHIP8_SUCCESS;
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
