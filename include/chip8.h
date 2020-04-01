#ifndef _CHIP8_H
#define _CHIP8_H

#include <cstdint>

#define NUM_OPCODES 60 // TODO find real val

typedef struct Instr {
    uint8_t op;
    uint16_t nnn;
    uint8_t vx;
    uint8_t vy;
} Instr;

class Chip8 {
private:
    uint16_t I;
    uint16_t pc;
    uint8_t V[16];
    // (void)(*fn)(Instr) opfn[NUM_OPCODES]; // use first byte to index into arr

    // op code fn go here


public:
    void step();
    void run();
};

#endif