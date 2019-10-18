/*
 * test-phases.c
 * 
 * Author: Travis Banken
 * 
 * Contains tests for the phases functions in the chip 8 processor
 */

#include <stdlib.h>
#include <stdio.h>

#include "phases.h"
#include "tests/tests.h"

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
	} else if (alu_res == (uint16_t)(alu_in1 + alu_in2) && carryout == 0) {
		TEST_PASSED("exec_alu", "ADD NO OVERFLOW");
	} else {
		TEST_FAILED("exec_alu", "ADD NO OVERFLOW");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST ADD w/ OVERFLOW
	alu_in1 = 0xffff;
	alu_in2 = 0xffff;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 2;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "ADD W/ OVERFLOW");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 + alu_in2) && carryout == 1) {
		TEST_PASSED("exec_alu", "ADD W/ OVERFLOW");
	} else {
		TEST_FAILED("exec_alu", "ADD W/ OVERFLOW");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SUB NO UNDERFLOW
	alu_in1 = 0b1101;
	alu_in2 = 0b1011;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 3;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SUB NO UNDERFLOW");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 - alu_in2) && carryout == 0) {
		TEST_PASSED("exec_alu", "SUB NO UNDERFLOW");
	} else {
		TEST_FAILED("exec_alu", "SUB NO UNDERFLOW");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SUB w/ OVERFLOW
	alu_in1 = 0x0fff;
	alu_in2 = 0xffff;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 3;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SUB W/ UNDERFLOW");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 - alu_in2) && carryout == 1) {
		TEST_PASSED("exec_alu", "SUB W/ UNDERFLOW");
	} else {
		TEST_FAILED("exec_alu", "SUB W/ UNDERFLOW");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST XOR
	alu_in1 = 0x0f3f;
	alu_in2 = 0xf5f1;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 4;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "XOR");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 ^ alu_in2) && carryout == 0) {
		TEST_PASSED("exec_alu", "XOR");
	} else {
		TEST_FAILED("exec_alu", "XOR");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SHIFT R NO CARRYOUT
	alu_in1 = 0b1100;
	alu_in2 = 0;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 5;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SHIFT R NO CARRYOUT");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 >> 1) && carryout == 0) {
		TEST_PASSED("exec_alu", "SHIFT R NO CARRYOUT");
	} else {
		TEST_FAILED("exec_alu", "SHIFT R NO CARRYOUT");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SHIFT R W/ CARRYOUT
	alu_in1 = 0x0fff;
	alu_in2 = 0;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 5;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SHIFT R W/ CARRYOUT");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 >> 1) && carryout == 1) {
		TEST_PASSED("exec_alu", "SHIFT R W/ CARRYOUT");
	} else {
		TEST_FAILED("exec_alu", "SHIFT W/ CARRYOUT");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SHIFT L NO CARRYOUT
	alu_in1 = 0b0101;
	alu_in2 = 0;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 6;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SHIFT L NO CARRYOUT");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 << 1) && carryout == 0) {
		TEST_PASSED("exec_alu", "SHIFT L NO CARRYOUT");
	} else {
		TEST_FAILED("exec_alu", "SHIFT L NO CARRYOUT");
		REASON("Wrong answer");
		rc = 1;
	}

	// TEST SHIFT L W/ CARRYOUT
	alu_in1 = 0xffff;
	alu_in2 = 0;
	alu_res = 0;
	carryout = 0;
	ctrl->alu_op = 6;

	if (exec_alu(alu_in1, alu_in2, &alu_res, &carryout, ctrl) != 0) {
		TEST_FAILED("exec_alu", "SHIFT L W/ CARRYOUT");
		REASON("Non-Zero return code");
		rc = 1;
	} else if (alu_res == (uint16_t)(alu_in1 << 1) && carryout == 1) {
		TEST_PASSED("exec_alu", "SHIFT L W/ CARRYOUT");
	} else {
		TEST_FAILED("exec_alu", "SHIFT L W/ CARRYOUT");
		REASON("Wrong answer");
		rc = 1;
	}

	return rc;
}
