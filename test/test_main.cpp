/*
 * test_main.cpp
 *
 * Travis Banken
 * 2020
 *
 * Main file for running tests on the chip8
 */

#include <iostream>

#include "test_mem.h"

int main()
{
    bool all_passed = true;
    std::cout << "---------------------------------------------\n";
    std::cout << "Running Memory tests...\n";
    std::cout << "---------------------------------------------\n";
    all_passed = all_passed && test_mem::run_all();
    std::cout << "---------------------------------------------\n";
    return !all_passed;
}