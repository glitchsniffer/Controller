#ifndef MCPExpander_h
#define MCPExpander_h

#include "Arduino.h"
#include <Wire.h>

#define MCP20017 18
#define MCP20008 8

// MCP2308 or 17 Control Registers  IOCON BANK set to 1
#define IOCON 0x0A		//	IOCON register address before switching it to BANK = 1 to allow the same registers to be used for the 23008 and 23017
#define IODIRA 0x00		//  I/O Direction register 1 = input, 0 = output
#define IODIRB 0x10		//  I/O Direction register 1 = input, 0 = output
#define IPOLA 0x01		//  Input Polarity register 1 = GPIO register bit will be inverted from the input pin.  0 = non inverted.
#define IPOLB 0x11		//  Input Polarity register 1 = GPIO register bit will be inverted from the input pin.  0 = non inverted.
#define GPINTENA 0x02	//	Interrupt on change control register 1 = enable 0 = disable GPIO input pin for interrupt
#define GPINTENB 0x12	//	Interrupt on change control register 1 = enable 0 = disable GPIO input pin for interrupt
#define DEFVALA 0x03	//	Default compare register for int on change. Defines the default value of the interrupts.
#define DEFVALB 0x13	//	Default compare register for int on change. Defines the default value of the interrupts.
#define INTCONA 0x04	//	Interrupt Control register 1 = pin is compared to DEFVAL 0 = pin is compared to the previous value
#define INTCONB 0x14	//	Interrupt Control register 1 = pin is compared to DEFVAL 0 = pin is compared to the previous value
#define IOCONA 0x05		//	bit2: 1=Open drain output 0=active driver output(INTPOL sets the polarity. bit1: 1=active high 0=active low for the interrupt pin
#define IOCONB 0x15		//	bit2: 1=Open drain output 0=active driver output(INTPOL sets the polarity. bit1: 1=active high 0=active low for the interrupt pin
#define GPPUA 0x06		//	Pull-up Resistor configuration.  1 = PU enabled, 0 = PU disabled.  100k
#define GPPUB 0x16		//	Pull-up Resistor configuration.  1 = PU enabled, 0 = PU disabled.  100k
#define INTFA 0x07		//	Interrupt Flag register. read only. 1 = pin caused interrupt, 0 = interrupt not pending
#define INTFB 0x17		//	Interrupt Flag register. read only. 1 = pin caused interrupt, 0 = interrupt not pending
#define INTCAPA 0x08	//	Interrupt catpure register.  captures the GPIO port value when an interrupt occures.  1=high, 0=low
#define INTCAPB 0x18	//	Interrupt catpure register.  captures the GPIO port value when an interrupt occures.  1=high, 0=low
#define GPIOA 0x09		//	Port GPIO register.  Writing 1=high, 0=low modifies the OLAT register
#define GPIOB 0x19		//	Port GPIO register.  Writing 1=high, 0=low modifies the OLAT register
#define OLATA 0x0A		//	Output Latch register.  1=high, 0=low writing to this register modifies the output latches.  reading, reads the output latches.
#define OLATB 0x1A		//	Output Latch register.  1=high, 0=low writing to this register modifies the output latches.  reading, reads the output latches.


class MCPExpander
{
public:
	MCPExpander(byte address);
	void writeByte(byte reg, byte data);
	byte readByte(byte reg);
	byte writeBit(byte bank, byte port, byte state);
	byte readBit(byte bank, byte port);

	uint16_t _Addr;

private:

};

#endif // !MCPExpander_h