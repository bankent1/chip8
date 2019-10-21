/*
 * testall.c
 * 
 * Author: Travis Banken
 * 
 * Runs all the tests
 */

#include "tests/tests.h"

int main(int argc, char **argv)
{
    int rc = 0;

    if (runall() != 0)
        rc = 1;

    return rc;
}

int runall()
{
    int rc = 0;

    if (test_exec_alu() != 0) 
        rc = 1;

    return rc;
}