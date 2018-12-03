#include "TouchMenu.h"
#include "UTFT.h"
#include <SPI.h>
#include "Arduino.h"
#include "EEprom.h"

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


		void ExitMenu();
		void BackMenu();
		void DrawMenuButtonArray(uint8_t buttons, struct MENU_ITEM *currentmenu);
		void MenuLoop(uint8_t menuSize);
		void ReadTouchData();
		void AnalyzeMenuTouchData(int menuSize);
		void ClearScreenHeader();
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
	{ "Brightness",			1, Brightness },
	{ "Time Format",		3, TimeFormat },
	{ "Display Sec",		3, SecondsDisplay },
	{ "Set Date/Time",		0, SetDateTime },
	{ "Back",				7, BackMenu }
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

	MENU_ITEM Seconds_Display[] = {
		{ "Off",			0,	REPLACEME },
		{ "On",				0,	REPLACEME },
		{ "Back",			0,	REPLACEME },
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
	{ "Back",			2, BackMenu }
};

//	Sensor Setup Arrays
MENU_ITEM Sensor_Setup[] = {
	{ "Temp Unit",		2, TempUnit },
	{ "Precision",		2, TempPrecision },
	{ "Read Delay",		2, TempReadDelay },
	{ "Sensor Names",	2, REPLACEME },
	{ "Back",			2, BackMenu }
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
	{ "On/Off",			2, FlowOnOff},
	{ "Calib Flow",		2, CalibrateFlowSensor},
	{ "Set Min Flow",	2, SetMinFlow},
	{ "Back",			2, BackMenu }
};

//	System Setup
MENU_ITEM System_Setup[] = {
	{ "Debugging",			2, REPLACEME },
	{ "Erase EEPROM",		2, REPLACEME },
	{ "Restore Defaults",	2, REPLACEME },
	{ "Back",				2, BackMenu }
};

MENU_ITEM CHANGE_ME[] = {
	{ "DUMMY", 0, REPLACEME },
	{ "DUMMY", 0, REPLACEME }
};

//  ***********************************************
//  TouchArea Arrays
//  ***********************************************

//	4 Button Main Menu Touch Areas
//	button number, xstart, ystart, xend, yend
TOUCH_LOC Buttons_4[] = {
	{1, 135, 68, 345, 108},
	{2, 135, 116, 345, 156},
	{3, 135, 164, 345, 204},
	{4, 135, 212, 345, 252},
	{5, 420, 212, 460, 252}		//	Back button if necessary
};

//	8 Button Main Menu Touch Areas
//	button number, xstart, ystart, xend, yend
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

void MenuLoop(uint8_t menusize) {
	delay(300);
	while (MenuLoopFlag == 1)		//  scans for a button press to do the appropriate action
	{
		menuTimeout++;
		/*		if (menuTimeout == 10000) { menuMode = 0; }	*/	//	this will exit the menu system after approx 20 seconds after a button has not been pushed
		if (Touch.dataAvailable()) {
			ReadTouchData();					//	read the data from the touchscreen
			AnalyzeMenuTouchData(menusize);		//  analyzes the touch screen data to determine what was pushed
			//Serial.printf("menuTimeout = %d\n\n", menuTimeout);
			delay(150);		//	small debounce delay for a touch

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
	else if ((maxbuttons > 4) && (maxbuttons <=8)) {
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
	//Serial.printf("Button %d was pressed\n", ButtonPressed);
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
		MenuLoop(MAIN_MENU_SIZE);	//	call the menu loop

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
		MenuLoop(USER_SETUP_SIZE);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("User Setup Button %d pressed\n\n", ButtonPressed);
		function = User_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

void Brightness(){
	//  Sets the brightness of the lcd screen
	{
		uint8_t menusize = 4;	//  Set how many buttons there are

		Serial.print("Change Brightness Level Entered\n");
		ClearScreenHeader();									//	clear the touch screen
		TFT.print("BRIGHTNESS", CENTER, 10, 0);					//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Backlight_Brightness);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Brightness Level %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen
		byte brightness;
		switch (ButtonPressed)	//	switch depending on which button was pressed
		{
		case 0:
			eeprom.write(25, 255);	//	write the data to the eeprom for High

			brightness = eeprom.read(25);
			analogWrite(TFT_B_LIGHT_PIN, brightness);

			//	print to the display to see what was done
			TFT.print("Backlight Level Set", CENTER, 70, 0);
			TFT.print("to High", CENTER, 130, 0);
			delay(1000);
			break;
		case 1:
			eeprom.write(25, 170);	//	write the data to the eeprom for High

			brightness = eeprom.read(25);
			analogWrite(TFT_B_LIGHT_PIN, brightness);

			//	print to the display to see what was done
			TFT.print("Backlight Level Set", CENTER, 70, 0);
			TFT.print("to Medium", CENTER, 130, 0);
			delay(1000);
			break;
		case 2:
			eeprom.write(25, 85);	//	write the data to the eeprom for High

			brightness = eeprom.read(25);
			analogWrite(TFT_B_LIGHT_PIN, brightness);

			//	print to the display to see what was done
			TFT.print("Backlight Level Set", CENTER, 70, 0);
			TFT.print("to Low", CENTER, 130, 0);
			delay(1000);
			break;
		default:
			break;
		}
		Serial.println();
	}
}

void TimeFormat()
//  Sets the time format to 12h or 24h
{
	uint8_t menusize = 3;	//  Set how many buttons there are

	Serial.print("Change Time Format Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("TIME FORMAT", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Time_Format);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize);	//	call the menu loop

	//	call the function depending on the button pressed
	Serial.printf("Time Format Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
		case 0:
			eeprom.write(23, 0);	//	write the data to the eeprom for 24 hour

			//	print to the display to see what was done
			TFT.print("Time Format Set to", CENTER, 70, 0);
			TFT.print("24 Hour", CENTER, 130, 0);
			delay(1000);
			break;
		case 1:
			eeprom.write(23, 1);	//	write the data to the eeprom for 12 hour

			//Need to add a check to make sure the data was written

			//	print to the display to see what was done
			TFT.print("Time Format Set to", CENTER, 70, 0);
			TFT.print("12 Hour", CENTER, 130, 0);
			delay(1000);
			break;
		default:
			break;
	}
	Serial.println();
}

void SecondsDisplay(){
	//  Set how many buttons there are
	uint8_t menusize = 3;

	Serial.print("Change Seconds Display Entered\n");
	ClearScreenHeader();								//	clear the touch screen
	TFT.print("SECONDS DISPLAY", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, Seconds_Display);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;		//	enables the menu to loop
	MenuLoop(menusize);		//	call the menu loop

	//	call the function depending on the button pressed
	Serial.printf("Seconds Display %d pressed\n\n", ButtonPressed);
	ClearScreenHeader();								//	clear the touch screen

	switch (ButtonPressed)
	{
	case 0:
	eeprom.write(24, 0);
		TFT.print("Seconds Display set", CENTER, 70, 0);
		TFT.print("to Off", CENTER, 130, 0);
		delay(1000);
		break;
	case 1:
		eeprom.write(24, 1);
		TFT.print("Seconds Display set", CENTER, 70, 0);
		TFT.print("to On", CENTER, 130, 0);
		delay(1000);
		break;
	default:
		break;
	}
	Serial.println();
}

void SetDateTime(){
}

//  TIMER SETUP MENU
//  ***********************************************

void TimerSetup(){
	while (SubMenuLoopFlag == 1)
	{
		Serial.print("Timer Setup Entered\n");
		ClearScreenHeader();								//	clear the touch screen
		TFT.print("TIMER SETUP", CENTER, 11, 0);	//  prints the menu name and centers it
		DrawMenuButtonArray(TIMER_SETUP_SIZE, Timer_Setup);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(TIMER_SETUP_SIZE);
		
		//	call the function depending on the button pressed
		Serial.printf("Timer Setup Button %d pressed\n\n", ButtonPressed);
		function = Timer_Setup[ButtonPressed].function;
		(function)();
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

	delay(2500);
}

//  SENSOR SETUP MENU
//  ***********************************************

void SensorSetup(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("Sensor Setup Entered\n");

		ClearScreenHeader();								//	clear the touch screen
		TFT.print("SENSOR SETUP", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SENSOR_SETUP_SIZE, Sensor_Setup);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(SENSOR_SETUP_SIZE);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Sensor Setup Button %d pressed\n\n", ButtonPressed);
		function = Sensor_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

void TempUnit() {
	//  Sets the temperature unit to celcius of fahrenheit
	{
		uint8_t menusize = 3;	//  Set how many buttons there are

		Serial.print("Change Temp Unit Entered\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("TEMP UNIT", CENTER, 10, 0);		//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Temp_Unit);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Temp Unit Button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)	//	switch depending on which button was pressed
		{
		case 0:
			eeprom.write(20, 0);	//	write the data to the eeprom for celcius

			//	print to the display to see what was done
			TFT.print("Temp Unit Set to", CENTER, 70, 0);
			TFT.print("Celcius", CENTER, 130, 0);
			delay(1000);
			break;
		case 1:
			eeprom.write(20, 1);	//	write the data to the eeprom for fahrenheit

			//	print to the display to see what was done
			TFT.print("Temp Unit Set to", CENTER, 70, 0);
			TFT.print("Fahrenheit", CENTER, 130, 0);
			delay(1000);
			break;
		default:
			break;
		}
		Serial.println();
	}
}

void TempPrecision() {
	//  Sets the temperature precision to 0, 1, or 2 decimals
	{
		uint8_t menusize = 3;	//  Set how many buttons there are

		Serial.print("Temp Precision Entered\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("TEMP PRECISION", CENTER, 10, 0);		//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Temp_Precision);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Temp Precision Button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)	//	switch depending on which button was pressed
		{
		case 0:
			eeprom.write(21, 0);	//	write the data to the eeprom for no decimal

			//	print to the display to see what was done
			TFT.print("Temp Precision Set", CENTER, 70, 0);
			TFT.print("to No Decimal", CENTER, 130, 0);
			delay(1000);
			break;
		case 1:
			eeprom.write(21, 1);	//	write the data to the eeprom for 1 decimal

			//	print to the display to see what was done
			TFT.print("Temp Precision Set", CENTER, 70, 0);
			TFT.print("to 1 Decimal", CENTER, 130, 0);
			delay(1000);
			break;
		default:
			break;
		}
		Serial.println();
	}
}

void TempReadDelay() {
	//  Sets the delay between reads of the temperature sensors
	{
		uint8_t menusize = 4;	//  Set how many buttons there are

		Serial.print("Temp Read Delay Entered\n");
		ClearScreenHeader();							//	clear the touch screen
		TFT.print("TEMP READ DELAY", CENTER, 10, 0);		//  prints the menu name and centers it
		DrawMenuButtonArray(menusize, Read_Delay);		//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;	//	enables the menu to loop
		MenuLoop(menusize);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Temp Read Delay Button %d pressed\n", ButtonPressed);
		ClearScreenHeader();	//	clear the touch screen

		switch (ButtonPressed)	//	switch depending on which button was pressed
		{
		case 0:
			eeprom.write(22, 10);	//	write the data to the eeprom for 10 seconds between reads

			//	print to the display to see what was done
			TFT.print("Temp Read Delay", CENTER, 70, 0);
			TFT.print("Set to 10 Sec", CENTER, 130, 0);
			delay(1000);
			break;
		case 1:
			eeprom.write(22, 20);	//	write the data to the eeprom for 20 sconds between reads

			//	print to the display to see what was done
			TFT.print("Temp Read Delay", CENTER, 70, 0);
			TFT.print("Set to 20 Sec", CENTER, 130, 0);
			delay(1000);
			break;
		case 2:
			eeprom.write(22, 30);	//	write the data to the eeprom for 30 sconds between reads

			//	print to the display to see what was done
			TFT.print("Temp Read Delay", CENTER, 70, 0);
			TFT.print("Set to 30 Sec", CENTER, 130, 0);
			delay(1000);
			break;
		default:
			break;
		}
		Serial.println();
	}
}

void SensorNames() {
}

//  SENSOR CALIBRATION MENU
//  ***********************************************

void SensorCalib(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("Sensor Calibration Entered\n");

		ClearScreenHeader();								//	clear the touch screen
		TFT.print("SENSOR CALIB", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SENSOR_CALIB_SIZE, Sensor_Calib);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(SENSOR_CALIB_SIZE);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("Sensor Calib Button %d pressed\n\n", ButtonPressed);
		function = Sensor_Calib[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

void FlowSensorCalib(){
}

void SensorAddrConfig(){
}

//  SETUP FLOW SENSOR MENU
//  ***********************************************

void SetupFlowSensor(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("Setup Flow Sensor Entered\n");

		ClearScreenHeader();								//	clear the touch screen
		TFT.print("SETUP FLOW SENSOR", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SETUP_FLOW_SIZE, Setup_Flow);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(SETUP_FLOW_SIZE);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("User Setup Button %d pressed\n\n", ButtonPressed);
		function = Setup_Flow[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

void FlowOnOff() {
}

void CalibrateFlowSensor(){
}

void SetMinFlow(){
}

void DisableFlow(){
}

//  SYSTEM SETUP MENU
//  ***********************************************

void SystemSetup(){
	while (SubMenuLoopFlag == 1) {
		Serial.print("System Setup Entered\n");

		ClearScreenHeader();								//	clear the touch screen
		TFT.print("SYSTEM SETUP SENSOR", CENTER, 10, 0);				//  prints the menu name and centers it
		DrawMenuButtonArray(SYSTEM_SETUP_SIZE, System_Setup);	//	draws the menu buttons for the specified menu size and array

		//	loop while we wait for touch screen data
		MenuLoopFlag = 1;			//	enables the menu to loop
		MenuLoop(SYSTEM_SETUP_SIZE);	//	call the menu loop

		//	call the function depending on the button pressed
		Serial.printf("System Setup Button %d pressed\n\n", ButtonPressed);
		function = System_Setup[ButtonPressed].function;		//	set the function to be called when the button is pressed
		(function)();	//	call the function set
	}
	SubMenuLoopFlag = 1;
}

void SerialDebugging(){
}

void EraseEEPROM(){
}

void RestoreDefaults(){
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


void Dummy()
//  Sets the CHANGEME
{
	String CHANGEME;
	uint8_t menusize = 3;	//  Set how many buttons there are

	Serial.print("CHANGEMEMENU Entered\n");
	ClearScreenHeader();							//	clear the touch screen
	TFT.print("CHANGEMEMENUITEM", CENTER, 10, 0);		//  prints the menu name and centers it
	DrawMenuButtonArray(menusize, CHANGE_ME);		//	draws the menu buttons for the specified menu size and array

	//	loop while we wait for touch screen data
	MenuLoopFlag = 1;	//	enables the menu to loop
	MenuLoop(menusize);	//	call the menu loop

	//	call the function depending on the button pressed
	Serial.printf("CHANGEME Button %d pressed\n", ButtonPressed);
	ClearScreenHeader();	//	clear the touch screen

	switch (ButtonPressed)	//	switch depending on which button was pressed
	{
	case 0:
		eeprom.write(0, 0);	//	write the data to the eeprom for 24 hour

		//	print to the display to see what was done
		TFT.print("CHANGEME Set to", CENTER, 70, 0);
		TFT.print("CHANGEME", CENTER, 130, 0);
		delay(1000);
		break;
	case 1:
		eeprom.write(0, 1);	//	write the data to the eeprom for 12 hour

		//Need to add a check to make sure the data was written

		//	print to the display to see what was done
		TFT.print("CHANGEME Set to", CENTER, 70, 0);
		TFT.print("CHANGEME", CENTER, 130, 0);
		delay(1000);
		break;
	default:
		break;
	}
	Serial.println();
}