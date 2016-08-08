#include "EEprom.h"
#include "Wire.h"

EEprom::EEprom(int addr)
//	initialization function to start the class and set the device address of the eeprom
//	addr = the I2C device address of the eeprom
{
	_Addr = addr;
}

bool EEprom::write(int address, byte data)
//	function to write a byte to the eeprom
//	address = is the address to write to in the EEprom
//	data = is the data byte to write to the eeprom

{
	byte bytewritten;
	Wire.beginTransmission(_Addr);				//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.write(data);							//  writes the byte data to the EEPROM at the address specified above
	Wire.endTransmission();						//  stops the write communication on the I2C
	delay(10);

	if ((serialDebug & 8) == 8)					//	if serialDebug is on for eeprom read/writes print to the serial console
	{
		Serial.print("Write ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(data);
	}

	bytewritten = read(address);				//	read the value that was written to the eeprom back out
	if (bytewritten == data){ return true; }	//	compare the value written to the value that was written to the eeprom and return the bool
	else false;
}

byte EEprom::read(int address)
//	function to read out a byte from the eeprom
//	address = the address to read from in the eeprom

{
	byte result;								//  returned value
	Wire.beginTransmission(_Addr);				//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.endTransmission(); 					//  stops the write communication on the I2C
	Wire.requestFrom(_Addr, 1);					//  gets 1 byte of data from the device
	result = Wire.read();						//  sets the value read to data

	if ((serialDebug & 8) == 8)
	{
		Serial.print("Read ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(result);
	}

	return result;								//  returns data to the previous call
}

void EEprom::eraseAll()
//	function to erase all of the eeprom

{
	Serial.println("Erasing EE2");
	uint16_t address = 0;
	byte data = 0;
	while (address < 256)
	{
		write(address, 0);
		address++;
	}
}

void EEprom::readAll()
//	function to read out all of the eeprom in one sweep. this will not return anything.
{
	Serial.println("Reading EE2");
	uint16_t address = 0;
	while (address < 256)
	{
		read(address);
		address++;
	}
}
