#ifndef _DEVICE_H
#define _DEVICE_H

// abstract class
class Device {
public:
	virtual void write(uint8_t data, uint16_t addr) = 0;
	virtual uint8_t read(uint16_t addr) = 0;
};

#endif