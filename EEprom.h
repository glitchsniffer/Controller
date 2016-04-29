#ifndef EEprom_h
#define EEprom_h

#include "Arduino.h"

class EEprom
{
public:
	void eeprom(int DEVICE_ADDRESS);
	bool write(uint16_t address, byte data, byte debug);
	byte read(uint16_t address, byte debug);
	void eraseAll(byte debug);
	void readAll(byte debug);
	int _DEVICE_ADDRESS;
private:
	
};

#endif	//	!EEprom_h