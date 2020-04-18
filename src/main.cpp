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
#include <cstdio>
#include <getopt.h>

#define MAX_CLOCK_SPEED 10
#define DEFAULT_CLOCK_SPEED 3
#define MAX_PIXEL_SCALE 32
#define DEFAULT_PIXEL_SCALE 16

static void sighandler(int sig);
static void exithandler(int rc, void *arg);
static void print_usage();

int main(int argc, char **argv)
{
    // *** start handle args ***
    bool step = false;
    uint clock_speed = DEFAULT_CLOCK_SPEED;
    uint pixel_scale = DEFAULT_PIXEL_SCALE;
    bool max_clock = false;
    char *filename = NULL;
    const char* const short_opts = "sc:p:mh";
    const option long_opts[] = {
        {"step", no_argument, nullptr, 's'},
        {"clock-speed", no_argument, nullptr, 'c'},
        {"pixel-scale", required_argument, nullptr, 'p'},
        {"help", no_argument, nullptr, 'h'},
        {"max-clock", no_argument, nullptr, 'm'}
    };
    while (1) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        if (-1 == opt)
            break;

        switch (opt) {
        case 's':
            step = true;
            break;
        case 'c':
            clock_speed = (uint)std::stoi(optarg);
            if (clock_speed > MAX_CLOCK_SPEED) {
                std::cerr << "Error: Invalid clock speed!\n";
                print_usage();
                return 1;
            }
            break;
        case 'p':
            pixel_scale = (uint)std::stoi(optarg);
            if (pixel_scale > MAX_PIXEL_SCALE) {
                std::cerr << "Error: Invalid pixel scale value!\n";
                print_usage();
                return 1;
            }
            break;
        case 'm':
            max_clock = true;
            break;
        case 'h':
            print_usage();
            return 0;
        case '?':
            print_usage();
            return 1;
        }
    }
    if (optind >= argc) {
        std::cerr << "Error: No rom-path provided!\n";
        print_usage();
        return 1;
    }
    filename = argv[optind];
    // *** end processing args ***

    std::clog << "-----------------------------------------\n";
    std::clog << "*** Settings ***\n";
    std::clog << "-----------------------------------------\n";
    std::clog << "Rom Path   : " << filename << std::endl;
    std::clog << "Pixel Scale: " << pixel_scale << std::endl;
    std::clog << "Clock Speed: " << clock_speed << std::endl;
    std::clog << "Step Mode  : " << (step ? "ON\n" : "OFF\n");
    std::clog << "Max Clock  : " << (step ? "TRUE\n" : "FALSE\n");
    std::clog << "-----------------------------------------\n";

    // setup sighandler
    sighandler_t res = signal(SIGINT, sighandler);
    assert(res != SIG_ERR);

    Chip8 chip8 = Chip8(filename, pixel_scale, clock_speed, max_clock);

    // setup exit handler
    on_exit(exithandler, (void*)&chip8);

    std::clog << "Starting Chip8...\n";
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

static void exithandler(int rc, void *arg)
{
    Chip8 *chip8 = (Chip8*) arg;
    if (rc != 0)
        chip8->dump();
}

static void print_usage()
{
    printf("Usage: chip8 [OPTIONS] <path-to-rom>\n");
    printf("Emulates a chip8 processor using the provided rom file.\n");
    printf("\n");
    printf("EXAMPLE:\n");
    printf("    chip8 -p 4 --clock-speed 5 ROMPATH -s\n");
    printf("\n");
    printf("OPTIONS:\n");
    printf("    -c, --clock-speed       Lets the user modify the clock rate of the chip8\n");
    printf("                            The speeds range from 0 to %d and modify how\n", MAX_CLOCK_SPEED);
    printf("                            quickly each instruction is executed in real time.\n");
    printf("                            The default value is %d\n", DEFAULT_CLOCK_SPEED);
    printf("    -m, --max-clock         The processor will run as fast as it can. Note that\n");
    printf("                            this will override the clock-speed option. Also note\n");
    printf("                            that this is faster than the max value of clock-speed.\n");
    printf("    -p, --pixel-scale       Sets the resolution scale, the default being %d.\n", DEFAULT_PIXEL_SCALE);
    printf("                            Adjust this to make the screen larger or smaller.\n");
    printf("                            Max value is %d\n", MAX_PIXEL_SCALE);
    printf("    -s, --step              When set the emulator will run in step mode.\n");
    printf("                            In step mode, the instruction will only be\n");
    printf("                            executed after ENTER key is pressed.\n");
    printf("    -h, --help              Display this usage message and exit\n");
}