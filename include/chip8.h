/*
 * chip8.h
 * 
 * Author: Travis Banken
 *
 * Header file for the main file for the chip 8 processor.
 */


#ifndef CHIP8_H
#define CHIP8_H

// sizes
#define CHIP8_MEM_SIZE 4096
#define CHIP8_NUM_REGS 16

// memory layout
#define CHIP8_DATA_START 0x200
#define CHIP8_INSTR_START 0x80
#define CHIP8_FONT_START 0x00

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
#define PRINT_ERROR(reason, ...) \
    fprintf((stderr), "Error [%s:%d]: ", __FUNCTION__, __LINE__); \
    fprintf((stderr), (reason), ##__VA_ARGS__);

#define EXIT_ERROR(function) fprintf(stderr, "[%s] failed, exiting...\n", (function));

// ERROR CODES
#define CHIP8_SUCCESS 0
#define CHIP8_ERROR 1

// Debug printing
void dgb_printf(const char *fmt, ...);
#define LOG(msg, ...) dbg_printf("%s: ", __FUNCTION__); \
    dbg_printf(msg, ##__VA_ARGS__);


#endif
