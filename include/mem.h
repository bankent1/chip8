#ifndef _MEM_H
#define _MEM_H

#include <device.h>

class Mem : public Device {
private:
	uint8_t mem[1024*4] = {0}; // 4 MB

	void write_font();

public:
	Mem();
	void write(uint8_t data, uint16_t addr);
	uint8_t read(uint16_t addr);
	void dump();
};

#endif