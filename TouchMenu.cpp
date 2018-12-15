#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>
#include "Arduino.h"
#include "EEprom.h"
#include <TimeLib.h>

//  ***********************************************
//  Variables
//  ***********************************************
uint8_t TotalButtons;
uint16_t Touchx;
uint16_t Touchy;
uint16_t ButtonPressed;
uint8_t SubMenuLoopFlag;
uint8_t MenuLoopFlag;
void(*function)(void);	//	stores the function name to be called from the arrays

uint8_t setweekday;
uint8_t setday;
uint8_t setmonth;
uint16_t setyear;
uint8_t sethour;
uint8_t setminute;
uint8_t setsecond;


//  ***********************************************
//  Defines
//  ***********************************************

#define MAX_MENU_NAME_LENGTH 13
#define MAIN_MENU_SIZE 7
#define USER_SETUP_SIZE 5
#define TIMER_SETUP_SIZE 9
#define SENSOR_SETUP_SIZE 5
#define SENSOR_CALIB_SIZE 5
#define	SETUP_FLOW_SIZE 4
#define SYSTEM_SETUP_SIZE 4

#define TFT_B_LIGHT_PIN 9

#define DS1307RTC 0x68			//	Set the address of the DS1307 RTC

//  ***********************************************
//  Function prototypes
//  ***********************************************
void MainMenu();
		void UserSetup();
			void Brightness();
			void TimeFormat();
			void SecondsDisplay();
			void SetDateTime();
		void TimerSetup();
			void SetTimer();
		void SensorSetup();
			void TempUnit();
			void TempPrecision();
			void TempReadDelay();
			void SensorNames();
		void SensorCalib();
			void SensorAddrConfig();
		void SetupFlowSensor();
			void FlowOnOff();
			void CalibrateFlowSensor();
			void SetMinFlow();
		void SystemSetup();
			void SerialDebugging();
			void EraseEEPROM();
			void RestoreDefaults();

		uint8_t AdjustTime();
		uint8_t AdjustDate();

		void ExitMenu();
		void BackMenu();
		void DrawMenuButtonArray(uint8_t buttons, struct MENU_ITEM *currentmenu);
		void MenuLoop(uint8_t menuSize, uint16_t delayms);
		void ReadTouchData();
		void AnalyzeMenuTouchData(int menuSize);
		void ClearScreenHeader();
		uint8_t DecToBcd(uint8_t val);
		uint8_t BcdToDec(uint8_t val);
		void Dummy();

		void REPLACEME();

//  ***********************************************
//  Menu Array Definitions
//  ***********************************************
typedef struct MENU_ITEM {
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
	{ "Sensor Calib",	8, SensorCalib },
	{ "Setup Flow",		4, SetupFlowSensor },
	{ "System Setup",	3, SystemSetup },
	{ "Exit",			0, BackMenu }
};

//	User Setup Arrays
MENU_ITEM User_Setup[] = {
	{ "Brightness",			4, Brightness },
	{ "Time Format",		3, TimeFormat },
	{ "Display Sec",		3, SecondsDisplay },
	{ "Set Date/Time",		13, SetDateTime },
	{ "Back",				0, BackMenu }
};

	MENU_ITEM Backlight_Brightness[] = {
		{ "High",	0,	REPLACEME },
		{ "Medium",	0,	REPLACEME },
		{ "Low",	0,	REPLACEME },
		{ "Back", 	0,	REPLACEME }
	};

	MENU_ITEM Time_Format[] = {
		{ "24 Hour",	0,	REPLACEME },
		{ "12 Hour",	0,	REPLACEME },
		{ "Back",		0,	REPLACEME }
	};

//	Timer Setup Arrays
MENU_ITEM Timer_Setup[] = {
	{ "Set Timer 1",	2, SetTimer },
	{ "Set Timer 2",	2, SetTimer },
	{ "Set Timer 3",	2, SetTimer },
	{ "Set Timer 4",	2, SetTimer },
	{ "Set Timer 5",	2, SetTimer },
	{ "Set Timer 6",	2, SetTimer },
	{ "Set Timer 7",	2, SetTimer },
	{ "Set Timer 8",	2, SetTimer },
	{ "Back",			0, BackMenu }
};

//	Sensor Setup Arrays
MENU_ITEM Sensor_Setup[] = {
	{ "Temp Unit",		3, TempUnit },
	{ "Precision",		3, TempPrecision },
	{ "Read Delay",		4, TempReadDelay },
	{ "Sensor Names",	5, REPLACEME },
	{ "Back",			0, BackMenu }
};

	MENU_ITEM Temp_Unit[] = {
		{ "Celcius",		0,	REPLACEME },
		{ "Fahrenheit",		0,	REPLACEME },
		{ "Back",			0,	REPLACEME }
	};

	MENU_ITEM Temp_Precision[] = {
		{ "No Decimal",		0,	REPLACEME },
		{ "1 Decimal",		0,	REPLACEME },
		{ "Back",			0,	REPLACEME }
	};

	MENU_ITEM Read_Delay[] = {
		{ "10 Seconds",		0,	REPLACEME },
		{ "20 Seconds",		0,	REPLACEME },
		{ "30 Seconds",		0,	REPLACEME },
		{ "Back",			0,	REPLACEME }
	};

	MENU_ITEM Sensor_Names[] = {
		{ "!Implemented",	0,	REPLACEME },
		{ "!Implemented",	0,	REPLACEME },
		{ "!Implemented",	0,	REPLACEME },
		{ "!Implemented",	0,	REPLACEME },
		{ "Back",			0,	REPLACEME }
	};

//	Sensor Calibration Arrays
MENU_ITEM Sensor_Calib[] = {
	{ "Calib Temp 1",	2, REPLACEME },
	{ "Calib Temp 2",	2, REPLACEME },
	{ "Calib Temp 3",	2, REPLACEME },
	{ "Calib Temp 4",	2, REPLACEME },
	{ "Back",			2, BackMenu }
};

//	Setup Flow Sensor Arrays
MENU_ITEM Setup_Flow[] = {
	{ "On/Off",			3, FlowOnOff},
	{ "Calib Flow",		2, CalibrateFlowSensor},
	{ "Set Min Flow",	2, SetMinFlow},
	{ "Back",			2, BackMenu }
};

//	System Setup
MENU_ITEM System_Setup[] = {
	{ "Debugging",			10, SerialDebugging },
	{ "Erase EEPROM",		2, EraseEEPROM },
	{ "Fact Defaults",		2, RestoreDefaults },
	{ "Back",				2, BackMenu }
};

	MENU_ITEM Serial_Debugging[] = {
		{ "All On",			0,	REPLACEME },
		{ "All Off",		0,  REPLACEME },
		{ "Temp Sensors",	0,	REPLACEME },
		{ "Menu",			0,	REPLACEME },
		{ "Alarms",			0,	REPLACEME },
		{ "EEPROM",			0,	REPLACEME },
		{ "Relays",			0,	REPLACEME },
		{ "System",			0,	REPLACEME },
		{ "Flow Sensor",	0,	REPLACEME },
		{ "Back",			0,	REPLACEME }
	};

MENU_ITEM Yes_No[] = {
	{ "Yes",		0,	REPLACEME },
	{ "No",			0,	REPLACEME }
};

MENU_ITEM On_Off[] = {
	{ "On",			0,	REPLACEME },
	{ "Off",		0,	REPLACEME },
	{ "Back",		0,	REPLACEME}
};

MENU_ITEM Back_Next[] = {
	{ "Back",	0, REPLACEME },
	{ "Next",		0, REPLACEME }
};

MENU_ITEM CHANGE_ME[] = {
	{ "DUMMY", 0, REPLACEME },
	{ "DUMMY", 0, REPLACEME }
};

char* WEEKDAY[] = { "INV", "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

//  ***********************************************
//  TouchArea Arrays
//  ***********************************************

//	4 Button Main Menu Touch Areas
//	button number, xstart, ystart, xend, yend
//	menusize = 1-5
TOUCH_LOC Buttons_4[] = {
	{1, 135, 68, 345, 108},
	{2, 135, 116, 345, 156},
	{3, 135, 164, 345, 204},
	{4, 135, 212, 345, 252},
	{5, 420, 212, 460, 252}		//	Back button if necessary
};

//	8 Button Main Menu Touch Areas
//	button number, xstart, ystart, xend, yend
//	menusize = 5-8
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

//	10 Button Main Menu Touch Areas
//	button number, xstart, ystart, xend, yend
//	menusize = 10
TOUCH_LOC Buttons_10[] = {
	{1, 20, 68, 230, 101},
	{2, 250, 68, 460, 101},
	{3, 20, 108, 230, 141},
	{4, 250, 108, 460, 141},
	{5, 20, 148, 230, 181},
	{6, 250, 148, 460, 181},
	{7, 20, 188, 230, 221},
	{8, 250, 188, 460, 221},
	{9, 20, 228, 230, 261},
	{10, 250, 228, 460, 261}
};

//	1 Button Bottom Located Main Menu Touch Areas 
//	button number, xstart, ystart, xend, yend
//	menusize = 11
TOUCH_LOC Buttons_1[] = {
	{1, 250, 218, 460, 258}
};

//	2 Buttons Bottom Located Main Menu Touch Areas 
//	button number, xstart, ystart, xend, yend
//	menusize = 12
TOUCH_LOC Buttons_12[] = {
	{1, 20, 218, 230, 258},
	{2, 250, 218, 460, 258}
};

//	8 Button, 3 Up Arrows, 3 Down Arrows, and 2 Bottom Back and OK buttons Touch Areas
//	button number, xstart, ystart, xend, yend
//	menusize = 13
TOUCH_LOC Buttons_13[] = {
	{1, 20, 218, 230, 258},		//	Back
	{2, 250, 218, 460, 258},	//	OK
	{3, 144, 70, 192, 104},		//	1st Up Arrow
	{4, 216, 70, 264, 104},		//	2nd Up Arrow
	{5, 288, 70, 336, 104},		//	3rd Up Arrow
	{6, 144, 172, 192, 206},	//	1st Down Arrow
	{7, 216, 172, 264, 206},	//	2nd Down Arrow
	{8, 288, 172, 336, 206}		//	3rd Down Arrow
};

//	10 Button, 4 Up Arrows, 4 Down Arrows, and 2 Bottom Back and OK buttons Touch Areas
//	button number, xstart, ystart, xend, yend
//	menusize = 14
TOUCH_LOC Buttons_14[] = {
	{1, 20, 218, 230, 258},		//	Back
	{2, 250, 218, 460, 258},	//	OK
	{3, 108, 70, 156, 104},		//	1st Up Arrow
	{4, 192, 70, 240, 104},		//	2nd Up Arrow
	{5, 264, 70, 312, 104},		//	3rd Up Arrow
	{6, 336, 70, 384, 104},		//	4th Up Arrow
	{7, 108, 172, 156, 206},	//	1st Down Arrow
	{8, 192, 172, 240, 206},	//	2nd Down Arrow
	{9, 264, 172, 312, 206},	//	3rd Down Arrow
	{10, 336, 172, 384, 206}	//	4th Down Arrow
};

TouchMenu::TouchMenu(){
	//	only here to initialize the class
}

void TouchMenu::EnterMenu()
//	Entry point to the menu system and keep looping till the menu times out or is exited.
{
	SubMenuLoopFlag = 1;
	MainMenu();	//	call the main menu function
	today = 0;	//  set today to 0 so that the date function gets called
}

void MenuLoop(uint8_t menusize, uint16_t delayms) {
	delay(delayms);
	while (MenuLoopFlag == 1)		//  scans for a button press to do the appropriate action
	{
		menuTimeout++;
		/*		if (menuTimeout == 10000) { menuMode = 0; }	*/	//	this will exit the menu system after approx 20 seconds after a button has not been pushed
		if (Touch.dataAvailable()) {
			ReadTouchData();					//	read the data from the touchscreen
			AnalyzeMenuTouchData(menusize);		//  analyzes the touch screen data to determine what was pushed
			//Serial.printf("menuTimeout = %d\n\n", menuTimeout);
			delay(delayms/2);		//	small debounce delay for a touch
			//  evaluate the button pressed for a valid area.
			if (ButtonPressed == 255) { MenuLoopFlag = 1; }		//	invalid button, reloop
			else { MenuLoopFlag = 0; }							//	valid button, exit loop
		}
	}
}

void DrawMenuButtonArray(uint8_t maxbuttons, struct MENU_ITEM *currentmenu) {
	//	This function will draw a button array depending on the number of buttons
	//	maxbuttons is the total number of buttons needed

	//	determine the number of buttons depending on the size of maxbuttons
	if (maxbuttons <= 4) {
		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < maxbuttons; i++) {

			//	variable for storing the button name 
			char* menustring = currentmenu[i].Menu_Name;

			//	draw the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);

			//	print the menu strings
			TFT.print(menustring, Buttons_4[i].X_Start + 1 + 0, Buttons_4[i].Y_Start + 5);
		}
	}
	else if ((maxbuttons > 4) && (maxbuttons <= 8)) {
		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < maxbuttons; i++) {

			//	variable for storing then button name 
			char* menustring = currentmenu[i].Menu_Name;

			//	draw the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);
			
			//	print the menu strings
			TFT.print(menustring, Buttons_8[i].X_Start + 1, Buttons_8[i].Y_Start + 5);
		}
	}
	else if ((maxbuttons > 8) && (maxbuttons <= 10)) {
		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < maxbuttons; i++) {

			//	variable for storing then button name 
			char* menustring = currentmenu[i].Menu_Name;

			//	draw the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_10[i].X_Start, Buttons_10[i].Y_Start, Buttons_10[i].X_End, Buttons_10[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_10[i].X_Start, Buttons_10[i].Y_Start, Buttons_10[i].X_End, Buttons_10[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			//TFT.setFont(Retro8x16);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);

			//	print the menu strings
			TFT.print(menustring, Buttons_10[i].X_Start + 3, Buttons_10[i].Y_Start + 1);
		}
	}
	else if (maxbuttons == 12) {
		maxbuttons = 2;	//	the actual maxbuttons for this loop

		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < maxbuttons; i++) {

			//	variable for storing the button name 
			char* menustring = currentmenu[i].Menu_Name;

			//	draw the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_12[i].X_Start, Buttons_12[i].Y_Start, Buttons_12[i].X_End, Buttons_12[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_12[i].X_Start, Buttons_12[i].Y_Start, Buttons_12[i].X_End, Buttons_12[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);

			//	print the menu strings
			TFT.print(menustring, Buttons_12[i].X_Start + 1 + 0, Buttons_12[i].Y_Start + 5);
			}
	}
	else if (maxbuttons == 13) {
		 maxbuttons = 8;	//	the actual maxbuttons for this loop

		 //	loop through and start printing the buttons up to maxbuttons
		 for (byte i = 0; i < maxbuttons; i++) {

			 //	variable for storing the button name 
			 char* menustring = currentmenu[i].Menu_Name;

			 //	draw the buttons
			 TFT.setColor(VGA_BLUE);
			 TFT.fillRoundRect(Buttons_13[i].X_Start, Buttons_13[i].Y_Start, Buttons_13[i].X_End, Buttons_13[i].Y_End);
			 TFT.setColor(VGA_WHITE);
			 TFT.drawRoundRect(Buttons_13[i].X_Start, Buttons_13[i].Y_Start, Buttons_13[i].X_End, Buttons_13[i].Y_End);

			 //	set up the font and colors for the strings
			 TFT.setFont(GroteskBold16x32);
			 TFT.setBackColor(VGA_BLUE);
			 TFT.setColor(VGA_WHITE);

			 //	print the menu strings only if it is the last 2 buttons for the back and ok buttons
			 if ((i == 0) || (i == 1)) { TFT.print(menustring, Buttons_13[i].X_Start + 1 + 0, Buttons_13[i].Y_Start + 5); }
			 //	draw the icons for the arrows
			 else if ((i >= 2) && (i <= 4)) { TFT.drawBitmap(Buttons_13[i].X_Start + 8, Buttons_13[i].Y_Start + 1, 32, 32, arrow_up); }
			 else if (i >= 5) { TFT.drawBitmap(Buttons_13[i].X_Start + 8, Buttons_13[i].Y_Start + 1, 32, 32, arrow_down); }
		 }
	 }
	else if (maxbuttons == 14) {
		maxbuttons = 10;	//	the actual maxbuttons for this loop

		//	loop through and start printing the buttons up to maxbuttons
		for (byte i = 0; i < maxbuttons; i++) {

			//	variable for storing the button name 
			char* menustring = currentmenu[i].Menu_Name;

			//	draw the buttons
			TFT.setColor(VGA_BLUE);
			TFT.fillRoundRect(Buttons_14[i].X_Start, Buttons_14[i].Y_Start, Buttons_14[i].X_End, Buttons_14[i].Y_End);
			TFT.setColor(VGA_WHITE);
			TFT.drawRoundRect(Buttons_14[i].X_Start, Buttons_14[i].Y_Start, Buttons_14[i].X_End, Buttons_14[i].Y_End);

			//	set up the font and colors for the strings
			TFT.setFont(GroteskBold16x32);
			TFT.setBackColor(VGA_BLUE);
			TFT.setColor(VGA_WHITE);

			//	print the menu strings only if it is the last 2 buttons for the back and ok buttons
			if ((i == 0) || (i == 1)) { TFT.print(menustring, Buttons_14[i].X_Start + 1 + 0, Buttons_14[i].Y_Start + 5); }
			//	draw the icons for the arrows
			else if ((i >= 2) && (i <= 5)) { TFT.drawBitmap(Buttons_14[i].X_Start + 8, Buttons_14[i].Y_Start + 1, 32, 32, arrow_up); }
			else if (i >= 6) { TFT.drawBitmap(Buttons_14[i].X_Start + 8, Buttons_14[i].Y_Start + 1, 32, 32, arrow_down); }
		}
	}
}

void ReadTouchData()
//	Read the coordinates where the screen was touched
{
	Touch.read();	//	request the coordinates from the touch screen
	Touchx = Touch.getX();	//	get the x coordinate from the touch screen
	Touchy = Touch.getY();	//	get the y coordinate from the touch screen
	//Serial.printf("Read Touch Data x=%d, y=%d\n", Touchx, Touchy);
	//menuTimeout = 0;
}

void AnalyzeMenuTouchData(int menuSize)
//	Analyze the touch screen data and determine which button to use
{
	if (menuSize <= 4) {
		for (int i = 0; i < menuSize; i++) {
			if ((Touchx >= Buttons_4[i].X_Start) && (Touchx <= Buttons_4[i].X_End) && (Touchy >= Buttons_4[i].Y_Start) && (Touchy <= Buttons_4[i].Y_End))
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_4[i].X_Start, Buttons_4[i].Y_Start, Buttons_4[i].X_End, Buttons_4[i].Y_End);

				//  draw a white outline around the box that was pressed
				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
	else if ((menuSize > 4) && (menuSize <= 8)) {
		for (int i = 0; i < menuSize; i++) {
			if ((Touchx >= Buttons_8[i].X_Start) && (Touchx <= Buttons_8[i].X_End) && (Touchy >= Buttons_8[i].Y_Start) && (Touchy <= Buttons_8[i].Y_End))
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				//  draw a white outline around the box that was pressed
				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_8[i].X_Start, Buttons_8[i].Y_Start, Buttons_8[i].X_End, Buttons_8[i].Y_End);

				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
	else if ((menuSize > 8) && (menuSize <= 10)) {
		for (int i = 0; i < menuSize; i++) {
			if ((Touchx >= Buttons_10[i].X_Start) && (Touchx <= Buttons_10[i].X_End) && (Touchy >= Buttons_10[i].Y_Start) && (Touchy <= Buttons_10[i].Y_End)) 
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_10[i].X_Start, Buttons_10[i].Y_Start, Buttons_10[i].X_End, Buttons_10[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				//  draw a white outline around the box that was pressed
				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_10[i].X_Start, Buttons_10[i].Y_Start, Buttons_10[i].X_End, Buttons_10[i].Y_End);

				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
	else if (menuSize == 12) {
		for (int i = 0; i < menuSize; i++) {
			menuSize = 2;	//	the actual maxbuttons for this loop
			if ((Touchx >= Buttons_12[i].X_Start) && (Touchx <= Buttons_12[i].X_End) && (Touchy >= Buttons_12[i].Y_Start) && (Touchy <= Buttons_12[i].Y_End))
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_12[i].X_Start, Buttons_12[i].Y_Start, Buttons_12[i].X_End, Buttons_12[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				//  draw a white outline around the box that was pressed
				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_12[i].X_Start, Buttons_12[i].Y_Start, Buttons_12[i].X_End, Buttons_12[i].Y_End);

				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
	else if (menuSize == 13) {
		menuSize = 8;	//	the actual maxbuttons for this loop
		for (int i = 0; i < menuSize; i++) {
			if ((Touchx >= Buttons_13[i].X_Start) && (Touchx <= Buttons_13[i].X_End) && (Touchy >= Buttons_13[i].Y_Start) && (Touchy <= Buttons_13[i].Y_End))
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_13[i].X_Start, Buttons_13[i].Y_Start, Buttons_13[i].X_End, Buttons_13[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				//  draw a white outline around the box that was pressed
				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_13[i].X_Start, Buttons_13[i].Y_Start, Buttons_13[i].X_End, Buttons_13[i].Y_End);

				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
	else if (menuSize == 14) {
		menuSize = 10;	//	the actual maxbuttons for this loop
		for (int i = 0; i < menuSize; i++) {
			if ((Touchx >= Buttons_14[i].X_Start) && (Touchx <= Buttons_14[i].X_End) && (Touchy >= Buttons_14[i].Y_Start) && (Touchy <= Buttons_14[i].Y_End))
			{
				//  draw a red outline around the box that was pressed
				TFT.setColor(VGA_RED);
				TFT.drawRoundRect(Buttons_14[i].X_Start, Buttons_14[i].Y_Start, Buttons_14[i].X_End, Buttons_14[i].Y_End);

				delay(150);	//	short delay to allow the red box to be noticable

				//  draw a white outline around the box that was pressed
				TFT.setColor(VGA_WHITE);
				TFT.drawRoundRect(Buttons_14[i].X_Start, Buttons_14[i].Y_Start, Buttons_14[i].X_End, Buttons_14[i].Y_End);

				ButtonPressed = i;	//	set the button that was pressed
				i = menuSize;		//	once the button has been found, exit the for loop
			}
			else { ButtonPressed = 255; }
		}
	}
}

void ClearScreenHeader()
//  this function clears the screen and sets the font to be used
{
	TFT.fillScr(VGA_NAVY);			//  fill the screen
	TFT.setBackColor(VGA_NAVY);		//  set the back color
	TFT.setFont(GroteskBold24x48);	//  set the font to be used	
}

void REPLACEME(){
}

//  MAIN MENU SECTION
//  ***********************************************

void MainMenu() {
	while (SubMenuLoopFlag == 1)
	{
		Serial.print("Main Menu Entered\n");
		ClearScreenHeader();								//	clear the touch screen
		TFT.print("MAIN MENU", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(MAIN_MENU_SIZE, Main_Menu);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(MAIN_MENU_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("Main Menu Button %d pressed\n\n", ButtonPressed);
		function = Main_Menu[ButtonPressed].function;	//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

//  USER SETUP MENU SECTION
//  ***********************************************

void UserSetup(){
	while (SubMenuLoopFlag == 1)
	{
		Serial.print("User Setup Entered\n");
		ClearScreenHeader();								//	clear the touch screen
		TFT.print("USER SETUP", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(USER_SETUP_SIZE, User_Setup);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(USER_SETUP_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("User Setup Button %d pressed\n\n", ButtonPressed);
		function = User_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
		Serial.println();
	}
	SubMenuLoopFlag = 1;
}

void Brightness(){
	//  Sets the brightness of the lcd screen
	uint8_t menusize = User_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Change Brightness Level Entered\n");
	ClearScreenHeader();									//	clear the touch screen
	TFT.print("BRIGHTNESS", CENTER, 10, 0);					//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Backlight_Brightness);	//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Brightness Level %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Backlight Level Set", CENTER, 70, 0);	//	print the header

	byte brightness;		//	byte to store the brightness level

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		brightness = 255;	//	set the brightness to max level
		TFT.print("to High", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		brightness = 170;	//	set the brightness to mid level
		TFT.print("to Medium", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 2:
		brightness = 70;	//	set the brightness to low level
		TFT.print("to Low", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	eeprom.write(25, brightness);	//	write the data to the eeprom
	brightness = eeprom.read(25);	//	read the data back out
	analogWrite(TFT_B_LIGHT_PIN, brightness);	//	write the brightness to the PWM pin
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void TimeFormat()
//  Sets the time format to 12h or 24h
{
	uint8_t menusize = User_Setup[ButtonPressed].Menu_Size;

	Serial.print("Change Time Format Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("TIME FORMAT", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Time_Format);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Time Format Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Time Format Set to", CENTER, 70, 0);		//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
		case 0:
			eeprom.write(23, 0);	//	write the data to the eeprom for 24 hour
			timeFormat = 0;
			TFT.print("24 Hour", CENTER, 130, 0);	//	print to the display to see what was done
			break;
		case 1:
			eeprom.write(23, 1);	//	write the data to the eeprom for 12 hour
			timeFormat = 1;
			TFT.print("12 Hour", CENTER, 130, 0);	//	print to the display to see what was done
			break;
		default:
			break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void SecondsDisplay(){
	//  Set how many buttons there are
	uint8_t menusize = User_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Change Seconds Display Entered\n");
	ClearScreenHeader();								//	clear the touch screen
	TFT.print("SECONDS DISPLAY", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, On_Off);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;		//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Seconds Display Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();								//	clear the touch screen
	TFT.print("Seconds Display set", CENTER, 70, 0);	//	print the header

	switch (ButtonPressed)
	{
	case 0:
		eeprom.write(24, 0);	//	write the data to the eeprom
		TFT.print("to Off", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		eeprom.write(24, 1);	//	write the data to the eeprom
		TFT.print("to On", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void SetDateTime(){
	//  Set how many buttons there are
	uint8_t menusize;		//	set the menusize
	uint8_t writetime = 0;	//	when set to 1 will enable the time save to the rtc
	uint8_t writedate = 0;	//	when set to 1 will wnable the date save to the rtc
	uint8_t twelvehour;		//	temp variable to store the 12 hour hour to display
	char buffer[12];		//	buffer to hold the assembled strings

	Serial.print("Set Date/Time Entered\n");

	//	Set the date strings to be written later
	ClearScreenHeader();						//	clear the touch screen
	TFT.print("SET DATE", CENTER, 10, 0);		//  prints the menu name and centers it
	Serial.print("Setting Date\n");
	menusize = 14;								//	set the menusize to call the date change display
	DrawMenuButtonArray(menusize, Back_Next);	//	draws the menu buttons for the specified menu size and array
	TFT.setBackColor(VGA_NAVY);					//  set the back color
	TFT.setFont(GroteskBold24x48);				//  set the font to be used

	writedate = AdjustDate();					//	call the adjust date function
	if (writedate == 0) { return; }				//	exits the funciton if the back button was pressed

	//	Set the time strings to be written later
	ClearScreenHeader();						//	clear the touch screen
	TFT.print("SET TIME", CENTER, 10, 0);		//  prints the menu name and centers it
	Serial.print("Setting Time\n");
	menusize = 13;								//	set the menusize to call the time change display
	DrawMenuButtonArray(menusize, Back_Next);	//	draws the menu buttons for the specified menu size and array
	TFT.setBackColor(VGA_NAVY);					//  set the back color
	TFT.setFont(GroteskBold24x48);				//  set the font to be used

	writetime = AdjustTime();					//	call the adjust time function
	if (writetime == 0) { return; }				//	exits the funciton if the back button was pressed

	//	assemble the strings and print the date and time to be confirmed
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("SET DATE/TIME TO", CENTER, 10, 0);	//  prints the menu name and centers it
	Serial.print("Saving Date/Time\n");

	sprintf(buffer, "%s %2d/%02d/%02d", WEEKDAY[setweekday], setmonth, setday, setyear);	//	assemble the string to print the date display
	TFT.print(buffer, CENTER, 85);			//	print the date to the screen
	Serial.printf("Date: %s\n", buffer);

	switch (timeFormat) {
	case 0:	//	24 hour display print
		sprintf(buffer, "%02d:%02d:%02d", sethour, setminute, setsecond);	//	assemble the time string to print 24 hour display
		break;
	case 1:	//	12 hour display print
		//	prep the hour to be displayed correctly for 12 hour
		if (sethour == 0) { twelvehour = 12; }
		else if (sethour < 12) { twelvehour = sethour; }
		else if (sethour == 12) { twelvehour = 12; }
		else if (sethour > 12) { twelvehour = sethour - 12; }

		//	print the hour depending on am or pm
		if (sethour >= 12) { sprintf(buffer, "%2d:%02d:%02d PM", twelvehour, setminute, setsecond); }	//	print the PM display
		else { sprintf(buffer, "%2d:%02d:%02d AM", twelvehour, setminute, setsecond); }		//	print the AM display
		break;
	default:
		break;
	}
	Serial.printf("Time: %s\n", buffer);
	TFT.print(buffer, CENTER, 139);			//	print the time string to the screen

	//	confirm the date and time to be set using the yes/no buttons
	//	draw the menu buttons
	menusize = 12;							//	set the menusize to call the 2 button display
	DrawMenuButtonArray(menusize, Yes_No);	//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;			//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop

	Serial.printf("Confirm date/time set button %d pressed\n\n", ButtonPressed);

	switch (ButtonPressed) {
	case 0:		//	yes button pressed, save the date and time
		ClearScreenHeader();
		TFT.print("Saving Date/Time", CENTER, 112);
		setTime(sethour, setminute, setsecond, setday, setmonth, setyear);
		RTC.set(now());
		delay(1000);

		//  GET THE TIME FROM THE RTC
		setSyncProvider(RTC.get);			//  this function get the time from the RTC
		if (timeStatus() != timeSet) {		//  checks to see if it can read the RTC
			RTC_Status = 0;
			Serial.print("Unable to get the RTC\n\n");
		}
		else { Serial.print("RTC system time\n"); }
		break;
	case 1:		//	no button pressed return to the user setup screen
		SubMenuLoopFlag = 0;
		break;
	default:
		break;
	}
}

uint8_t AdjustDate() {
//  Use for anything that needs the time adjusted.
//	It will store the results in the global variables sethours, setminutes, and setseconds variables for use outsid of this function.
	uint8_t menusize = 14;		//	set the menusize
	uint8_t w = weekday();
	uint8_t m = month();
	uint8_t d = day();
	uint16_t y = year() - 2000;
	char buffer[12];			//	a buffer to hold the text to display

	//	loop until we are finished editing the time or until the back button is hit
	while (SubMenuLoopFlag == 1) {
		//	assemble the date to display
		sprintf(buffer, "%s %2d/%02d/%02d", WEEKDAY[w], m, d, y);	//	print the date display
		TFT.print(buffer, 96, 114, 0);		//  prints the menu name

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;		//	enables the menu to loop
		MenuLoop(menusize, 0);	//	call the menu loop with no delay
		if (ButtonPressed == 0) { return 0; }	//	return if the back button was pressed

		//	evaluate the button pressed
		Serial.printf("Set Date Button %d pressed\n", ButtonPressed);

		switch (ButtonPressed) {
		case 0:		//	BACK BUTTON
			//	NOT NEEDED AS THE IF FUNCTION ABOVE RETURNS THE FUNCTION
			break;
		case 1:		//	OK BUTTON
			//	store the adjusted date to the global varibales
			setweekday = w;
			setmonth = m;
			setday = d;
			setyear = y + 2000;
			//	returns 1 to verify that the date will be set
			return 1;
			break;
		case 2:		//	WEEKDAY UP BUTTON
			w++;		//	increment weekday
			if (w > 7) { w = 1; }	//	roll over to 1
			break;
		case 3:		//	MONTHS UP BUTTON
			m++;	//	increment month
			if (m > 12) { m = 1; }	//	roll over to 1
			break;
		case 4:		//	DAY UP BUTTON
			d++;	//	increment day
			if (d > 31) { d = 1; }	// roll over to 0
			break;
		case 5:		//	YEAR UP BUTTON
			y++;	//	increment year
			if (y > 99) { y = 0; }	// roll over to 0
			break;
		case 6:		//	WEEKDAY DOWN BUTTON
			w--;		//	decrement the weekday
			if (w == 0) { w = 7; }		//	roll back to 23
			break;
		case 7:		//	MONTHS DOWN BUTTON
			m--;	//	decrement the month
			if (m == 0) { m = 12; }	//	roll back to 59
			break;
		case 8:		//	DAY DOWN BUTTON
			d--;	//	decrement the day
			if (d == 0) { d = 31; }	//	roll back to 59
			break;
		case 9:		//	YEAR DOWN BUTTON
			y--;	//	decrement the year
			if (y == 255) { y = 99; }	//	roll back to 99
			break;
		default:
			break;
		}
	}
}

uint8_t AdjustTime() {
	//  Use for anything that needs the time adjusted.
	//	It will store the results in the global variables sethours, setminutes, and setseconds variables for use outsid of this function.
	uint8_t menusize = 13;	//	set the menusize
	uint8_t h = hour();
	uint8_t m = minute();
	uint8_t s = second();

	uint8_t disphours = h;	//	variable for storing the hours to display on the screen
	char buffer[12];			//	a buffer to hold the text to display

	if ((timeFormat == 1) && (h > 12)) { disphours = h - 12; }	//	sets the display hours to 12 hours when entering the loop

	//	loop until we are finished editing the time or until the back button is hit
	while (SubMenuLoopFlag == 1) {
		//	assemble the time to display depending on the format set
		switch (timeFormat) {
		case 0:	//	24 hour display print
			sprintf(buffer, "%02d:%02d:%02d", disphours, m, s);	//	assemble the time string to print 24 hour display
			break;
		case 1:	//	12 hour display print
			if (h >= 12) { sprintf(buffer, "%2d:%02d:%02d PM", disphours, m, s); }	//	print the AM display
			else sprintf(buffer, "%2d:%02d:%02d AM", disphours, m, s);	//	print the PM display
			break;
		default:
			break;
		}
		TFT.print(buffer, 144, 114, 0);		//  prints the menu name and centers it

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;		//	enables the menu to loop
		MenuLoop(menusize, 0);	//	call the menu loop with no delay
		if (ButtonPressed == 0) { return 0; }	//	return if the back button was pressed

		//	evaluate the button pressed
		Serial.printf("Set Time Button %d pressed\n", ButtonPressed);

		switch (ButtonPressed) {
		case 0:		//	BACK BUTTON
			//	NOT NEEDED AS THE IF FUNCTION ABOVE RETURNS THE FUNCTION
			break;
		case 1:		//	OK BUTTON
			//	store the adjusted time to the global varibales
			sethour = h;
			setminute = m;
			setsecond = s;
			//	returns 1 to verify that the time will be set
			return 1;
			break;
		case 2:		//	HOURS UP BUTTON
			h++;		//	increment hours
			if (h > 23) { h = 0; }	//	roll over to 0
			disphours = h;					//	set the hours to display o the screen
			if ((timeFormat == 1) && (h > 12)) { disphours = h - 12; }	//	adjust 24 hour time to 12 hour time
			if ((timeFormat == 1) && (h == 0)) { disphours = 12; }				//	adjust 24 hour time to 12 hour time
			break;
		case 3:		//	MINUTES UP BUTTON
			m++;	//	increment minutes
			if (m > 59) { m = 0; }	//	roll over to 0
			break;
		case 4:		//	SECONDS UP BUTTON
			s++;	//	increment seconds
			if (s > 59) { s = 0; }	// roll over to 0
			break;
		case 5:		//	HOURS DOWN BUTTON
			h--;		//	decrement the hours
			if (h == 255) { h = 23; }		//	roll back to 23
			disphours = h;						//	set the hours to display on the screen
			if ((timeFormat == 1) && (h > 12)) { disphours = h - 12; }	//	adjust 24 hour time to 12 hour time
			if ((timeFormat == 1) && (h == 0)) { disphours = 12; }				//	adjust 24 hour time to 12 hour time
			break;
		case 6:		//	MINUTES DOWN BUTTON
			m--;	//	decrement the minutes
			if (m == 255) { m = 59; }	//	roll back to 59
			break;
		case 7:		//	SECONDS DOWN BUTTON
			s--;	//	decrement the seconds
			if (s == 255) { s = 59; }	//	roll back to 59
			break;
		default:
			break;
		}
	}
}

//  TIMER SETUP MENU
//  ***********************************************

void TimerSetup(){
	while (SubMenuLoopFlag == 1)
	{
		Serial.print("Timer Setup Entered\n");
		ClearScreenHeader();									//	clear the touch screen
		TFT.print("TIMER SETUP", CENTER, 11, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(TIMER_SETUP_SIZE, Timer_Setup);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(TIMER_SETUP_SIZE, 300);	//	call the menu loop with 300ms of delay
		
		//	call the function depending on the button pressed
		Serial.printf("Timer Setup Button %d pressed\n\n", ButtonPressed);
		function = Timer_Setup[ButtonPressed].function;
		(function)();
		Serial.println();
	}
	SubMenuLoopFlag = 1;
}

void SetTimer(){
	Serial.printf("Set Timer %d Entered\n", ButtonPressed + 1);
	ClearScreenHeader();								//	clear the touch screen

	String timerstring;					//	create a string for the timer
	char buffer[16];					//	create a buffer for the time string
	sprintf(buffer, "Timer %d Entered", ButtonPressed + 1);		//	prepare the string to print
	timerstring = String(buffer);		//	create a string for the version

	TFT.print(timerstring, CENTER, 10, 0);
	delay(1000);	//	Small delay to allow the user to see the change on the screen

}

//  SENSOR SETUP MENU
//  ***********************************************

void SensorSetup(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("Sensor Setup Entered\n");

		ClearScreenHeader();									//	clear the touch screen
		TFT.print("SENSOR SETUP", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SENSOR_SETUP_SIZE, Sensor_Setup);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;				//	enables the menu to loop
		MenuLoop(SENSOR_SETUP_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("Sensor Setup Button %d pressed\n\n", ButtonPressed);
		function = Sensor_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
		Serial.println();
	}
	SubMenuLoopFlag = 1;
}

void TempUnit() {
//  Sets the temperature unit to celcius of fahrenheit
	uint8_t menusize = Sensor_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Change Temp Unit Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("TEMP UNIT", CENTER, 10, 0);			//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Temp_Unit);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Temp Unit Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Temp Unit Set to", CENTER, 70, 0);	//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(20, 0);	//	write the data to the eeprom for celcius
		TFT.print("Celcius", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		eeprom.write(20, 1);	//	write the data to the eeprom for fahrenheit
		TFT.print("Fahrenheit", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void TempPrecision() {
//  Sets the temperature precision to 0, 1, or 2 decimals
	uint8_t menusize = Sensor_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Temp Precision Entered\n");
	ClearScreenHeader();								//	clear the touch screen
	TFT.print("TEMP PRECISION", CENTER, 10, 0);			//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Temp_Precision);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Temp Precision Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Temp Precision Set", CENTER, 70, 0);		//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(21, 0);	//	write the data to the eeprom for no decimal
		TFT.print("to No Decimal", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		eeprom.write(21, 1);	//	write the data to the eeprom for 1 decimal
		TFT.print("to 1 Decimal", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void TempReadDelay() {
//  Sets the delay between reads of the temperature sensors
	uint8_t menusize = Sensor_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Temp Read Delay Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("TEMP READ DELAY", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Read_Delay);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Temp Read Delay Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Temp Read Delay", CENTER, 70, 0);	//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(22, 10);	//	write the data to the eeprom for 10 seconds between reads
		TFT.print("Set to 10 Sec", CENTER, 130, 0);
		break;
	case 1:
		eeprom.write(22, 20);	//	write the data to the eeprom for 20 sconds between reads
		TFT.print("Set to 20 Sec", CENTER, 130, 0);
		break;
	case 2:
		eeprom.write(22, 30);	//	write the data to the eeprom for 30 sconds between reads
		TFT.print("Set to 30 Sec", CENTER, 130, 0);
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void SensorNames() {
}

//  SENSOR CALIBRATION MENU
//  ***********************************************

void SensorCalib(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("Sensor Calibration Entered\n");
		ClearScreenHeader();									//	clear the touch screen
		TFT.print("SENSOR CALIB", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SENSOR_CALIB_SIZE, Sensor_Calib);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;				//	enables the menu to loop
		MenuLoop(SENSOR_CALIB_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("Sensor Calib Button %d pressed\n\n", ButtonPressed);
		function = Sensor_Calib[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function se	delay(1000);	//	Small delay to allow the user to see the change on the screen
		Serial.println();
	}
}

void FlowSensorCalib(){
}

void SensorAddrConfig(){
}

//  SETUP FLOW SENSOR MENU
//  ***********************************************

void SetupFlowSensor(){
	while (SubMenuLoopFlag == 1)
	{
		Serial.print("Setup Flow Sensor Entered\n");
		ClearScreenHeader();								//	clear the touch screen
		TFT.print("SETUP FLOW SENSOR", CENTER, 10, 0);		//  prints the menu name and centers it
		DrawMenuButtonArray(SETUP_FLOW_SIZE, Setup_Flow);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(SETUP_FLOW_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("User Setup Button %d pressed\n\n", ButtonPressed);
		function = Setup_Flow[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
		Serial.println();
	}
	SubMenuLoopFlag = 1;
}

void FlowOnOff() {
//  Sets the flow sensor on or off

	uint8_t menusize = Setup_Flow[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Flow On/Off Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("FLOW SENSOR ON/OFF", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, On_Off);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	call the function depending on the button pressed
	Serial.printf("Flow On/Off Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("Flow Sensor Set to", CENTER, 70, 0);	//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(27, 1);	//	write the data to the eeprom for CHANGEME
		TFT.print("On", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		eeprom.write(27, 0);	//	write the data to the eeprom for CHANGEME
		TFT.print("Off", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}


void CalibrateFlowSensor() {
//  Sets the calibration of the flow sensor
	//*************************************************************************
	//**  THIS FUNCTION NEEDS BETTER TESTING WITH THE FLOW SENSOR HOOKED UP  **
	//*************************************************************************
	uint8_t menusize = Setup_Flow[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Calibrate Flow Sensor Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("CALIBRATE FLOW", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Yes_No);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	call the function depending on the button pressed
	Serial.printf("Calibrate Flow Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		//	Ask the user to verify to perform the calibration
		menusize = 2;	//  Set how many buttons there are

		Serial.print("Are you sure you want to calibrate the flow sensor\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("ARE YOU SURE?", CENTER, 10, 0);		//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Yes_No);			//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
		if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

		//	call the function depending on the button pressed
		Serial.printf("Verify calibrate button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)
		{
		case 0:
			TFT.print("Calibrating", CENTER, 10, 0);
			TFT.print("Flow Sensor", CENTER, 70, 0);

			Alarm.disable(tempReadID);
			Alarm.disable(flowReadID);
			TFT.print("Taking Reading #", CENTER, 130, 0);
			for (uint8_t i = 0; i <= 4; i++)
			{
				flowRateMax = 0;
				TFT.printNumI(i + 1, CENTER, 190, 0);
				FlowSensorRead();
				delay(1500);
			}
			menusize = 2;
			ClearScreenHeader();
			char buffer[15];
			sprintf(buffer, "Avg Flow = %d", flowPulseTotal / 5);
			TFT.print(buffer, CENTER, 10, 0);
			TFT.print("Set As Normal Flow?", CENTER, 170, 0);
			DrawMenuButtonArray(menusize, Yes_No);			//	draws the menu buttons for the specified menu size and array

			//	loop while we wait for touch screen data
			MenuLoopFlag = 1;	//	enables the menu to loop
			MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
			ClearScreenHeader();

			if (ButtonPressed == menusize - 1) {
				TFT.print("Not Saving", CENTER, CENTER, 0);
				return;		//	return to the previous menu if the back button was pressed
			}
			else {
				TFT.print("Normal Flow", CENTER, 70, 0);
				TFT.print("Rate Saved", CENTER, 130, 0);
				eeprom.write(28, (flowPulseTotal / 5));
				flowRateMax = eeprom.read(28);
				//eeprom.write(27, 1);
				//flowSensorEnable = eeprom.read(27);
			}
			Alarm.enable(tempReadID);
			Alarm.enable(flowReadID);
		default:
			break;
		}
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void SetMinFlow(){
}

//  SYSTEM SETUP MENU
//  ***********************************************

void SystemSetup(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("System Setup Entered\n");
		ClearScreenHeader();									//	clear the touch screen
		TFT.print("SYSTEM SETUP ", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SYSTEM_SETUP_SIZE, System_Setup);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;				//	enables the menu to loop
		MenuLoop(SYSTEM_SETUP_SIZE, 300);	//	call the menu loop with 300ms of delay

		//	call the function depending on the button pressed
		Serial.printf("System Setup Button %d pressed\n\n", ButtonPressed);
		function = System_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
		Serial.println();
	}
	SubMenuLoopFlag = 1;
}

void SerialDebugging(){
//  Sets the serial debugging byte to turn on or off messages sent to the serial port
	uint8_t menusize = System_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Serial Debugging Entered\n");
	ClearScreenHeader();								//	clear the touch screen
	TFT.print("SERIAL DEBUGGING", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Serial_Debugging);	//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	evaluate the button pressed
	Serial.printf("Serial Debugging Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	uint8_t	eeread = eeprom.read(5);	//	read out the eeprom for comparison
		
	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeread = 255;	//	set the data for all on
		//	print to the display to see what was done
		TFT.print("All Debugging is", CENTER, 70, 0);
		TFT.print("On", CENTER, 130, 0);
		break;
	case 1:
		eeread = 0;		//	set the data for all off
		//	print to the display to see what was done
		TFT.print("All Debugging is", CENTER, 70, 0);
		TFT.print("Off", CENTER, 130, 0);
		break;
	case 2:
		TFT.print("Temp Sensor Debug", CENTER, 70, 0);	// print the tft header
		if ((eeread & 1) == 1) {	//  see if the bit flag is set.
			eeread = eeread - 1;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 1;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done
		}
		break;
	case 3:
		TFT.print("Menu Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 2) == 2) {	//  see if the 1st bit flag is set.
			eeread = eeread - 2;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 2;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	case 4:
		TFT.print("Alarm Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 4) == 4) {	//  see if the 1st bit flag is set.
			eeread = eeread - 4;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 4;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	case 5:
		TFT.print("EEPROM Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 8) == 8) {	//  see if the 1st bit flag is set.
			eeread = eeread - 8;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 8;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	case 6:
		TFT.print("Relay Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 16) == 16) {	//  see if the 1st bit flag is set.
			eeread = eeread - 16;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 16;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	case 7:
		TFT.print("System Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 32) == 32) {	//  see if the 1st bit flag is set.
			eeread = eeread - 32;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 32;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	case 8:
		TFT.print("Flow Sensor", CENTER, 10, 0);	// print the tft header
		TFT.print("Debugging", CENTER, 70, 0);	// print the tft header
		if ((eeread & 64) == 64) {	//  see if the 1st bit flag is set.
			eeread = eeread - 64;	//  if it is set, turn it off
			TFT.print("is Disabled", CENTER, 130, 0);	//	print to the display what was done
		}
		else {
			eeread = eeread + 64;	//  if it is not set, turn it on
			TFT.print("is Enabled", CENTER, 130, 0);	//	print to the display what was done				TFT.print("is On", CENTER, 130, 0);
		}
		break;
	default:
		break;
	}
	eeprom.write(5, eeread);		//	write the data to the eeprom
	serialDebug = eeprom.read(5);	//	set serialDebug according to the eeprom
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void EraseEEPROM(){
	//  Erases the EEPROM
	uint8_t menusize = System_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Erase EEPROM Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("Erase EEPROM", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Yes_No);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	call the function depending on the button pressed
	Serial.printf("Erase EEPROM Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		//	Ask the user to verify the EEPROM erase
		menusize = 2;	//  Set how many buttons there are

		Serial.print("Are you sure you want to erase EEPROM\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("ARE YOU SURE?", CENTER, 10, 0);	//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Yes_No);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
		if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

		//	call the function depending on the button pressed
		Serial.printf("Verify erase button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)
		{
		case 0:
			TFT.print("Erasing EEPROM", CENTER, 70, 0);
			eeprom.eraseAll();
			ClearScreenHeader();
			TFT.print("Erase Complete", CENTER, 70, 0);
			break;
		default:
			break;
		}
		break;
	case 1:
		TFT.print("Not Erasing the", CENTER, 70, 0);
		TFT.print("EEPROM", CENTER, 130, 0);
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

void RestoreDefaults(){
	//  Erases the EEPROM
	uint8_t menusize = System_Setup[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("Factory Defaults Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("Factory Defaults", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Yes_No);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	call the function depending on the button pressed
	Serial.printf("Factory Defaults Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		//	Ask the user to verify the EEPROM erase
		menusize = 2;	//  Set how many buttons there are

		Serial.print("Are you sure you want to Restore Defaults\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("ARE YOU SURE?", CENTER, 10, 0);	//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Yes_No);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
		if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

		//	call the function depending on the button pressed
		Serial.printf("Verify restore button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)
		{
		case 0:
			TFT.print("Restoring Factory", CENTER, 70, 0);
			TFT.print("Defaults", CENTER, 130, 0);
			factoryDefaultset();
			ClearScreenHeader();
			TFT.print("Factory Defaults", CENTER, 70, 0);
			TFT.print("Restored", CENTER, 130, 0);
			break;
		default:
			break;
		}
		break;
	case 1:
		TFT.print("Not Restoring", CENTER, 70, 0);
		TFT.print("Defaults", CENTER, 130, 0);
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}

//  EXIT MENU
//  ***********************************************
void ExitMenu(){
	Serial.print("Exiting current menu\n");
	MenuLoopFlag = 0;
	SubMenuLoopFlag = 0;
}

//  Back MENU
//  ***********************************************
void BackMenu() {
	SubMenuLoopFlag = 0;
}

//	DECIMAL TO BCD FUNCTION
//  ***********************************************
uint8_t DecToBcd(uint8_t val)
{
	return ((val / 10 * 16) + (val % 10));
}

//	BCD TO DECIMAL FUNCTION
//  ***********************************************
uint8_t BcdToDec(uint8_t val)
{
	return ((val / 16 * 10) + (val % 16));
}

void Dummy() {
//  Sets the CHANGEME
	String CHANGEME;
	uint8_t menusize = CHANGE_ME[ButtonPressed].Menu_Size;	//	set the menusize

	Serial.print("CHANGEMEMENU Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("CHANGEMEMENUITEM", CENTER, 10, 0);	//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, CHANGE_ME);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize, 300);	//	call the menu loop with 300ms of delay
	if (ButtonPressed == menusize - 1) { return; }	//	return to the previous menu if the back button was pressed

	//	call the function depending on the button pressed
	Serial.printf("CHANGEME Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen
	TFT.print("CHANGEME Set to", CENTER, 70, 0);	//	print the header

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(0, 0);	//	write the data to the eeprom for CHANGEME
		TFT.print("CHANGEME", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	case 1:
		eeprom.write(0, 1);	//	write the data to the eeprom for CHANGEME
		TFT.print("CHANGEME", CENTER, 130, 0);	//	print to the display to see what was done
		break;
	default:
		break;
	}
	delay(1000);	//	Small delay to allow the user to see the change on the screen
}