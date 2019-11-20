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

// memsize = 4KB
#define MEM_SIZE 4096
#define NUM_REGS 16

#define DATA_START 0x200
#define INSTR_START 0x80
#define FONT_SET_START 0x00

#ifdef DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

// prototypes
static int load_program(uint8_t *mem, FILE *bin_prog);
static int exec_program(uint8_t *mem, size_t memsize, uint8_t *regfile, 
                        size_t regsize, uint16_t *i_reg);
static void dump_all(uint8_t *mem, uint8_t *regfile);


int main(int argc, char *argv[])
{
    int rc = 0;
    printf("Hello, CHIP 8!\n");

    if (debug) {
        printf("Debug Mode On!!\n");
    }

    // TODO: not this
    char *program_name;
    if (argc == 1) {
        program_name = malloc(15);
        strcpy(program_name, "roms/jason.ch8");
    } else {
        program_name = argv[1];
    }

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
        if (debug) {
            dump_all(mem, reg_file);
        }
        return 1;
    }

    return 0;
}

static int exec_program(uint8_t *mem, size_t memsize, uint8_t *regfile, 
                 size_t regsize, uint16_t *I_reg)
{
    int rc = 0;
    int res = 0;

    struct instruction *instr = malloc(sizeof(struct instruction));
    struct ctrl_bits *ctrl = malloc(sizeof(struct ctrl_bits));

    // init chip8 state
    struct chip8_state *state = malloc(sizeof(struct chip8_state));
    state->ctrl = ctrl;
    state->instr = instr;
    state->regfile = regfile;
    state->regsize = regsize;
    state->mem = mem;
    state->memsize = memsize;
    state->I_reg = I_reg;
    state->alu_res = 0;
    state->carry_out = 0;
    state->pc = INSTR_START;

    while (state->pc < DATA_START) {
        if (state->pc < INSTR_START) {
            PRINT_ERROR("exec_program", "Invalid PC value: 0x%04x", state->pc)
            rc = 1;
            break;
        }

        // fetch instr
        uint16_t raw_instr;
        res = fetch_instr(state, &raw_instr);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("fetch_instr")
            rc = 1;
            break;
        }

        // decode instr
        decode_instr(raw_instr, state->instr);

        // fill ctrl bits
        res = fill_ctrl_bits(state->instr, state->ctrl);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("fill_ctrl_bits")
            rc = 1;
            break;
        }

        // exec alu
        uint16_t alu_in1, alu_in2;
        res = get_aluin1(state, &alu_in1);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin1")
            rc = 1;
            break;
        }
        res = get_aluin2(state, &alu_in2);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin2")
            rc = 1;
            break;
        }
        res = exec_alu(alu_in1, alu_in2, state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("exec_alu")
            rc = 1;
            break;
        }

        // mem phase
        res = mem_phase(state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("mem_phase")
            rc = 1;
            break;
        }

        // write back phase
        uint8_t randnum = (uint8_t) rand(); // may need optimization (allowed to be negative??)
        // res = wbphase(ctrl, instr, mem, i_reg, memsize, regfile, regsize, alu_res, randnum);
        res = wbphase(state, randnum);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("wb_phase")
            rc = 1;
            break;
        }

        // TODO: write I

        // carry out TODO: This should happen in wbphase!
        if (ctrl->vf_write == 1) {
            regfile[VF] = state->carry_out;
        }

        // TODO: update screen buffer

        // get next pc
        res = get_nextpc(state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_nextpc");
            rc = 1;
            break;
        }
    }

    // clean up and return
    free(state->instr);
    free(state->ctrl);
    free(state);
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

static void dump_all(uint8_t *mem, uint8_t *regfile)
{
    FILE *memout = fopen("memdump", "w");
    FILE *regout = fopen("regdump", "w");
    if (memout == NULL || regout == NULL) {
        PRINT_ERROR("dump_all", "Failed to open file for dumping\n");
        exit(CHIP8_ERROR);
    }

    dump_mem(memout, mem, MEM_SIZE);
    dump_regfile(regout, regfile, NUM_REGS);
}
