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
#include <csignal>
#include <cassert>
#include <cstdlib>
#include <cstring>


static void sighandler(int sig);
void exithandler(int rc, void *arg);

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "No program specified!\n";
		return 1;
	}

	// TODO replace with getopts later (if more options)
	bool step = false;
	char *filename = NULL;
	if (argc == 2) {
		filename = argv[1];
		step = false;
	} else if (strcmp(argv[1], "-s") == 0) {
		filename = argv[2];
		step = true;
	} else if (strcmp(argv[2], "-s") == 0) {
		filename = argv[1];
		step = true;
	}
	if (filename == NULL) {
		std::cerr << "No program specified!\n";
		return 1;
	}

	// setup sighandler
	sighandler_t res = signal(SIGINT, sighandler);
	assert(res != SIG_ERR);

	
    std::cout << "Hello, Chip8!\n";
    Chip8 chip8 = Chip8(filename);

    // setup exit handler
	on_exit(exithandler, (void*)&chip8);

	// check if in step mode
    if (step) {
	    while (1) {
	    	chip8.step();
	    	printf("Press ENTER to continue...\n");
	    	getchar();
	    }
    } else {
	    chip8.run();
    }
}

static void sighandler(int sig)
{
	if (sig == SIGINT)
		std::exit(1);
}

void exithandler(int rc, void *arg)
{
	Chip8 *chip8 = (Chip8*) arg;
	if (rc == 1)
		chip8->dump();
}
