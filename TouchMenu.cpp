#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>

//  ***********************************************
//  Variables
//  ***********************************************
uint8_t TotalButtons;
uint16_t Touchx;
uint16_t Touchy;

//  ***********************************************
//  Function prototypes
//  ***********************************************
		void UserSetup(void);
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
		void DrawMenuButtonArray(uint8_t buttons);
		void MenuLoop();
		void ReadTouchData();
		void AnalyzeTouchData();


//  ***********************************************
//  Menu Array Definitions
//  ***********************************************
typedef struct MENU_ITEM {
	byte Menu_Number;
	char Menu_Name[20];
	byte Menu_Size;
	void (*function)(void);
};

//  ***********************************************
//  Menu Array Definitions
//  ***********************************************
typedef struct TOUCH_LOC_8 {
	int Button_Number;
	int X_Start;
	int Y_Start;
	int X_End;
	int Y_End;
};

//  ***********************************************
//  Menu Arrays
//  ***********************************************

//	Main Menu Array
MENU_ITEM MainMenu[] = {
	{1, "User Setup",			7, UserSetup},
	{2, "Timers Setup",			8, TimerSetup},
	{3, "Sensor Addr Setup",	5, SensorSetup},
	{4, "System Setup",			3, SystemSetup}
};

//	TouchArea Array
TOUCH_LOC_8 Buttons[] = {
	{1, 20, 20, 230, 60},
};


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

void DrawMenuButtonArray(uint8_t maxbuttons) {
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

void MenuLoop()
{
	if (Touch.dataAvailable()) { ReadTouchData(); }	//	read the data from the touchscreen
}

void ReadTouchData()
{
	Touch.read();	//	request the coordinates from the touch screen
	Touchx = Touch.getX();	//	get the x coordinate from the touch screen
	Touchy = Touch.getY();	//	get the y coordinate from the touch screen
}

void AnalyzeTouchData()
{
	for (int i = 0; i == TotalButtons; i++) {

	}
}

void UserSetup()
{
	TFT.clrScr();
	TFT.fillScr(VGA_BLUE);
	TFT.print("User Setup", CENTER, 50, 0);
}

void TempType()
{
}

void TempPrecision()
{
}

void TempReadDelay()
{
}

void Brightness()
{
}

void TimeFormat()
{
}

void SecondsDisplay()
{
}

void FlowOnOff()
{
}

void TimerSetup()
{
}

void SetTimer(short numbertoset)
{
}

void SensorSetup()
{
}

void TempSensorCalib(byte numbertocalib)
{
}

void FlowSensorCalib()
{
}

void SensorAddrConfig()
{
}

void SystemSetup()
{
}

void SerialDebugging()
{
}

void EraseEEPROM()
{
}

void RestorDefaults()
{
}