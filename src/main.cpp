/*
 * main.cpp
 *
 * Travis Banken
 * 2020
 * 
 * Start point for the chip8 emulator
 */

#include <iostream>

#include <mem.h>

int main(int argc, char **argv)
{
    std::cout << "Hello, Chip8!\n";
    Mem mem = Mem();
    for (int i = 0; i < 0x1234; i++) {
    	mem.write(0x42, i);
    }
    mem.dump();
}