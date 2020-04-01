/*
 * chip8.cpp
 */

/*
OPCODES
0NNN -- Call RCA 1802 program at addr NNN. Not necessary for most ROMs
00E0 -- return from subroutine
1NNN -- jmp to adr NNN
2NNN -- call subroutine at NNN
3XNN -- skip next instr if VX == NN
4XNN -- skip next instr if VX != NN
5XY0 -- skip next instr if VX == VY
6XNN -- set VX to NN
7XNN -- Add NN to VX (carry flag not changed)

8XY0 -- set VX == VY
8XY1 -- VX = VX | VY
8XY2 -- VX = VX & VY
8XY3 -- VX = VX ^ VY
8XY4 -- VX = VX + VY (VF set to 1 if carry out, 0 if not)
8XY5 -- VX = VX - VY (VF set to 0 if borrow, 1 if not)
8XY6 -- VX = VX >> 1 (Store least sig bit of VX in VF before shift)
8XY7 -- VX = VY - VX (VF set to 0 if  borrow, 1 if not)
8XY8 -- VX = VX << 1 (Store most sig bit of VX in VF before shift)

9XY0 -- skip next isntr if VX != VY
ANNN -- set I to addr NNN
BNNN -- Jmp to addr NNN + V0
CXNN -- VX = rand() & NN
DXYN -- Draw sprite at coordinate (VX,VY) with width 8 pixels and height N pixels

EX9E -- Skip next instr if key stored in VX is pressed
EXA1 -- Skip next instr if key stored in VX isn't pressed

FX07 -- Set VX to the value of the delay timer.
FX0A -- Key press is awaited, then stored in VX
FX15 -- Sets the delay timer to VX
FX18 -- Sets the sound timer to VX
FX1E -- Adds VX to I. VF is set to 1 when there is a range overflow (I+VX > 0xFFF),
        and 0 otherwise
FX29 -- Sets I to the location of the sprite for the character in VX. Characters
        0-F are represented by a 4x5 font
FX33 -- take the decimal representation of VX, place the hundreds digit in memory
        at location in I, the tens digit at location I+1, and the ones digit at 
        location I+2
FX55 -- Store V0 to VX (inclusive) in mem starting at addr I.
FX65 -- Fill V0 to VX (inclusive) in mem starting at addr I.
*/

#include <chip8.h>

