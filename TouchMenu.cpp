#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>
#include "Arduino.h"

//  ***********************************************
//  Variables
//  ***********************************************
uint8_t TotalButtons;
uint16_t Touchx;
uint16_t Touchy;

//  ***********************************************
//  Defines
//  ***********************************************

#define MAX_MENU_NAME_LENGTH 13

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
			void SetDateTime();
			void FlowOnOff();
		void TimerSetup();
			void SetTimer();
		void SensorSetup();
			void TempSensorCalib();
		void SensorAddrConfig();
		void SetupFlowSensor();
			void FlowOnOff();
			void CalibrateFlowSensor();
			void SetMinFlow();
		void SystemSetup();
			void SerialDebugging();
			void EraseEEPROM();
			void RestorDefaults();


		void ExitMenu();
		void DrawMenuButtonArray(uint8_t buttons);
		void MenuLoop();
		void ReadTouchData();
		void AnalyzeMenuTouchData();

		void REPLACEME();

//  ***********************************************
//  Menu Array Definitions
//  ***********************************************
struct MENU_ITEM {
	char Menu_Name[20];
	byte Menu_Size;
	void(*function)(void);
};

//  ***********************************************
//  Button Location Array Definitions
//  ***********************************************
struct TOUCH_LOC {
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
MENU_ITEM Main_Menu[] = {
	{ "User Setup",		7, UserSetup },
	{ "Timers Setup",	8, TimerSetup },
	{ "Sensor Setup",	5, SensorSetup },
	{ "Sensor Calib",	8, TempSensorCalib },
	{ "Setup Flow",		4, SetupFlowSensor },
	{ "System Setup",	3, SystemSetup },
	{ "Exit",			0, ExitMenu }
};

MENU_ITEM User_Setup[] = {
	{ "Brightness",			1, Brightness },
	{ "Time Format",		2, TimeFormat },
	{ "Display Sec",		2, SecondsDisplay },
	{ "Set Date/Time",		2, SetDateTime },
	{ "Back",				7, UserSetup }
};

MENU_ITEM Timer_Setup[] = {
	{ "Set Timer 1",	2, SetTimer },
	{ "Set Timer 2",	2, SetTimer },
	{ "Set Timer 3",	2, SetTimer },
	{ "Set Timer 4",	2, SetTimer },
	{ "Set Timer 5",	2, SetTimer },
	{ "Set Timer 6",	2, SetTimer },
	{ "Set Timer 7",	2, SetTimer },
	{ "Set Timer 8",	2, SetTimer }
};

MENU_ITEM Sensor_Setup[] = {
	{ "Temp Unit",		2, REPLACEME },
	{ "Precision",		2, REPLACEME },
	{ "Read Delay",		2, REPLACEME },
	{ "Sensor Names",	2, REPLACEME },
	{ "Back",			2, REPLACEME }
};

MENU_ITEM Sensor_Calib[] = {
	{ "Calib Temp 1",	2, REPLACEME },
	{ "Calib Temp 2",	2, REPLACEME },
	{ "Calib Temp 3",	2, REPLACEME },
	{ "Calib Temp 4",	2, REPLACEME },
	{ "Back",			2, REPLACEME }
};

MENU_ITEM Setup_Flow[] = {
	{ "On/Off",			2, FlowOnOff},
	{ "Calib Flow",		2, CalibrateFlowSensor},
	{ "Set Min Flow",	2, SetMinFlow}
};

MENU_ITEM System_Setup[] = {
	{ "Debugging",			2, REPLACEME },
	{ "Erase EEPROM",		2, REPLACEME },
	{ "Restore Defaults",	2, REPLACEME },
	{ "Back",				2, REPLACEME }
};

//	TouchArea Array
TOUCH_LOC Buttons_8[] = {
	{1, 20, 68, 230, 108},
	{2, 250, 68, 460, 108},
	{3, 20, 116, 230, 156},
	{4, 250, 116, 460, 156},
	{5, 20, 164, 230, 204},
	{6, 250, 164, 460, 204},
	{7, 20, 212, 230, 252},
	{8, 250, 212, 460, 252}
};

TOUCH_LOC Buttons_4[] = {
	{1, 135, 68, 345, 108},
	{2, 135, 116, 345, 156},
	{3, 135, 164, 345, 204},
	{4, 135, 212, 345, 252}
};


TouchMenu::TouchMenu(){
	//	only here to initialize the class
}

void TouchMenu::MainMenu(){
	TFT.fillScr(VGA_NAVY);
	TFT.setBackColor(VGA_NAVY);
	TFT.setFont(GroteskBold24x48);
	TFT.print("MAIN MENU", CENTER, 10, 0);
	Serial.println(Main_Menu[0].Menu_Name);
	DrawMenuButtonArray(4);
	menuMode = 1;
	MenuLoop();
	menuMode = 0;
	today = 0;	//  set today to 0 so that the date function gets called
}

void DrawMenuButtonArray(uint8_t maxbuttons) {
	//	This function will draw a button array depending on the number of buttons
	//	maxbuttons is the total number of buttons needed

	//	initalize variables
	uint8_t buttoncount = 0;	//	counter for the number of buttons printed

	//	determine you need 1 or 2 columns of buttons depending on the size of max buttons
	if (maxbuttons <= 4) {
		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < 8; i++) {

			//	determine where the starting point will be for the text based on the length of the menu item
			String menustring = Main_Menu[i].Menu_Name;
			if (menustring.length() > 13) { menustring = "Too Many Char"; }
			int size = menustring.length();
			int startoffset = (208 - (size * 16)) / 2;

			//	print the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);

			//	print the menu strings
			TFT.print(menustring, Buttons_4[i].X_Start + 1 + startoffset, Buttons_4[i].Y_Start + 5);

			//	increment the buttoncount and break if needed
			buttoncount++;
			Serial.println(buttoncount);
			if (buttoncount == maxbuttons) { break; }
		}
	}
	else {
		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < 8; i++) {

			//	determine where the starting point will be for the text based on the length of the menu item
			String menustring = Main_Menu[i].Menu_Name;
			int size = menustring.length();
			int startoffset = (208 - (size * 16)) / 2;

			//	print the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);
			
			//	print the menu strings
			TFT.print(Main_Menu[i].Menu_Name, Buttons_8[i].X_Start + 1 + startoffset, Buttons_8[i].Y_Start + 5);

			//	increment the buttoncount and break if needed
			buttoncount++;
			Serial.println(buttoncount);
			if (buttoncount == maxbuttons) { break; }
		}
	}
	//delay(5000);	//	small delay to continue showing the buttons
}

void MenuLoop(){
	while (menuMode == 1)		//  scans for a button press to do the appropriate action
	{
		menuTimeout++;
/*		if (menuTimeout == 10000) { menuMode = 0; }	*/	//	this will exit the menu system after approx 20 seconds after a button has not been pushed
		if (Touch.dataAvailable()) {
			ReadTouchData();	//	read the data from the touchscreen
			AnalyzeMenuTouchData();
			Serial.printf("menuTimeout = %d\n\n", menuTimeout);
			delay(250);		//	small debounce delay for a touch
		}
	}
}

void ReadTouchData(){
	Touch.read();	//	request the coordinates from the touch screen
	Touchx = Touch.getX();	//	get the x coordinate from the touch screen
	Touchy = Touch.getY();	//	get the y coordinate from the touch screen
	Serial.printf("Read Touch Data x=%d, y=%d\n", Touchx, Touchy);
	//menuTimeout = 0;
}

void AnalyzeMenuTouchData() {
	for (int i = 0; i <= 3; i++) {
		if ((Touchx >= Buttons_4[i].X_Start) && (Touchx <= Buttons_4[i].X_End) && (Touchy >= Buttons_4[i].Y_Start) && (Touchy <= Buttons_4[i].Y_End)) {
			TFT.setColor(VGA_RED);
			TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

			delay(150);

			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

			Serial.printf("x=%d, y=%d\n", Touchx, Touchy);
			Serial.printf("%d pass\n", i);
		}
	}
}

void REPLACEME(){
}

void UserSetup(){
	TFT.clrScr();
	TFT.fillScr(VGA_BLUE);
	TFT.print("User Setup", CENTER, 50, 0);
}

void TempType(){
}

void TempPrecision(){
}

void TempReadDelay(){
}

void Brightness(){
}

void TimeFormat(){
}

void SecondsDisplay(){
}

void SetDateTime(){
}

void FlowOnOff(){
}

void TimerSetup(){
}

void SetTimer(){
}

void SensorSetup(){
}

void TempSensorCalib(){
}

void FlowSensorCalib(){
}

void SensorAddrConfig(){
}

void SetupFlowSensor(){
}

void CalibrateFlowSensor(){
}

void SetMinFlow(){
}

void DisableFlow(){
}

void SystemSetup(){
}

void SerialDebugging(){
}

void EraseEEPROM(){
}

void RestorDefaults(){
}

void ExitMenu(){
}
