/*
 * tests.h
 * 
 * Header file for all the entire test suite
 */

#ifndef TESTS_H
#define TESTS_H

#define TEST_FAILED(function, testname) printf("Test [%s] [%s]\t\tFAILED\n", function, testname)
#define TEST_PASSED(function, testname) printf("Test [%s] [%s]\t\tPASSED\n", function, testname)
#define REASON(reason) printf("        --> %s\n", reason)

// testall.c
int runall();

// test-phases.c
int test_exec_alu();

#endif