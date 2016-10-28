#ifndef TouchMenu_h
#define TouchMenu_h

#include <UTFT.h>

#include <UTouch.h>
#include <SPI.h>

#include "Arduino.h"

class TouchMenu
{
public:
	TouchMenu(byte init);			//	function to start an instance of the class
	void MainMenu(byte enter);		//	function to enter the main menu


private:
		void UserSetup();
			void TempType();
			void TempPrecision();
			void TempReadDelay();
			void Brightness();
			void TimeFormat();
			void SecondsDisplay();
			void FlowOnOff();
		void TimerSetup();
			void SetTimer(short numbertoset);
		void SensorSetup();
			void TempSensorCalib(byte numbertocalib);
			void FlowSensorCalib();
			void SensorAddrConfig();
		void SystemSetup();
			void SerialDebugging();
			void EraseEEPROM();
			void RestorDefaults();
};
#endif	//	!TouchMenu