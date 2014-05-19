/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Mega 2560 or Mega ADK, Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define _VMDEBUG 1
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

void setup(void);
//
void AlarmAON();
void AlarmAOFF();
void RL_Toggle();
void START_SCREEN();
void LCD_Time_Display();
void Display_Date();
void DS18B20_Read();
void printAddress(DeviceAddress deviceAddress);
void writeEEPROM(int address, byte data);
byte readEEPROM(int address);
void factoryDefaultset();
void MenuButtonPress();
void MenuTitle();
void MenuLines();
void MenuLoop();
void MenuUp();
void MenuDown();
void MenuSelect();
void MenuBack();
void MenuDo();
void MenuBinary();

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "D:\Users\Robbie\My Documents\GitHub\Controller\LCD_Shield_with_Temperature.ino"
#include "D:\Users\Robbie\My Documents\GitHub\Controller\Menu_System.ino"
