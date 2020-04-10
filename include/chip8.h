#ifndef _CHIP8_H
#define _CHIP8_H

#include <cstdint>
#include <mem.h>
#include <periphs.h>
#include <string>

#define NUM_INSTR 35
#define NUM_OPS 16

typedef struct Instr {
    uint16_t raw;
    uint8_t op;
    uint16_t nnn;
    uint8_t nn;
    uint8_t n;
    uint8_t vx;
    uint8_t vy;
} Instr;


class Chip8 {
    typedef void(Chip8::*OpFunction)(Instr);
private:
    uint16_t I;
    uint16_t pc;
    uint8_t V[16] = {0};
    Mem mem;
    Periphs periphs;
    OpFunction opfuncs[NUM_OPS] = {0}; // use first byte to index into arr

    void load_program(const std::string program);

    // op code fn go here
    void nop();
    void op0(Instr instr);
    void op1(Instr instr);
    void op2(Instr instr);
    void op3(Instr instr);
    void op4(Instr instr);
    void op5(Instr instr);
    void op6(Instr instr);
    void op7(Instr instr);
    void op8(Instr instr);
    void op9(Instr instr);
    void opA(Instr instr);
    void opB(Instr instr);
    void opC(Instr instr);
    void opD(Instr instr);
    void opE(Instr instr);
    void opF(Instr instr);

public:
    Chip8(const std::string program);
    void step();
    void run();
    void dump();
};


#endif