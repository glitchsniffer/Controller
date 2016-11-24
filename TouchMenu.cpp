#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>

TouchMenu::TouchMenu()
{
	//	only here to initialize the class
}

void TouchMenu::MainMenu()
{
	TFT.fillScr(VGA_NAVY);
	TFT.setBackColor(VGA_NAVY);
	TFT.setFont(GroteskBold24x48);
	TFT.print("MAIN MENU", CENTER, 10, 0);
	DrawMenuButtonArray(8);
	today = 0;	//  set today to 0 so that the date function gets called
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

void TouchMenu::DrawMenuButtonArray(uint8_t maxbuttons) {
	//	This function will draw a button array depending on the number of buttons
	//	maxbuttons is the total number of buttons needed

	//	initalize variables
	uint8_t xs;					//	horizontal start position
	uint8_t ys = 68;			//	vertical start position
	uint8_t width = 210;		//	button width
	uint8_t height = 40;		//	button height
	uint8_t xspace = 20;		//	horizontal space between buttons
	uint8_t yspace = 8;			//	vertical space between buttons
	uint8_t columns;			//	number of columns that will be needed
	uint8_t buttoncount = 0;	//	counter for the number of buttons printed

	//	determine you need 1 or 2 columns of buttons depending on the size of max buttons
	//	and set the horizontal start position
	if (maxbuttons <= 4) {
		xs = 135;
		columns = 1;
	}
	else {
		xs = 20;
		columns = 2;
	}

	//	loop through and start printing the buttons up to maxbuttons
	for (uint8_t r = 0; r < 4; r++) {
		if (buttoncount == maxbuttons) { break; }
		uint16_t rowstart = ys + (r * (height + yspace));

		for (uint8_t c = 0; c < columns; c++) {
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(xs + (c * (width + xspace)), rowstart, xs + width + (c * (width + xspace)), rowstart + height);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(xs + (c * (width + xspace)), rowstart, xs + width + (c * (width + xspace)), rowstart + height);
			buttoncount++;
			Serial.println(buttoncount);

			if (buttoncount == maxbuttons) { break; }
		}
	}
	delay(5000);	//	small delay to continue showing the buttons
}
