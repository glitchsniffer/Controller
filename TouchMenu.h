#ifndef TouchMenu_h
#define TouchMenu_h

#include <UTFT.h>
#include <UTouch.h>
#include <SPI.h>

#include "Arduino.h"

extern UTFT TFT;		//	transfers the TFT instance to the touch menu
extern UTouch Touch;	//	transfers the Touch instance to the touch men

extern uint8_t GroteskBold16x32[];	//	make the font availiable to use
extern uint8_t GroteskBold24x48[];	//	make the font availiable to use

extern uint8_t today;	//	variable used to force the date to be displayed
extern volatile int8_t menuMode;
extern uint16_t menuTimeout;

class TouchMenu
{
public:
	TouchMenu(void);			//	function to start an instance of the class
	void MainMenu();		//	function to enter the main menu

//private:
//		void UserSetup(void);
//			void TempType();
//			void TempPrecision();
//			void TempReadDelay();
//			void Brightness();
//			void TimeFormat();
//			void SecondsDisplay();
//			void FlowOnOff();
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