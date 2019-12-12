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
#include <stdarg.h>

#include "chip8.h"
#include "phases.h"
#include "dbgutils.h"

#ifdef DEBUG
static int debug = 1;
#else
static int debug = 0;
#endif

void dbg_printf(const char *fmt, ...)
{
    if (!debug)
        return;

    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

// prototypes
static int load_program(uint8_t *mem, FILE *bin_prog);
static int exec_program(uint8_t *mem, uint8_t *regfile, uint16_t *i_reg);
static void dump_all(uint8_t *mem, uint8_t *regfile);


int main(int argc, char *argv[])
{
    int rc = 0;
    printf("Hello, CHIP 8!\n");

    dbg_printf("Debug turned on\n");

    // TODO: not this
    char *program_name;
    if (argc == 1) {
        program_name = malloc(15);
        strcpy(program_name, "roms/jason.ch8");
    } else {
        program_name = argv[1];
    }

    // init memory
    uint8_t mem[CHIP8_MEM_SIZE];
    memset((void *) mem, 0, CHIP8_MEM_SIZE);

    // init reg_file and spec regs
    uint8_t reg_file[CHIP8_NUM_REGS];
    uint16_t *I_reg = 0;

    LOG("Opening %s\n", program_name);
    FILE *program = fopen(program_name, "r");
    if (program == NULL) {
        perror("fopen");
        return CHIP8_ERROR;
    }

    LOG("Loading program %s into memory.\n", program_name);
    rc = load_program(mem, program);
    if (rc != CHIP8_SUCCESS) {
        EXIT_ERROR("load_program");
        return CHIP8_ERROR;
    }

    LOG("Starting %s...\n", program_name);
    rc = exec_program(mem, reg_file, I_reg);
    if (rc != CHIP8_SUCCESS) {
        EXIT_ERROR("exec_program");
        if (debug) {
            dump_all(mem, reg_file);
        }
        return CHIP8_ERROR;
    }

    return CHIP8_SUCCESS;
}

static int exec_program(uint8_t *mem, uint8_t *regfile, uint16_t *I_reg)
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
    state->mem = mem;
    state->I_reg = I_reg;
    state->alu_res = 0;
    state->carry_out = 0;
    state->pc = CHIP8_INSTR_START;

    while (state->pc < CHIP8_DATA_START) {
        dbg_printf("------------------------------------------\n");
        LOG("Current pc -> 0x%04x\n", state->pc);
        dbg_printf("------------------------------------------\n");
        if (state->pc < CHIP8_INSTR_START) {
            PRINT_ERROR("Invalid PC value: 0x%04x", state->pc);
            rc = CHIP8_ERROR;
            break;
        }

        // fetch instr
        uint16_t raw_instr;
        res = fetch_instr(state, &raw_instr);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("fetch_instr")
            rc = CHIP8_ERROR;
            break;
        }

        // decode instr
        decode_instr(raw_instr, state->instr);

        // fill ctrl bits
        res = fill_ctrl_bits(state->instr, state->ctrl);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("fill_ctrl_bits")
            rc = CHIP8_ERROR;
            break;
        }

        // exec alu
        uint16_t alu_in1, alu_in2;
        res = get_aluin1(state, &alu_in1);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin1")
            rc = CHIP8_ERROR;
            break;
        }
        res = get_aluin2(state, &alu_in2);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_aluin2")
            rc = CHIP8_ERROR;
            break;
        }
        res = exec_alu(alu_in1, alu_in2, state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("exec_alu")
            rc = CHIP8_ERROR;
            break;
        }

        // mem phase
        res = mem_phase(state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("mem_phase")
            rc = CHIP8_ERROR;
            break;
        }

        // write back phase
        uint8_t randnum = (uint8_t) rand(); // may need optimization (allowed to be negative??)
        res = wbphase(state, randnum);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("wb_phase")
            rc = CHIP8_ERROR;
            break;
        }

        // TODO: write I

        // TODO: update screen buffer

        // get next pc
        res = get_nextpc(state);
        if (res != CHIP8_SUCCESS) {
            EXIT_ERROR("get_nextpc");
            rc = CHIP8_ERROR;
            break;
        }
        dbg_printf("------------------------------------------\n");
    }

    // clean up and return
    free(state->instr);
    free(state->ctrl);
    free(state);
    return rc;
}

static int load_program(uint8_t *mem, FILE *bin_prog)
{
    uint8_t *memptr = mem + CHIP8_INSTR_START;
    if (fread(memptr, 1, (CHIP8_DATA_START - CHIP8_INSTR_START), bin_prog) == 0) {
        if (ferror(bin_prog) != 0) {
            PRINT_ERROR("Error on fread");
            return CHIP8_ERROR;
        }
    }

    return CHIP8_SUCCESS;
}

static void dump_all(uint8_t *mem, uint8_t *regfile)
{
    FILE *memout = fopen("memdump", "w");
    FILE *regout = fopen("regdump", "w");
    if (memout == NULL || regout == NULL) {
        PRINT_ERROR("Failed to open file for dumping\n");
        exit(CHIP8_ERROR);
    }

    dump_mem(memout, mem, CHIP8_MEM_SIZE);
    dump_regfile(regout, regfile, CHIP8_NUM_REGS);
}
