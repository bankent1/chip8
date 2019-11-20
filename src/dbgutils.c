/*
 * dbgutils.c
 * 
 * Author: Travis Banken
 * 
 * Debug utils for the chip 8 processor.
 */

#include <stdio.h>

#include "phases.h"
#include "dbgutils.h"

void dump_instr(FILE *outfile, struct instruction *instr)
{
    fprintf(outfile, "Dumping instruction struct [%p]...\n", instr);
    fprintf(outfile, "opcode:    0x%x\n", instr->opcode);
    fprintf(outfile, "vx:          %u\n", instr->vx);
    fprintf(outfile, "vy:          %u\n", instr->vy);
    fprintf(outfile, "funct:     0x%x\n", instr->funct);
}

void dump_ctrlbits(FILE *outfile, struct ctrl_bits *ctrl)
{
    fprintf(outfile, "Dumping ctrl-bits struct [%p]...\n", ctrl);
    fprintf(outfile, "vfwrite_enable:      %u\n", ctrl->vfwrite_enable);
    fprintf(outfile, "reg_src:             %u\n", ctrl->reg_src);
    fprintf(outfile, "write_reg:           %u\n", ctrl->write_reg);
    fprintf(outfile, "i_src:               %u\n", ctrl->i_src);
    fprintf(outfile, "mem_src:             %u\n", ctrl->mem_src);
    fprintf(outfile, "mem_write:           %u\n", ctrl->mem_write);
    fprintf(outfile, "fb_write:            %u\n", ctrl->fb_write);
    fprintf(outfile, "fb_row:              %u\n", ctrl->fb_row);
    fprintf(outfile, "sp_src:              %u\n", ctrl->sp_src);
    fprintf(outfile, "sp_write:            %u\n", ctrl->sp_write);
    fprintf(outfile, "pc_src:              %u\n", ctrl->pc_src);
    fprintf(outfile, "delay_hold:          %u\n", ctrl->delay_hold);
    fprintf(outfile, "sound_hold:          %u\n", ctrl->sound_hold);
    fprintf(outfile, "alu_src:             %u\n", ctrl->alu_src);
    fprintf(outfile, "alu_op:              %u\n", ctrl->alu_op);
    fprintf(outfile, "not_alu_res:         %u\n", ctrl->not_alu_res);
}

void dump_mem(FILE *outfile, uint8_t *mem, size_t size)
{
    fprintf(outfile, "==============================================\n");
    fprintf(outfile, "             *** Dumping MEMORY ***           \n");
    for (int i = 0; i < size; i++) {
        if ((i) % 8 == 0)
            fprintf(outfile, "\n");
        fprintf(outfile, "0x%04x: 0x%02x  |  ", i, mem[i]);
    }
    fprintf(outfile, "\n\n==============================================\n");
}

void dump_regfile(FILE *outfile, uint8_t *regfile, size_t size)
{
    fprintf(outfile, "=========================\n");
    fprintf(outfile, "      Dumping REGFILE    \n");
    for (int i = 0; i < size; i++) {
        fprintf(outfile, "$%d: %u\n", i, regfile[i]);
    }
    fprintf(outfile, "=========================\n");
}