#ifndef EEprom_h
#define EEprom_h

#include "Arduino.h"

extern byte serialDebug;

class EEprom
{
public:
	EEprom(uint8_t addr);
	bool write(uint16_t address, byte data);
	byte read(uint16_t address);
	void eraseAll();
	void readAll();
	uint16_t _Addr;
private:
	
};

#endif	//	!EEprom_h