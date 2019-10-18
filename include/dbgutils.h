/*
 * dbgutils.h
 * 
 * Author: Travis Banken
 * 
 * Header file for the debug utils on the chip 8 processor.
 */

#ifndef DBGUTILS_H
#define DBGUTILS_H

/*
 * Dumps the contents of the given instruction struct to the given file pointer.
 * The outfile is assumed to be open and write-able.
 */
void dump_instr(FILE *outfile, struct instruction *instr);

/*
 * Dumps the contents of the given control bits struct to the given file.
 * The outfile is assumed to be open and write-able.
 */
void dump_ctrlbits(FILE *outfile, struct ctrl_bits *ctrl);

/*
 * Dumps the contents of memory to the given outfile.
 * The outfile is assumed to be open and write-able.
 */
void dump_mem(FILE *outfile, uint8_t *mem, size_t size);

/*
 * Dumps the contents of all registers to the given file *outfile
 * The outfile is assumed to be open and write-able.
 */
void dump_regfile(FILE *outfile, uint8_t *regfile, size_t size);

#endif