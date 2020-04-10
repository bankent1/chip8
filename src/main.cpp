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


static void sighandler(int sig);
void exithandler(int rc, void *arg);

int main(int argc, char **argv)
{
	if (argc < 2) {
		std::cerr << "No program specified!\n";
		return 1;
	}

	// setup sighandler
	sighandler_t res = signal(SIGINT, sighandler);
	assert(res != SIG_ERR);

	char *filename = argv[1];
	
    std::cout << "Hello, Chip8!\n";
    Chip8 chip8 = Chip8(filename);

    // setup exit handler
	on_exit(exithandler, (void*)&chip8);


    chip8.run();
    // while (1) {
    // 	chip8.step();
    // 	printf("Press ENTER to continue...\n");
    // 	getchar();
    // }
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
