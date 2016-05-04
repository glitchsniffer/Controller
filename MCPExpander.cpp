#include "MCPExpander.h"
#include "Wire.h"

MCPExpander::MCPExpander(byte address)
//	initialization function to start the class and set the device address of the eeprom
//	addr = the I2C device address of the eeprom
{
	_Addr = address;
}

void MCPExpander::writeByte(byte reg, byte data)
{
	//  Send data to the address and register of the I2C device
	Wire.beginTransmission(_Addr);    //  begin the transmission of data
	Wire.write(reg);                    //  write to the specific register
	Wire.write(data);                   //  write the data byte
	Wire.endTransmission();             //  end the transmission of the data
}

byte MCPExpander::readByte(byte reg)
{
	uint8_t data = 0;

	//  Start by sending the register address
	Wire.beginTransmission(_Addr);
	Wire.write(reg);
	Wire.endTransmission();

	//  request 1 bytes from the device
	Wire.requestFrom(_Addr, 1);		//	initiate the request for , x bytes from the I2C device
	if (!Wire.available()) {}			//  wait until data is received
	data = Wire.read();					//	write the bytes to data once it is done receiving

	return data;						//	return the data as a byte
}

byte MCPExpander::writeBit(byte bank, byte port, byte state)
{
	//  address = the address of the MCP to write to
	//  bank = is the GPIO bank A or B that you want to write to
	//  port = is the port on the bank that you want to write to 0 - 7
	//  state = is the bit you want to write to that port 0 or 1

	//  will return the last reading from the port

	uint8_t read = 0;

	//  read the current setting IO port bank
	if (bank == 0)
	{
		read = readByte(GPIOA);						//	read the GPIO ports from the MCP
		writeByte(GPIOA, read ^ (1 << port));		//	write the specific port back to the MCP either turning it off
		read = readByte(GPIOB);						//	read back the GPIO port to be able to print out what the final result is
	}
	else if (bank == 1)
	{
		read = readByte(GPIOB);						//	read the GPIO ports from the MCP
		writeByte(GPIOB, read ^ (1 << port));		//	write the specific port back to the MCP either turning it on
		read = readByte(GPIOB);						//	read back the GPIO port to be able to print out what the final result is
	}
	else { Serial.println("Incorrect bank selection"); }
	
	return read;		//	return what was the final read was
}

byte MCPExpander::readBit(byte bank, byte port)
{
	//  address = the address of the MCP to read from
	//  bank = is the GPIO bank A or B that you want to read from
	//  port = is the port on the bank that you want to read from to 0 - 7

	//	will return the current bit of the port selected as a 1 or 0

	uint8_t read = 0;

	//  read the current setting IO port bank
	if (bank == 0)
	{
		read = readByte(GPIOA);
	}
	else if (bank == 1)
	{
		read = readByte(GPIOB);
	}
	else { Serial.println("Incorrect bank selection"); }

	//	this will take the reading and shift it in and out to isolate the bit

	read = (read ^ (1 << port));
	read = (read << (7 - port));
	read = (read >> 7);

	//	evaluate the bit selected and return the proper value
	switch (read)
	{
	case 0:
		return 0;
	case 1:
		return 1;
	default:
		return 2;
	}
}
