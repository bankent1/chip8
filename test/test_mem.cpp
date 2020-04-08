/*
 * test_mem.cpp
 *
 * Travis Banken
 * 2020
 *
 * Tests for the memory unit
 */

#include <iostream>
#include <mem.h>
#include "test_mem.h"
#include "test_utils.h"

#define MEM_SIZE (4 * 1024)

static bool test_read_write()
{
	Mem mem = Mem();
	bool all_passed = true;
	for (int i = 0; i < MEM_SIZE; i++) {
		mem.write(42, i);
		if (i >= 0x80) {
			printf("Testing @ 0x%03X...", i);
			TEST(mem.read(i) == 42);
			all_passed = all_passed && mem.read(i) == 42;
		}
	}
	return all_passed;
}

bool test_mem::run_all()
{
	bool res = true;
	res = res && test_read_write();
	return res;
}