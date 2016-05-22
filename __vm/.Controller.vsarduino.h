/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Due (Programming Port), Platform=sam, Package=arduino
*/

#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
#define printf iprintf
#define F_CPU 84000000L
#define ARDUINO 10608
#define ARDUINO_SAM_DUE
#define ARDUINO_ARCH_SAM
#define __SAM3X8E__
#define USB_VID 0x2341
#define USB_PID 0x003e
#define USBCON
#define __cplusplus 201103L
#define __ARM__
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __volatile__
#define __SIZE_TYPE__ long

#define __ICCARM__
#define __ASM
#define __INLINE
#define __builtin_va_list void
//#define _GNU_SOURCE 
//#define __GNUC__ 0
//#undef  __ICCARM__
//#define __GNU__

typedef long Pio;
typedef long Efc;
typedef long Adc;
typedef long Pwm;
typedef long Rtc;
typedef long Rtt;
typedef long pRtc;
typedef long Spi;
typedef long spi;
typedef long Ssc;
//typedef long p_scc;
typedef long Tc;
//typedef long pTc;
typedef long Twi;
typedef long Wdt;
//typedef long pTwi;
typedef long Usart;
typedef long Pdc;
typedef long Rstc;

extern const int ADC_MR_TRGEN_DIS = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG0 = 0;
extern const int ADC_MR_TRGSEL_Pos = 0;

extern const int ADC_MR_TRGSEL_Msk = 0;
extern const int ADC_MR_TRGEN = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG1 = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG2 = 0;
extern const int ADC_MR_TRGSEL_ADC_TRIG3 = 0;



#define __ARMCC_VERSION 400678
#define __attribute__(noinline)

#define prog_void
#define PGM_VOID_P int


typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}



#include <arduino.h>
#include <pins_arduino.h> 
#include <variant.h> 
#undef F
#define F(string_literal) ((const PROGMEM char *)(string_literal))
#undef PSTR
#define PSTR(string_literal) ((const PROGMEM char *)(string_literal))
#undef cli
#define cli()
#define pgm_read_byte(address_short)
#define pgm_read_word(address_short)
#define pgm_read_word2(address_short)
#define digitalPinToPort(P)
#define digitalPinToBitMask(P) 
#define digitalPinToTimer(P)
#define analogInPinToBit(P)
#define portOutputRegister(P)
#define portInputRegister(P)
#define portModeRegister(P)

//
//
void AlarmON();
void AlarmOFF();
void RelayToggleALL();
void RelayToggle(uint8_t relay, uint8_t onoff);
void RelayStatusDisplay(uint8_t col, uint8_t row);
void StartScreen();
void LCDTimeDisplay(byte disp, uint8_t col, uint8_t row, uint8_t hour, uint8_t min, uint8_t sec, uint8_t space);
void LCDDateDisplay(byte display, uint8_t col, uint8_t row);
void ReadTempSensors();
String convertTempSensorAddress(DeviceAddress deviceAddress);
void logger();
void FlowSensorRead();
void FlowSensorCounter();
void MenuButtonPress();
void factoryDefaultset();
uint16_t freeRam();
byte decToBcd(byte val);
byte bcdToDec(byte val);
void error(char*str);
void TestSDLEDS();
void AlarmSet(byte id);
void AlarmSetDisplay(uint8_t id);
void MenuTitle();
void MenuLoop();
void MenuUp();
void MenuDown();
void MenuSelect();
void MenuBack();
void MenuDo();
uint16_t MenuNumSel(uint16_t type, uint16_t addr, uint16_t start, uint16_t min, uint16_t max, uint16_t step, uint16_t col, uint16_t row, uint16_t dmicro);

#include <Controller.ino>
#include <Alarms.ino>
#include <EEprom.cpp>
#include <EEprom.h>
#include <GroteskBold16x32.c>
#include <GroteskBold24x48.c>
#include <MCPExpander.cpp>
#include <MCPExpander.h>
#include <Menu_System.ino>
#endif
