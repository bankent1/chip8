/*
 * chip8.c
 *
 * Author: Travis Banken
 *
 * Main file for the chip 8 processor
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "chip8.h"
#include "phases.h"
#include "dbgutils.h"
#include "chip8-error.h"

// memsize = 4KB
#define MEM_SIZE 4096
#define NUM_REGS 16

#define DATA_START 0x200
#define INSTR_START 0x80
#define FONT_SET_START 0x00

// prototypes
static int load_program(uint8_t *mem, FILE *bin_prog);
static int exec_program(uint8_t *mem, size_t memsize, uint8_t *regfile, 
                        size_t regsize, uint16_t *i_reg);


int main(int argc, char *argv[])
{
    int rc = 0;
    printf("Hello, CHIP 8!\n");

    // init memory
    uint8_t mem[MEM_SIZE];
    memset((void *) mem, 0, MEM_SIZE);

    // init reg_file and spec regs
    uint8_t reg_file[NUM_REGS];
    uint16_t *I_reg = 0;

    FILE *program = fopen("roms/jason.ch8", "r");
    if (program == NULL) {
        perror("fopen");
        return 1;
    }

    rc = load_program(mem, program);
    if (rc != CHIP8_SUCCESS) {
        EXIT_ERROR("load_program");
        return 1;
    }

    rc = exec_program(mem, MEM_SIZE, reg_file, NUM_REGS, I_reg);
    if (rc != CHIP8_SUCCESS) {
        EXIT_ERROR("exec_program");
        return 1;
    }

    return 0;
}

// static void zero_mem(uint8_t *mem, size_t size)
// {
//     for (int i = 0; i < size; i++) {
//         mem[i] = 0;
//     }
// }

static int exec_program(uint8_t *mem, size_t memsize, uint8_t *regfile, 
                 size_t regsize, uint16_t *i_reg)
{
    int rc = 0;
    int res = 0;

    struct instruction *instr = malloc(sizeof(struct instruction));
    struct ctrl_bits *ctrl = malloc(sizeof(struct ctrl_bits));

    uint32_t pc = INSTR_START;
    while (pc < DATA_START) {
        if (pc < INSTR_START) {
            PRINT_ERROR("exec_program", "Invalid PC value: 0x%04x", pc)
            rc = 1;
            break;
        }

        // fetch instr
        uint16_t raw_instr;
        if (fetch_instr(mem, memsize, pc, &raw_instr) != CHIP8_SUCCESS) {
            EXIT_ERROR("fetch_instr")
            rc = 1;
            break;
        }

        // decode instr
        decode_instr(raw_instr, instr);

        // fill ctrl bits
        res = fill_ctrl_bits(instr, ctrl);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("fill_ctrl_bits")
            rc = 1;
            break;
        }

        // exec alu
        uint16_t alu_in1, alu_in2;
        res = get_aluin1(instr, regfile, regsize, &alu_in1);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin1")
            rc = 1;
            break;
        }

        res = get_aluin2(ctrl, instr, regfile, regsize, &alu_in2);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin2")
            rc = 1;
            break;
        }

        uint16_t alu_res;
        uint8_t carry_out;
        res = exec_alu(alu_in1, alu_in2, &alu_res, &carry_out, ctrl);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("exec_alu")
            rc = 1;
            break;
        }

        res = mem_phase(ctrl, instr, mem, memsize, i_reg, regfile, regsize);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("mem_phase")
            rc = 1;
            break;
        }

        int randnum = rand(); // may need optimization
        res = wbphase(ctrl, instr, mem, i_reg, memsize, regfile, regsize, alu_res, randnum);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("wb_phase")
            rc = 1;
            break;
        }

        // TODO: write I

        // TODO: carry out
        // TODO: get next pc
    }

    // clean up and return
    free(instr);
    free(ctrl);
    return rc;
}

static int load_program(uint8_t *mem, FILE *bin_prog)
{
    uint8_t *memptr = mem + INSTR_START;
    if (fread(memptr, 1, (DATA_START - INSTR_START), bin_prog) == 0) {
        if (ferror(bin_prog) != 0) {
            PRINT_ERROR("load_program", "Error on fread")
            return 1;
        }
    }

    return 0;
}
