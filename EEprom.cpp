#include "EEprom.h"
#include "Wire.h"



void EEprom::eeprom(int DEVICE_ADDRESS)
{
	int _DEVICE_ADDRESS = DEVICE_ADDRESS;
	Wire.begin();
}

bool EEprom::write(uint16_t address, byte data, byte debug)
{
	byte bytewritten;
	Wire.beginTransmission(_DEVICE_ADDRESS);			//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.write(data);							//  writes the byte data to the EEPROM at the address specified above
	Wire.endTransmission();						//  stops the write communication on the I2C
	delay(10);

	if ((debug & 8) == 8)						//	if serialDebug is on for eeprom read/writes print to the serial console
	{
		Serial.print("Write ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(data);
	}

	bytewritten = read(address, debug);			//	read the value that was written to the eeprom back out
	if (bytewritten == data){ return true; }	//	compare the value written to the value that was written to the eeprom and return the bool
	else false;
}

byte EEprom::read(uint16_t address, byte debug)
{
	byte result;								//  returned value
	Wire.beginTransmission(_DEVICE_ADDRESS);			//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.endTransmission(); 					//  stops the write communication on the I2C
	Wire.requestFrom(_DEVICE_ADDRESS,1);	//  gets 1 byte of data from the device
	result = Wire.read();						//  sets the value read to data

	if ((debug & 8) == 8)						//	if serialDebug is on for eeprom read/writes print to the serial console
	{
		Serial.print("Read ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(result);
	}

	return result;								//  returns data to the previous call
}

void EEprom::eraseAll(byte debug)
{
	Serial.println("Erasing EE2");
	uint16_t address = 0;
	byte data = 0;
	while (address < 256)
	{
		write(address, 0, debug);
		address++;
	}
}

void EEprom::readAll(byte debug)
{
	Serial.println("Reading EE2");
	uint16_t address = 0;
	while (address < 256)
	{
		read(address, debug);
		address++;
	}
}
