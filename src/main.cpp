/*
 * main.cpp
 *
 * Travis Banken
 * 2020
 * 
 * Start point for the chip8 emulator
 */

#include <iostream>
#include <fstream>
#include <mem.h>
#include <chip8.h>
#include <periphs.h>
#include <clogs.h>

int log_limit;
int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "No program specified!\n";
		return 1;
	}

	// TODO set log limit from cmdargs
	log_limit = 10;
	char *filename = argv[1];
	std::ifstream program;
	program.open(filename, std::ios::binary | std::ios::in);
	if (!program.is_open()) {
		std::cerr << "Failed to open file " << filename << std::endl;
		return 1;
	}
	
    std::cout << "Hello, Chip8!\n";
    Chip8 chip8 = Chip8(program);

    Periphs peri = Periphs("This is a test Chip8!");

    // chip8.run();
    // chip8.dump();

    program.close();
}