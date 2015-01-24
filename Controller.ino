#include "Alarms.h"
#include <Time.h>               //Time Library
#include <TimeAlarms.h>			//TimeAlarms Library
#include <DS1307RTC.h>          //DS RTC library
#include <OneWire.h>            //OneWire Library for the DS Sensors
#include <DallasTemperature.h>  //Dallas Temperature library
#include <LiquidCrystal_I2C.h>  //LCD I2C library
#include <Wire.h>               //I2C Wire library


//  INITIALIZE THE EEPROM
//  ***********************************************

#define EEPROM_DEV_ADDR 0x50	//  Set the address of the EEPROM

int eePointer = 0;			//  Sets the pointer location to 0 initially
byte data = 0;				//  Sets the value of data to be written to 0 initially


//  CONFIGURATION SETUP
//  ***********************************************
byte e2Empty;				//	initializes the byte e2Emtpy
byte configID;				//  initializes the byte configID
byte serialDebug;			//	initializes the byte serialDebug
byte tempType;				//  initializes the byte tempTye
byte tempPrecision;			//	initializes the byte tempPrecision
byte tempReadDelay;			//	initializes the byte tempReadDelay
byte timeFormat;			//	initializes the byte timeFormat
byte backlightLevel;		//	initializes the byte backlightLevel
int version = 0;				//  Sets the version number for the current program
int build = 14;					//  Sets the build number for the current program
int today = 0;					//  Sets the today to the current date to display on the RTC

//  INITIALIZE THE LCD
//  ***********************************************
#define LCD_DEV_ADDR 0x3F		//  Set the address of the LCD screen	(Both systems)

// Define the LCD Pins for the I2C
#define B_Light 3
#define En 2
#define Rw 1
#define Rs 0
#define D4 4
#define D5 5
#define D6 6
#define D7 7
LiquidCrystal_I2C lcd(LCD_DEV_ADDR,En,Rw,Rs,D4,D5,D6,D7);  // Pass the lcd pins for the LiquidCrystal_I2C library to use


//  SETUP CUSTOM CHARACTERS
byte degree[8] = {B01100,B10010,B10010,B01100,B00000,B00000,B00000,};  //  set the lcd char for the degree symbol
byte rarrow[8] = {B00000,B01000,B01100,B01110,B01100,B01000,B00000,};  //  set the lcd char for the right arrow symbol
byte uarrow[8] = {B00000,B00000,B00100,B01110,B11111,B00000,B00000,};  //  set the lcd char for the up arrow symbol
byte larrow[8] = {B00000,B00010,B00110,B01110,B00110,B00010,B00000,};  //  set the lcd char for the left arrow symbol
byte darrow[8] = {B00000,B00000,B11111,B01110,B00100,B00000,B00000,};  //  set the lcd char for the down arrow symbol
byte bell[8] = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,};  //  set the lcd char for the timer bell symbol
byte relon[8] = {B11100,B10100,B11100,B00000,B00111,B00101,B00101,};  //  set the lcd char for the relay on symbol


//  DEFINE BUTTON PINS
//  ***********************************************
#define upButton 53			//  set the up button to pin 53  Orange
#define downButton 50		//  set the down button to pin 50  Blue
#define leftButton 52		//  set the right button to pin 52  Yellow
#define rightButton 51		//  set the left button to pin 51  Green

volatile int menuMode = 0;	//  allow the variable state to change the led on and off

const int button[]={42,43,44,45,46,47,48,49};	//  sets the pins for Button0 - button 7 respectively


//  INITIALIZE THE MENU VARIABLES
//  ***********************************************
int mPoint = 0;			//  current main menu pointer position
int mCur = 1;			//  current position of the main menu cursor
int mStart = 0;			//  current starting cursor
int mLevel = 0;			//  current menu level.  main menu level=0, menu items=1, menu item selected=2
int m1Sel = 0;			//  current selection for menu level 1
int m2Sel = 0;			//	current selection for menu level 2
int m3Sel = 0;			//  current selection for menu level 3
int m0Start = 0;		//	starting cursor position for mLevel0
int m1Start = 0;		//	starting cursor position for mLevel1
int m2Start = 0;		//  starting cursor position for mLevel2
int m3Start = 0;		//  starting cursor position for mLevel3
int miMax = 0;			//  current selected menu item for purposes of up and down movement
int mRet = 0;			//	variable to determine if the menu has just started.  if it has then it calls MenuLoop, otherwise it returns

char* m0Items[]={"", "System Config", "Timers Setup", "Sensor Addr Config","Calibration",""};  //  setup menu items here  Min Cursor = 0 and Max Cursor = 3
	char* m1Items0[]={"", "Temp Type", "Temp Precision", "Time Format", "B Light Brightness", "Set Date/Time", "Serial Debugging", "Temp Read Delay", "Erase EEPROM", ""};  //  setup menu item 1 for System Config Min 0 Max 6
		char* m2Items00[]={"", "Celsius", "Fahrenheit", ""};
		char* m2Items01[]={"", "No Decimal", "1 Decimal", ""};
		char* m2Items02[]={"", "24 Hour", "12 Hour", ""};
		char* m2Items03[]={"", "Set BL Brightness", ""};
		char* m2Items04[]={"", "Need Date/Time Here", ""};
		char* m2Items05[]={"", "Disabled", "All", "Temp Sensors", "Menu", "Alarm", "EEPROM", "", "System", ""};
		char* m2Items06[]={"", "Set Temp Read Delay", "" };
		char* m2Items07[]={"", "Exit Erase EEPROM", "Erase EEPROM", "" };
		char* m1Items1[] = { "", "Set Timer 1", "Set Timer 2", "Set Timer 3", "Set Timer 4", "Set Timer 5", "Set Timer 6", "Set Timer 7", "Set Timer 8", "" };  //  setup menu item 2 for Timer Setup Min 0 Max 3
		char* m2Items10[]={"", "Edit", "Exit", ""};
		char* m2Items11[]={"", "Edit", "Exit", ""};
		char* m2Items12[]={"", "Edit", "Exit", ""};
		char* m2Items13[]={"", "Edit", "Exit", ""};
		char* m2Items14[]={"", "Edit", "Exit", ""};
		char* m2Items15[]={"", "Edit", "Exit", ""};
		char* m2Items16[]={"", "Edit", "Exit", ""};
		char* m2Items17[]={"", "Edit", "Exit", ""};
	char* m1Items2[]={"", "Temp Sens 1 Add", "Temp Sens 2 Add", "Temp Sens 3 Add", "Temp Sens 4 Add", ""};  //  setup menu item 3 for Timer Setup Min 0 Max 3
		char* m2Items20[]={"", "Set Sens 1 Address", "Exit", ""};
		char* m2Items21[]={"", "Set Sens 2 Address", "Exit", ""};
		char* m2Items22[]={"", "Set Sens 3 Address", "Exit", ""};
		char* m2Items23[]={"", "Set Sens 4 Address", "Exit", ""};
	char* m1Items3[]={"", "Temp 1 Calib", "Temp 2 Calib", "Temp 3 Calib", "Temp 4 Calib", "Flow Calib", ""};  //  setup menu item 4 for Timer Setup Min 0 Max 4
		char* m2Items30[]={"", "Calibrate Sensor 1", "Exit", ""};
		char* m2Items31[]={"", "Calibrate Sensor 2", "Exit", ""};
		char* m2Items32[]={"", "Calibrate Sensor 3", "Exit", ""};
		char* m2Items33[]={"", "Calibrate Sensor 4", "Exit", ""};
		char* m2Items34[]={"", "Calibrate Flow Sens", "Exit", ""};


//  INITIALIZE THE DS18B20 TEMPERATURE SENSORS
//  ***********************************************

// define the DS18B20 global variables
const int ONE_WIRE_BUS[]={12};		// the array to define which pins you will use for the busses ie {2,3,4,5};

#define TEMPERATURE_PRECISION 10
#define NUMBER_OF_BUS 1				// how many busses will you use for the sensors

OneWire *oneWire[NUMBER_OF_BUS];	//  Setup oneWire instances to communicate with any OneWire Devices

DallasTemperature *sensors[NUMBER_OF_BUS];  // Pass onewire reference to Dallas temperatures.

DeviceAddress tempDeviceAddress[8];		//  arrays to hold device addresses

int numberOfDevices[NUMBER_OF_BUS];		//  define the variable to store the number of busses

int RTC_Status=1;


//  INITIALIZE THE ALARM Variables
//  ***********************************************

byte AlarmEnable;			//  byte for storing all 8 alarm's enable flags as bits
byte AlarmState;			//  byte for storing all 8 alarm's state flags as bits
byte AlarmType[8];			//  type of alarm 0=Day Lights, 1=Night Lights, ""room to expand""
byte AlarmRelay[8];			//  # of the relay this alarm will trigger
byte AlarmHourOn[8];		//  hour time the alarm will come on at
byte AlarmMinOn[8];			//	minute time the alarm will come on at
byte AlarmHourOff[8];		//	hour time the alarm will go off at
byte AlarmMinOff[8];		//  minute time the alarm will go off at

AlarmID_t ReadDelay_ID;		//  delay between reading the temperature sensors
AlarmID_t AlarmIDOn[8];		//  alarm IDs for each alarm's On event
AlarmID_t AlarmIDOff[8];	//	alarm IDs for each alarm's Off event


//  INITIALIZE THE RELAYS
//  ***********************************************

int relayPins[] = {23,24,25,26,27,28,29,30};		//  Initialize the relay pins
int relayCount = 8;		//  Set the number of relays


void setup()
{
	//  SETUP THE SERIAL PORT
	serialDebug = readEEPROM(5);		//	reads out user setting to turn serial debugging on for each type of debugging
	Serial.begin(115200);				//  start the serial port if debugging is on
	Serial.println();

	//  SETUP THE RTC
	setSyncProvider(RTC.get);		//  this function get the time from the RTC
	if (timeStatus() != timeSet)		//  checks to see if it can read the RTC
	{
		RTC_Status = 0;
		Serial.println("Unable to get the RTC");
		Serial.println();
	}
	else{ Serial.println("RTC has set the system time"); }

	//  READ THE VARIABLES OUT OF THE EEPROM
	
		//  READ NON USER SETTINGS FROM EEPROM
		e2Empty = readEEPROM(0);			//	reads the e2Empty out to determine if it needs to set defaults
		if (e2Empty == 0){factoryDefaultset();}
		configID = readEEPROM(0);			//  reads the configID out

		//  READ USER SETTINGS FROM EEPROM
		tempType = readEEPROM(20);			//  reads out user setting to selects between 0 = Celsius or 1 = Fahrenheit
		tempPrecision = readEEPROM(21);		//	reads out user setting to selece the decimal precision of the temp sensors 0 = No Decimal or 1 = 1 Decimal
		tempReadDelay = readEEPROM(22);		//	reads out user setting for the amount of time in seconds between reading the temp sensors
		timeFormat = readEEPROM(23);		//	reads out user setting for the time format 0 = 24 hour and 1 = 12 hour
		backlightLevel = readEEPROM(24);	//	reads out the user setting to control the backlight level

		//  READ ALARM SETTINGS FROM EEPROM AND SETUP THE ALARMS IN THE TIMEALARMS LIBRARY

		ReadDelay_ID = Alarm.timerRepeat(tempReadDelay, DS18B20_Read);		//	sets an alarm to read the temp sensors at the specified delay and returns the Alarm_ID to ReadDelayID
		AlarmEnable = readEEPROM(100);		//	reads out the byte for the enable flags for all 8 alarms
		AlarmState = readEEPROM(101);		//	reads out the byte for the state flags for all 8 alarms
	
		if ((serialDebug & 4) == 4)
		{
			Serial.println();
			Serial.println("********ALARM EEPROM READING********");
			Serial.println("ID IDON IDOFF Type Rly   ON     OFF");
		}

		for (int id = 0; id < 8; id++)		//  read each of the alarms values out of the EEPROM
		{
			if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop			
			AlarmType[id] = readEEPROM(102 + (id * 6));
			AlarmRelay[id] = readEEPROM(103 + (id * 6));
			AlarmHourOn[id] = readEEPROM(104 + (id * 6));
			AlarmMinOn[id] = readEEPROM(105 + (id * 6));
			AlarmHourOff[id] = readEEPROM(106 + (id * 6));
			AlarmMinOff[id] = readEEPROM(107 + (id * 6));
			AlarmIDOn[id] = Alarm.alarmRepeat(AlarmHourOn[id], AlarmMinOn[id], 0, AlarmON);
			AlarmIDOff[id] = Alarm.alarmRepeat(AlarmHourOff[id], AlarmMinOff[id], 30, AlarmOFF);

			if ((serialDebug & 4) == 4)
			{
				Serial.print(id);
				Serial.print("    ");
				Serial.print(AlarmIDOn[id]);
				if (AlarmIDOn[id] >= 10){ Serial.print("   "); }
				else { Serial.print("    "); }
				Serial.print(AlarmIDOff[id]);
				if (AlarmIDOff[id] >= 10){ Serial.print("   "); }
				else { Serial.print("    "); }
				Serial.print(AlarmType[id]);
				Serial.print("    ");
				Serial.print(AlarmRelay[id]);
				Serial.print("   ");

				if (AlarmHourOn[id] < 10){ Serial.print(" "); }
				Serial.print(AlarmHourOn[id]);
				Serial.print(":");
				if (AlarmMinOn[id] < 10){Serial.print("0");
					Serial.print(AlarmMinOn[id]);}
				else{ Serial.print(AlarmMinOn[id]); }
				Serial.print("  ");
				if (AlarmHourOff[id] < 10){ Serial.print(" "); }
				Serial.print(AlarmHourOff[id]);
				Serial.print(":");
				if (AlarmMinOff[id] < 10){
					Serial.print("0");
					Serial.println(AlarmMinOff[id]);
				}
				else{ Serial.println(AlarmMinOff[id]); }
			}
		}
		serialDebug = readEEPROM(5);		//	read out the serial debug againg in case it was disable during the alarm print

		if ((serialDebug & 4) == 4)
		{
			int rd;
			Serial.println();
			Serial.print("Alarm Enable : ");
			Serial.println(AlarmEnable, BIN);
			Serial.print("Alarm State : ");
			Serial.println(AlarmState, BIN);
			Serial.print("Num of Alarms : ");
			rd = Alarm.count();
			Serial.println(rd);
			Serial.print("Read Read Delay ID = ");
			Serial.print(ReadDelay_ID);
			Serial.print(" : ");
			rd = Alarm.read(ReadDelay_ID);
			Serial.println(rd);
			Serial.println();

		}
		
	//  SETUP THE BUTTONS
	pinMode(upButton, INPUT);		//  sets the UpButton to an input
	pinMode(downButton, INPUT);		//  sets the DownButton to an input
	pinMode(leftButton, INPUT);		//  sets the LeftButton to an input
	pinMode(rightButton, INPUT);	//  sets the RightButton to an input
	
	for(int b = 0; b < 8; b++){pinMode(button[b], INPUT);}	//  sets Button0-7 pins as inputs
		
	attachInterrupt(4, MenuButtonPress, RISING);		//  Attaches int.4, pin 19(RX1) and sets it to trigger on a low input from the menu button
		
	//  SETUP THE RELAYS OUTPUTS
	for(int relay = 0; relay < relayCount; relay++){pinMode(relayPins[relay], OUTPUT);}
	for(int relay = 0; relay < relayCount; relay++){digitalWrite(relayPins[relay], HIGH);}

	//  SETUP THE LCD SCREEN
	lcd.begin(20,4);						//  setup the LCD's number of columns and rows
	lcd.createChar(1, degree);				//  init custom characters as numbers
	lcd.createChar(2, rarrow);				//  init custom characters as numbers
	lcd.createChar(3, uarrow);				//  init custom characters as numbers
	lcd.createChar(4, larrow);				//  init custom characters as numbers
	lcd.createChar(5, darrow);				//  init custom characters as numbers
	lcd.createChar(6, bell);				//  init custom characters as numbers
	lcd.createChar(7, relon);				//  init custom characters as numbers
	lcd.setBacklightPin(B_Light,POSITIVE);  //  set the backlight pin and polarity
	lcd.setBacklight(HIGH);					//  toggle the backlight on
	
	START_SCREEN();		//  call the start up screen function
	
	lcd.setCursor(0,3);
	lcd.print("--------");	//  set the initial display for the relays
	
	lcd.setCursor(0,2);
	lcd.write(byte(2));
	lcd.write(byte(3));
	lcd.write(byte(4));
	lcd.write(byte(5));
	lcd.write(byte(6));
	lcd.write(byte(7));
	
	//  SETUP THE DS18B20 SENSORS
	//  Check to see how many sensors are on the busses
	for(int i=0;i<NUMBER_OF_BUS; i++)   //search each bus one by one
	{
		oneWire[i] = new OneWire(ONE_WIRE_BUS[i]);
		sensors[i] = new DallasTemperature(oneWire[i]);
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();
		
		if ((serialDebug & 1) == 1)
		{
			Serial.print("Locating devices...");
			Serial.print("Found ");
			Serial.print(numberOfDevices[i], DEC);
			Serial.print(" devices on port ");
			Serial.println(ONE_WIRE_BUS[i], DEC);
		}
		for(int j=0;j<numberOfDevices[i]; j++)
		{
			// Search the wire for address
			if(sensors[i]->getAddress(tempDeviceAddress[j], j))
			{
				
				if ((serialDebug & 1) == 1)
				{
					Serial.print("Found device ");
					Serial.print(j, DEC);
					Serial.print(" with address: ");
					printAddress(tempDeviceAddress[j]);
					Serial.println("");
				}

				// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
				sensors[i]->setResolution(tempDeviceAddress[j], TEMPERATURE_PRECISION);
			}
			else{
				if ((serialDebug & 1) == 1)
				{
					Serial.print("Found ghost device at ");
					Serial.print(j, DEC);
					Serial.print(" but could not detect address. Check power and cabling");
				}
			}
		}
		delay(200);
	}
	if ((serialDebug & 1) == 1){ Serial.println(); }
	DS18B20_Read();
	Serial.println("Starting Loop");
	Serial.println();
}

void loop()
{
	if (menuMode == 1)
	{
		Serial.println("Entering Menu");		//  calls the MenuTitle as long as menuMode = 1
		MenuTitle();
		Serial.println("Exiting Menu");
	}
	if (RTC_Status==1){Display_Date();}						//  only calls Display_Date if the RTC has been set
	if (RTC_Status==1){LCD_Time_Display();}					//  only calls LCD_Time_Display if the RTC has been set
	Alarm.delay(1000);										//  uses the Alarm.delay to use the timer
}

void AlarmON()
{	
	byte id;
	id = Alarm.getTriggeredAlarmId();
	id = ((AlarmIDOn[id]-1)/2);
	Serial.print("Alarm ID = ");
	Serial.println(id);
	Serial.println("Alarm: - turn lights ON");
	digitalWrite(relayPins[id], LOW);
	lcd.setCursor(id, 3);
	lcd.print("+");
	if ((serialDebug & 4) == 4)
	{
		int trigger;
		trigger = Alarm.getNextTrigger();
		Serial.print("Next trigger after ALRM ON: ");
		Serial.println(trigger);
		Serial.println();
	}
}

void AlarmOFF()
{
	byte id;
	id = Alarm.getTriggeredAlarmId();
	id = ((AlarmIDOn[id] - 2) / 2);
	Serial.print("Alarm ID = ");
	Serial.println(id);
	Serial.println("Alarm: - turn lights OFF");
	digitalWrite(relayPins[id], HIGH);
	lcd.setCursor(id, 3);
	lcd.print("-");
	if ((serialDebug & 4) == 4)
	{
		int trigger;
		trigger = Alarm.getNextTrigger();
		Serial.print("Next trigger after ALRM OFF: ");
		Serial.println(trigger);
	}
}

void RL_Toggle()
{
	for (int relay = 0; relay < relayCount; relay++){
		digitalWrite(relayPins[relay], LOW);
		lcd.setCursor(relay,3);
		lcd.print("+");
		delay(100);}
	for (int relay = 0; relay < relayCount; relay++){		
		digitalWrite(relayPins[relay], HIGH);
		lcd.setCursor(relay,3);
		lcd.print(" ");
		delay(100);}
	for (int relay = 0; relay < relayCount; relay++){
	digitalWrite(relayPins[relay], HIGH);
	delay(100);}
}

void START_SCREEN()
{
	lcd.setCursor(3,0);
	lcd.print("GLITCHSNIFFER'S");
	lcd.setCursor(6,1);
	lcd.print("AQUARIUM");
	lcd.setCursor(5,2);
	lcd.print("CONTROLLER");
	lcd.setCursor(4,3);
	lcd.print("VERSION ");
	lcd.print(version);
	lcd.print(".");
	if (build < 10){lcd.print("0");}
	lcd.print(build);
	delay(1000);
	lcd.setCursor(0,0);
	lcd.clear();
}
	
void LCD_Time_Display()
{
	if ((serialDebug & 32) == 32){ Serial.println(second()); }
	lcd.setCursor(0,0);
	lcd.print("           ");
	switch (timeFormat)
	{
		case 0:
			if(hour() >= 10){lcd.setCursor(1,0);}
				else{lcd.setCursor(1,0);
				lcd.print('0');}
			lcd.print(hour());
			break;
		case 1:
			if(hourFormat12() >=10){lcd.setCursor(0,0);}
				else{lcd.setCursor(1,0);}
			lcd.print(hourFormat12());
			break;
	}
		
	lcd.print(":");

	if(minute()<10){lcd.print('0');
	lcd.print(minute());}
	else{lcd.print(minute());}
	lcd.print(":");
	
	if(second()<10){lcd.print('0');
	lcd.print(second());}
	else{lcd.print(second());}

	if(isAM()==1 && timeFormat==1){lcd.print("AM");}
	if(isAM()==0 && timeFormat==1){lcd.print("PM");}
}

void Display_Date()
{
	if(day()==today){return;}		//	if the day hasn't changed, dont refresh it	
	else{
	lcd.setCursor(0,1);
	lcd.print("           ");
	if(month() >=10){lcd.setCursor(0,1);}
		else{lcd.setCursor(1,1);}
	lcd.print(month());
	lcd.print("/");
	lcd.print(day());
	lcd.print("/");
	lcd.print(year());
	today = day();}					//	set the day = to today so that it doesn't refresh the display with it until tomorrow
}

void DS18B20_Read()
{
	int c;
	//  Read the DS sensors found in void setup
	for (int i = 0; i < NUMBER_OF_BUS; i++)   // poll every bus
	{
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();
		sensors[i]->requestTemperatures();
		// print the device information
		for (int j = 0; j < numberOfDevices[i]; j++)    // poll devices connect to the bus
		{
			sensors[i]->getAddress(tempDeviceAddress[j], j);
			if ((serialDebug & 1) == 1)
			{
				printAddress(tempDeviceAddress[j]);      //print ID
				Serial.print(";");
			}

			float tempC = sensors[i]->getTempC(tempDeviceAddress[j]);
			float tempF = sensors[i]->getTempF(tempDeviceAddress[j]);

			if ((serialDebug & 1) == 1)
			{
				Serial.print(j);
				Serial.print(" - ");
			}
			switch (tempPrecision)
			{
			case 0:
				lcd.setCursor(13, j);	//  Set the lcd cursor depending on what sensor your are reading
				lcd.print("S");
				lcd.print(j + 1);			//  print the sensor number
				lcd.print(("   "));		//  clear the display to prevent extra characters from interfering when F rolls from 3 to 2 digits

				//  determine if padding is needed for temps higher than 3 digits
				if (tempC >= 100 || tempF >= 100){ lcd.setCursor(15, j); }
				else{ lcd.setCursor(16, j); }
				break;
			case 1:
				lcd.setCursor(11, j);	//  Set the lcd cursor depending on what sensor your are reading
				lcd.print("S");
				lcd.print(j + 1);			//  print the sensor number
				lcd.print(("   "));		//  clear the display to prevent extra characters from interfering when F rolls from 3 to 2 digits

				//  determine if padding is needed for temps higher than 3 digits
				if (tempC >= 100 || tempF >= 100){ lcd.setCursor(13, j); }
				else{ lcd.setCursor(14, j); }
				break;
			}

			//  print the temp to the LCD screen in Celsius or Fahrenheit depending on what the user set in TempType	
			if (tempType == 0)
			{
				lcd.print(tempC, tempPrecision);
				lcd.write(byte(1));
				lcd.print("C");
			}
			else
			{
				lcd.print(tempF, tempPrecision);
				lcd.write(byte(1));
				lcd.print("F");
			}

			//  send the temps to the serial port

			if ((serialDebug & 1) == 1)
			{
				Serial.print(tempC, tempPrecision);
				Serial.print("C ");
				Serial.print(tempF, tempPrecision);
				Serial.println("F");
			}
		}
		if ((serialDebug & 1) == 1){ Serial.println(); }
	}
}

void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

void MenuButtonPress()
{
	menuMode = !menuMode;		//  handler toggle for the menuMode after a button press interrupt
}

void writeEEPROM(int address, byte data)
{
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((int)(address >> 8));			//  writes the first byte of the pointer address to the device
	Wire.write((int)(address & 0xFF));			//  writes the second byte of the pointer address to the device
	Wire.write(data);							//  writes the byte data to the EEPROM at the address specified above
	Wire.endTransmission();						//  stops the write communication on the I2C
	delay(10);
	if ((serialDebug & 8) == 8)
	{
		Serial.print("Writing to address ");
		Serial.print(address);
		Serial.print(" - ");
		Serial.println(data);
	}
}

byte readEEPROM(int address)
{
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((int)(address >> 8));			//  writes the first byte of the pointer address to the device
	Wire.write((int)(address & 0xFF));			//  writes the second byte of the pointer address to the device
	Wire.endTransmission(); 					//  stops the write communication on the I2C
	Wire.requestFrom(EEPROM_DEV_ADDR, 1);		//  gets 1 byte of data from the device
	data = Wire.read();							//  sets the value read to data
	if ((serialDebug & 8) == 8)
	{
		Serial.print("Reading from address ");
		Serial.print(address);
		Serial.print(" - ");
		Serial.println(data);
	}
	return data;								//  returns data to the previous call
}

void factoryDefaultset()
{
	Serial.println("Writing Factory Defaults to EEPROM");

	//  Non User Settings
	writeEEPROM(0, 1);		//	writes the e2Empty value = 1 or Set
	writeEEPROM(1, 1);		//  determines if this is the first run or not
	writeEEPROM(2, 14);		//  writes the build year
	writeEEPROM(3, 10);		//  writes the build month
	writeEEPROM(4, 12);		//  writes the build day
	writeEEPROM(5, 0);		//	writes the serialDebug value = 1 or OFF

	//  User Settings
	writeEEPROM(20, 1);		//  writes the tempType value = 1 or Fahrenheit
	writeEEPROM(21, 1);		//	writes the tempPrecision value = 0 or No Decimal
	writeEEPROM(22, 10);	//	writes the tempReadDelay value = 10 or 10 Seconds
	writeEEPROM(23, 1);		//	writes the timeFormat value =  1 or 12 hour
	writeEEPROM(24, 100);	//	writes the backlightLevel value = 100 or half
	writeEEPROM(25, 30);	//  writes the backlightTimeout to be 30 seconds
	writeEEPROM(26, 0);		//	writes the flow sensor 100% value to 0
	writeEEPROM(27, 75);	//  writes the flow sensor user level to 75 or 75%

	//  Alarm Settings
	writeEEPROM(100, 0);	//  writes alarms enable flag to off
	writeEEPROM(101, 0);	//  writes the alarm state flag to 0 or Off

	for (int i = 0; i < 8; i++)		//	loop through all 8 alarms
	{
		writeEEPROM(102 + (i * 6), 0);	//  writes the alarm type to 0, Day Lights
		writeEEPROM(103 + (i * 6), 0);	//	writes the relay trigger to relay 1
		writeEEPROM(104 + (i * 6), 12);	//  writes the alarm on hour 12
		writeEEPROM(105 + (i * 6), 11);	//  writes the alarm on minute 1
		writeEEPROM(106 + (i * 6), 15);	//  writes the alarm off hour 23
		writeEEPROM(107 + (i * 6), 1);	//  writes the alarm off minute 11
	}
	Serial.println("Factory Defaults Restored");
}

void eraseEEPROM()
{
	Serial.println("Erasing EE2");
	int address = 0;
	data = 0;
	while (address < 256)
	{
		writeEEPROM(address, 0);
		address++;
	}
	Serial.println("Reading EE2");
	address = 0;
	while (address < 256)
	{
		readEEPROM(address);
		address++;
	}
}