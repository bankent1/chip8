#ifndef _BUS_H
#define _BUS_H

#include <cstdint>


class Bus {
private:
	std::list<Device> devs;

public:
	void write(uint8_t byte, uint16_t mem);
	uint8_t read(uint16_t addr);
	void connect(Device& dev);
	bool disconnect(Device& dev);
};

#endif