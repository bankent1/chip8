/*
 * chip8.h
 * 
 * Author: Travis Banken
 *
 * Header file for the main file for the chip 8 processor.
 */


#ifndef CHIP8_H
#define CHIP8_H

#include "phases.h"


struct chip8_state {
	struct ctrl_bits *ctrl;
	struct instruction *instr;
	uint8_t *mem;
	size_t memsize;

	uint16_t *I_reg;

	uint8_t *regfile;
	size_t regsize;
	
	uint16_t alu_res;
	uint8_t carry_out;
	uint32_t pc;
};

// REG MACROS
#define V0 0x0
#define V1 0x1
#define V2 0x2
#define V3 0x3
#define V4 0x4
#define V5 0x5
#define V6 0x6
#define V7 0x7
#define V8 0x8
#define V9 0x9
#define VA 0xA
#define VB 0xB
#define VC 0xC
#define VD 0xD
#define VE 0xE
#define VF 0xF

// ERROR MACROS
#define PRINT_ERROR(function, reason, ...) \
    fprintf((stderr), "Error [%s]: ", (function)); \
    fprintf((stderr), (reason), ##__VA_ARGS__);

#define EXIT_ERROR(function) fprintf(stderr, "[%s] failed, exiting...\n", (function));

// ERROR CODES
#define CHIP8_SUCCESS 0
#define CHIP8_ERROR 1


#endif
