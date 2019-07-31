#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "phases.h"

/*
 * Fetches the instruction from the given address.
 *
 * Returns 1 on bad address call.
 */
int fetch_instr(uint8_t *mem, size_t size, uint16_t addr, uint16_t *instr)
{
	if (addr >= size || addr + 1 >= size) {
		fprintf(stderr, "fetch_instr(): addr [0x%x] too large\n", addr);
		return 1;
	}

	*instr = (mem[addr] << 8) | mem[addr + 1];
	return 0;
}

int decode_instr(uint8_t raw_instr, struct instruction *instr)
{
	return 0;
}

