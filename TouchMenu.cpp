#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>

UTFT Display(ITDB43, 25, 26, 27, 28);	//	start an instance of the UTFT class using the display model and the pins used



TouchMenu::TouchMenu(byte init)
{
}

void TouchMenu::MainMenu(byte enter)
{
	if (enter == 1)
	{
		Display.fillScr(VGA_SILVER);
		enter = 0;
		delay(500);
	}
	else
	{
		Display.fillScr(VGA_GREEN);
		delay(500);
	}

	Display.fillScr(VGA_BLUE);

}

void TouchMenu::UserSetup()
{
}

void TouchMenu::TempType()
{
}

void TouchMenu::TempPrecision()
{
}

void TouchMenu::TempReadDelay()
{
}

void TouchMenu::Brightness()
{
}

void TouchMenu::TimeFormat()
{
}

void TouchMenu::SecondsDisplay()
{
}

void TouchMenu::FlowOnOff()
{
}

void TouchMenu::TimerSetup()
{
}

void TouchMenu::SetTimer(short numbertoset)
{
}

void TouchMenu::SensorSetup()
{
}

void TouchMenu::TempSensorCalib(byte numbertocalib)
{
}

void TouchMenu::FlowSensorCalib()
{
}

void TouchMenu::SensorAddrConfig()
{
}

void TouchMenu::SystemSetup()
{
}

void TouchMenu::SerialDebugging()
{
}

void TouchMenu::EraseEEPROM()
{
}

void TouchMenu::RestorDefaults()
{
}
