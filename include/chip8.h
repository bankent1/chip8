/*
 * chip8.h
 * 
 * Author: Travis Banken
 *
 * Header file for the main file for the chip 8 processor.
 */

#ifndef CHIP8_H
#define CHIP8_H

void zero_mem(uint8_t *mem, size_t size);

int load_program(uint8_t *mem, FILE *bin_prog);

#endif
