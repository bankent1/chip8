/*
 * test-phases.c
 * 
 * Author: Travis Banken
 * 
 * Contains tests for the phases functions in the chip 8 processor
 */

#include <stdlib.h>
#include <stdio.h>

#include "../phases.h"
#include "tests.h"

int test_exec_alu()
{
	struct ctrl_bits *ctrl = malloc(sizeof(struct ctrl_bits));
	int rc = 0;


	// TEST AND
	uint16_t alu_in1 = 0b1101;
	uint16_t alu_in2 = 0b1011;
	uint16_t alu_res = 0;
	uint8_t carryout = 0;
	ctrl->alu_op = 0;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "AND");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == 0b1001) {
		TEST_PASSED("exec_alu", "AND");
	} else {
		TEST_FAILED("exec_alu", "AND");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST OR
	alu_in1 = 0b1101;
	alu_in2 = 0b1011;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 1;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "OR");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == 0b1111) {
		TEST_PASSED("exec_alu", "OR");
	} else {
		TEST_FAILED("exec_alu", "OR");
		REASON("Wrong answer");
		rc = 1;
	}
	
	// TEST ADD NO OVERFLOW
	alu_in1 = 0b1101;
	alu_in2 = 0b1011;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 2;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "ADD NO OVERFLOW");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (alu_in1 + alu_in2) && carryout == 0) {
		TEST_PASSED("exec_alu", "ADD NO OVERFLOW");
	} else {
		TEST_FAILED("exec_alu", "ADD NO OVERFLOW");
		REASON("Wrong answer");
		rc = 1;
	}

	return rc;
}
