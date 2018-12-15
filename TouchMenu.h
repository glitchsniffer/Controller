#ifndef TouchMenu_h
#define TouchMenu_h

#include <UTFT.h>
#include <UTouch.h>
#include <SPI.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include "Arduino.h"
#include "EEprom.h"
#include "TimeAlarms.h"

extern UTFT TFT;		//	transfers the TFT instance to the touch menu
extern UTouch Touch;	//	transfers the Touch instance to the touch menu
extern EEprom eeprom;	//	transfers the EEprom instance to the touch menu


extern uint8_t GroteskBold16x32[];	//	make the font availiable to use
extern uint8_t GroteskBold24x48[];	//	make the font availiable to use
extern uint8_t Retro8x16[];			//	make the font availiable to use
extern unsigned short arrow_up[0x400];		//	make the icon availiable for use
extern unsigned short arrow_down[0x400];	//	make the icon availiable for use
extern unsigned short arrow_left[0x400];	//	make the icon availiable for use
extern unsigned short arrow_right[0x400];	//	make the icon availiable for use

extern uint8_t today;	//	variable used to force the date to be displayed
extern uint8_t RTC_Status;	//	global variable from Controller for the RTC status
extern uint16_t menuTimeout;

extern AlarmID_t tempReadID;
extern AlarmID_t flowReadID;
extern uint16_t flowRateMax;
extern uint16_t flowPulseTotal;
extern uint16_t flowRateMax;
extern uint8_t timeFormat;

void factoryDefaultset();
void FlowSensorRead();
//String TimeString(byte disp, uint8_t hour, uint8_t min, uint8_t sec);


class TouchMenu
{
public:
	TouchMenu();			//	function to start an instance of the class
	void EnterMenu();			//	function to enter the main menu
		//void UserSetup();
		//	void TempType();
		//	void TempPrecision();
		//	void TempReadDelay();
		//	void Brightness();
		//	void TimeFormat();
		//	void SecondsDisplay();
		//	void FlowOnOff();
//		void TimerSetup();
//			void SetTimer(short numbertoset);
//		void SensorSetup();
//			void TempSensorCalib(byte numbertocalib);
//			void FlowSensorCalib();
//			void SensorAddrConfig();
//		void SystemSetup();
//			void SerialDebugging();
//			void EraseEEPROM();
//			void RestorDefaults();
//		void DrawMenuButtonArray(uint8_t buttons);
//		void MenuLoop();
};
#endif	//	!TouchMenu