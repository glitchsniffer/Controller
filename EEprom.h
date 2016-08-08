#ifndef EEprom_h
#define EEprom_h

#include "Arduino.h"

extern byte serialDebug;

class EEprom
{
public:
	EEprom(int addr);
	bool write(int address, byte data);
	byte read(int address);
	void eraseAll();
	void readAll();
	int _Addr;
private:
	
};

#endif	//	!EEprom_h