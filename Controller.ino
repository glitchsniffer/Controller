
//#include <SPI.h>
//#include <SD.h>
#include <TimeLib.h>
#include <Time.h>               //Time Library
#include <TimeAlarms.h>			//TimeAlarms Library
//#include <DS1307RTC.h>          //DS RTC library
#include <OneWire.h>            //OneWire Library for the DS Sensors
#include <DallasTemperature.h>  //Dallas Temperature library
#include <Wire.h>               //I2C Wire library
#include <LiquidCrystal_I2C.h>  //LCD I2C library
//#include <Alarms.h>

#define LOOP_INTERVAL 1000				//	millis between log readings

//  INITIALIZE THE EEPROM & RTC
//  ***********************************************

#define DS1307RTC 0x68			//	Set the address of the DS1307 RTC
#define EEPROM_DEV_ADDR 0x50	//  Set the address of the EEPROM

uint16_t eePointer = 0;			//  Sets the pointer location to 0 initially
byte data = 0;					//  Sets the value of data to be written to 0 initially


//  CONFIGURATION SETUP
//  ***********************************************
byte e2Empty;				//	initializes the byte e2Emtpy
byte configID;				//  initializes the byte configID
byte serialDebug;			//	initializes the byte serialDebug
byte timeFormat;			//	initializes the byte timeFormat
byte RTC_Status = 1;		//	initializes teh byte for the RTC status
byte backlightLevel;		//	initializes the byte backlightLevel
byte backlightTimeout;		//  initializes the byte backlighttimeout
byte secondsDisplay;		//	initializes the byte secondsDisplay
byte version = 0;			//  Sets the version number for the current program
byte build = 35;			//  Sets the build number for the current program
byte subbuild = 2;			//	Sets the sub build number between major version releases
byte today = 0;				//  Sets the today to the current date to display on the RTC

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
const int16_t backlight = 8;		//	variable used to store the pin for the backlight

//  SETUP CUSTOM CHARACTERS
byte degree[8] = {B01100,B10010,B10010,B01100,B00000,B00000,B00000,};	//  set the lcd char for the degree symbol
byte rarrow[8] = {B00000,B01000,B01100,B01110,B01100,B01000,B00000,};	//  set the lcd char for the right arrow symbol
byte uarrow[8] = {B00000,B00000,B00100,B01110,B11111,B00000,B00000,};	//  set the lcd char for the up arrow symbol
//byte larrow[8] = {B00000,B00010,B00110,B01110,B00110,B00010,B00000,};		//  set the lcd char for the left arrow symbol
//byte darrow[8] = {B00000,B00000,B11111,B01110,B00100,B00000,B00000,};		//  set the lcd char for the down arrow symbol
//byte bell[8] = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,};		//  set the lcd char for the timer bell symbol
//byte relon[8] = {B11100,B10100,B11100,B00000,B00111,B00101,B00101,};		//  set the lcd char for the relay on symbol


//  DEFINE BUTTON PINS
//  ***********************************************
#define upButton 49			//  set the up button to pin 53  Orange
#define leftButton 48		//  set the right button to pin 52  Yellow
#define rightButton 47		//  set the left button to pin 51  Green
#define downButton 46		//  set the down button to pin 50  Blue

volatile int8_t menuMode = 0;	//  set the variable that will be change to enable the menu in the interrupt

//const uint8_t button[]={42,43,44,45,46,47,48,49};	//  sets the pins for Button0 - button 7 respectively


//  INITIALIZE THE MENU VARIABLES
//  ***********************************************
byte mPoint = 0;		//  current main menu pointer position
byte mCur = 1;			//  current position of the main menu cursor
byte mStart = 0;		//  current starting cursor
int mLevel = 0;		//  current menu level.  main menu level=0, menu items=1, menu item selected=2
byte m1Sel = 0;			//  current selection for menu level 1
byte m2Sel = 0;			//	current selection for menu level 2
byte m3Sel = 0;			//  current selection for menu level 3
byte m0Start = 0;		//	starting cursor position for mLevel0
byte m1Start = 0;		//	starting cursor position for mLevel1
byte m2Start = 0;		//  starting cursor position for mLevel2
byte m3Start = 0;		//  starting cursor position for mLevel3
byte miMax = 0;			//  current selected menu item for purposes of up and down movement
byte mRet = 0;			//	variable to determine if the menu has just started.  if it has then it calls MenuLoop, otherwise it returns
int menuTimeout;	//	variable to count for a menu system timeout
int to = 0;		//	generic variable to determine if the menu system needs to time out

char* m0Items[] = { "", "User Setup", "Timers Setup", "Sensor Addr Setup","Sensor Setup","System Setup", ""};  //  setup menu items here  Min Cursor = 0 and Max Cursor = 4
char* m1Items0[] = { "", "Temp Type", "Temp Precision", "Temp Read Delay", "B Light Brightness", "Time Format", "Seconds Display", "Set Date/Time", "Flow Sensor On/Off", "XXXX", "" };  //  setup menu item 1 for System Config Min 0 Max 8
		char* m2Items00[] = { "", "Celsius", "Fahrenheit", ""};
		char* m2Items01[] = { "", "No Decimal", "1 Decimal", ""};
		char* m2Items02[] = { "", "Set Temp Read Delay", "" };
		char* m2Items03[] = { "", "Set BL Brightness", "" };
		char* m2Items04[] = { "", "24 Hour", "12 Hour", ""};
		char* m2Items05[] = { "", "Do Not Display Sec", "Display Seconds", "" };
		char* m2Items06[] = { "", "Exit", "Set Date and Time", ""};
		char* m2Items07[] = { "", "Disable", "Enable", "" };
		char* m2Items08[] = { "", "XXXX", "XXXX", "" };
	char* m1Items1[] = { "", "Set Timer 1", "Set Timer 2", "Set Timer 3", "Set Timer 4", "Set Timer 5", "Set Timer 6", "Set Timer 7", "Set Timer 8", "" };  //  setup menu item 2 for Timer Setup Min 0 Max 3
		char* m2Items10[] = { "", "Edit", "Exit", ""};
	char* m1Items2[] = { "", "Temp Sens 1 Add", "Temp Sens 2 Add", "Temp Sens 3 Add", "Temp Sens 4 Add", ""};  //  setup menu item 3 for Timer Setup Min 0 Max 3
		char* m2Items20[] = { "", "Address Sens ", "Exit", ""};
	char* m1Items3[] = { "", "Temp 1 Calib", "Temp 2 Calib", "Temp 3 Calib", "Temp 4 Calib", "Flow Calib", ""};  //  setup menu item 4 for Sensor Calibration Min 0 Max 4
		char* m2Items30[] = { "", "Calibrate Sensor ", "Exit", ""};
		char* m2Items34[] = { "", "Calibrate Flow Sens", "Set Min Flow", "Disable Flow Sensor", "Exit", "" };
	char* m1Items4[] = { "", "Serial Debugging", "Erase EEPROM", "Restore Defaults" };	//	setup menu item 4 for System Setup
		char* m2Items40[] = { "", "Disabled", "All", "Temp Sensors", "Menu", "Alarm", "EEPROM", "Relays", "System", "Flow Sensor", "" };
		char* m2Items41[] = { "", "Exit Erase EEPROM", "Erase EEPROM", "" };
		char* m2Items42[] = { "", "Exit", "Restore Defaults", "" };

	String strExiting = "Exiting";

//  INITIALIZE THE DS18B20 TEMPERATURE SENSORS
//  ***********************************************

// define the DS18B20 global variables
const uint8_t ONE_WIRE_BUS[]={42};		// the array to define which pins you will use for the busses ie {2,3,4,5};

#define TEMPERATURE_PRECISION 10
#define NUMBER_OF_BUS 1				// how many busses will you use for the sensors

OneWire *oneWire[NUMBER_OF_BUS];	//  Setup oneWire instances to communicate with any OneWire Devices

DallasTemperature *sensors[NUMBER_OF_BUS];  // Pass onewire reference to Dallas temperatures.

DeviceAddress tempDeviceAddress[8];		//  arrays to hold device addresses

uint16_t numberOfDevices[NUMBER_OF_BUS];		//  define the variable to store the number of busses
byte tempType;				//  initializes the byte tempType
byte tempPrecision;			//	initializes the byte tempPrecision
byte tempReadDelay;			//	initializes the byte tempReadDelay

float tempReadC[4];		//	array to hold the temperature readings taken
float tempReadF[4];		//	array to hold the temperature readings taken

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
byte RelayState;			//	byte for storing the state of all 8 relays

AlarmID_t tempReadID;		//  delay between reading the temperature sensors
AlarmID_t flowReadID;		//	delay between reading the flow sensor
AlarmID_t AlarmIDOn[8];		//  alarm IDs for each alarm's On event
AlarmID_t AlarmIDOff[8];	//	alarm IDs for each alarm's Off event

//  INITIALIZE THE FLOW SENSOR
//  ***********************************************

byte flowSensorInterrupt = 5;		//	interrupt to trigger for the flow sensor
byte flowSensorIntPin = 18;			//	pin on the arduino to use for the interrupt
byte flowSensorEnable;				//	initializes the byte flowSensorEnable
byte flowReadDelay = 30;				//	initializes the byte flowReadDelay (currently in seconds)

volatile uint16_t flowPulseCount = 0;	//	number of pulses trigger in the interrupt
		
uint16_t flowRateMax;					//	maximum flow rate as calibrated by the user
uint16_t flowRateMin;					//  minimum flow rate before an alarm is triggered, this is a percentage of flowRateMax

uint16_t	flowPulseReading[5];		//	the array to store the last 5 flow pulse readings in
byte flowPulseIndex = 0;				//	the number that selects the number of the flowPulseReadings array to write to
uint16_t flowPulseTotal;				//	the variable used to store the running total of the last 5 flowPulseCount samples

//  INITIALIZE THE RELAYS
//  ***********************************************

byte relayPins[] = { 22, 23, 24, 25, 26, 27, 28, 29 };		//  Initialize the relay pins
byte relayCount = 7;		//  Set the number of relays

//  INITIALIZE THE SD CARD
//  ***********************************************
//#define	SDCARD_WRITE_INTERVAL 60000		//	millis between SD Card writes
//uint32_t syncTime = 0;					//	time of the last sync
//
//const uint8_t chipSelect = 53;			//	pin for the chip select line on the SD Card
uint8_t SDexist = 0;					//	variable to determine if there is a problem with the sd card.  If there is then dont use the SD card.
//
//File logfile;							//	initialize the file to log to

//  INITIALIZE THE LEDS
//  ***********************************************
#define redledpin 44
#define greenledpin 45
#define blueledpin 43


void setup()
{
	//  SETUP THE SERIAL PORT
	Wire.begin();
	delay(250);
	Serial.begin(115200);				//  start the serial port if debugging is on
	Serial.println();
	serialDebug = readEEPROM(5);		//	reads out user setting to turn serial debugging on for each type of debugging

	//  SHOW THE COMPILE DATE AND TIME	
	//Serial.println("Compiled: " __DATE__ ", " __TIME__ ", " __AVR_LIBC_VERSION_STRING__);

	//  SETUP THE RTC
	//setSyncProvider(RTC.get);		//  this function get the time from the RTC
	//if (timeStatus() != timeSet)		//  checks to see if it can read the RTC
	//{
	//	RTC_Status = 0;
	//	Serial.println("Unable to get the RTC");
	//	Serial.println();
	//}
	//else{ Serial.println("RTC system time"); }

	//  READ THE VARIABLES OUT OF THE EEPROM

	//  READ NON USER SETTINGS FROM EEPROM

	e2Empty = readEEPROM(0);			//	reads the e2Empty out to determine if it needs to set defaults
	if (e2Empty == 0){ factoryDefaultset(); }
	configID = readEEPROM(0);			//  reads the configID out

	//  READ USER SETTINGS FROM EEPROM
	tempType = readEEPROM(20);			//  reads out user setting to selects between 0 = Celsius or 1 = Fahrenheit
	tempPrecision = readEEPROM(21);		//	reads out user setting to selece the decimal precision of the temp sensors 0 = No Decimal or 1 = 1 Decimal
	tempReadDelay = readEEPROM(22);		//	reads out user setting for the amount of time in seconds between reading the temp sensors
	timeFormat = readEEPROM(23);		//	reads out user setting for the time format 0 = 24 hour and 1 = 12 hour
	secondsDisplay = readEEPROM(24);	//	reads out user setting to display seconds 0 = No and 1 = Yes
	backlightLevel = readEEPROM(25);	//	reads out the user setting to control the backlight level
	backlightTimeout = readEEPROM(26);	//	reads out the user setting to control when the backlight level times out
	flowSensorEnable = readEEPROM(27);	//	reads out the user setting to determine if the flow sensor is enabled
	flowRateMax = (readEEPROM(28));		//	reads out the user setting that is set for the max flow rate as an integer
	flowRateMin = readEEPROM(29);		//	reads out the user setting for the minimum flow rate

	//  READ ALARM SETTINGS FROM EEPROM AND SETUP THE ALARMS IN THE TIMEALARMS LIBRARY

	tempReadID = Alarm.timerRepeat(tempReadDelay, DS18B20_Read);		//	sets an alarm to read the temp sensors at the specified delay and returns the Alarm_ID to tempReadID
	AlarmEnable = readEEPROM(100);		//	reads out the byte for the enable flags for all 8 alarms
	AlarmState = readEEPROM(101);		//	reads out the byte for the state flags for all 8 alarms
	RelayState = readEEPROM(150);

	if ((serialDebug & 4) == 4)
	{
		Serial.println();
		Serial.println("ALARM EEPROM SETTINGS");
		Serial.println("ID ON OFF  En  Typ  ON     OFF   Relay");
	}

	for (uint8_t id = 0; id <= relayCount; id++)		//  read each of the alarms values out of the EEPROM
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
			Serial.print("  ");
			Serial.print(AlarmIDOn[id]);
			if (AlarmIDOn[id] >= 10){ Serial.print("  "); }
			else { Serial.print("   "); }
			Serial.print(AlarmIDOff[id]);
			if (AlarmIDOff[id] >= 10){ Serial.print(" "); }
			else { Serial.print("  "); }
			if ((AlarmEnable & (1 << id)) == (1 << id)){ Serial.print(" ON   "); }
			else{ Serial.print(" OFF  "); }
			Serial.print(AlarmType[id]);
			Serial.print("  ");

			if (AlarmHourOn[id] < 10){ Serial.print(" "); }
			Serial.print(AlarmHourOn[id]);
			Serial.print(":");
			if (AlarmMinOn[id] < 10){
				Serial.print("0");
				Serial.print(AlarmMinOn[id]);
			}
			else{ Serial.print(AlarmMinOn[id]); }
			Serial.print("  ");
			if (AlarmHourOff[id] < 10){ Serial.print(" "); }
			Serial.print(AlarmHourOff[id]);
			Serial.print(":");
			if (AlarmMinOff[id] < 10){
				Serial.print("0");
				Serial.print(AlarmMinOff[id]);
			}
			else{ Serial.print(AlarmMinOff[id]); }
			Serial.print("  ");
			Serial.println(AlarmRelay[id], BIN);
		}
	}

	flowReadID = Alarm.timerRepeat(flowReadDelay, FlowSensorRead);		//	sets an alarm to read the flow sensor at the specified dalay and returns the Alarm_IS to flowReadID
	if (flowSensorEnable == 0)
	{
		Alarm.disable(flowReadID);
		Serial.println("Flow Readings Disabled");
	}
	else
	{
		Alarm.enable(flowReadID);
		Serial.println("Flow Readings Enabled");
	}

	serialDebug = readEEPROM(5);		//	read out the serial debug again in case it was disable during the alarm print

	if ((serialDebug & 4) == 4)
	{
		uint8_t rd;
		Serial.println();
		Serial.print("AlarmEnable ");
		Serial.println(AlarmEnable, BIN);
		Serial.print("AlarmState ");
		Serial.println(AlarmState, BIN);
		Serial.print("RelayState ");
		Serial.println(RelayState, BIN);
		Serial.print("# of Alarms ");
		rd = Alarm.count();
		Serial.println(rd);
		Serial.print("tempReadID ");
		Serial.print(tempReadID);
		Serial.print(",");
		rd = Alarm.read(tempReadID);
		Serial.println(rd);
		Serial.print("flowReadID ");
		Serial.print(flowReadID);
		Serial.print(",");
		rd = Alarm.read(flowReadID);
		Serial.println(rd);

		Serial.println();
	}
////////////////////
	//  SETUP THE BUTTONS
	pinMode(upButton, INPUT);		//  sets the UpButton to an input
	pinMode(downButton, INPUT);		//  sets the DownButton to an input
	pinMode(leftButton, INPUT);		//  sets the LeftButton to an input
	pinMode(rightButton, INPUT);	//  sets the RightButton to an input

	//for(uint8_t b = 0; b < 8; b++){pinMode(button[b], INPUT);}	//  sets Button0-7 pins as inputs

	attachInterrupt(4, MenuButtonPress, RISING);		//  Attaches int.4, pin 19(RX1) and sets it to trigger on a low input from the menu button

	//  SETUP THE FLOW SENSOR

	pinMode(flowSensorIntPin, INPUT);				//	set the flow sensor as an input to the pin number in the variable
	digitalWrite(flowSensorIntPin, HIGH);			//	write the pin high to be active low

	//  SETUP THE RELAYS OUTPUTS
	for (uint8_t relay = 0; relay <= relayCount; relay++){ pinMode(relayPins[relay], OUTPUT); }
	for (uint8_t relay = 0; relay <= relayCount; relay++){ digitalWrite(relayPins[relay], HIGH); }

	//  SETUP THE LCD SCREEN
	lcd.begin(20, 4);						//  setup the LCD's number of columns and rows
	lcd.createChar(1, degree);				//  init custom characters as numbers
	lcd.createChar(2, rarrow);				//  init custom characters as numbers
	lcd.createChar(3, uarrow);				//  init custom characters as numbers
	//lcd.createChar(4, larrow);				//  init custom characters as numbers
	//lcd.createChar(5, darrow);				//  init custom characters as numbers
	//lcd.createChar(6, bell);				//  init custom characters as numbers
	//lcd.createChar(7, relon);				//  init custom characters as numbers
	lcd.setBacklightPin(B_Light, POSITIVE);  //  set the backlight pin and polarity
	lcd.setBacklight(HIGH);					//  toggle the backlight on
	pinMode(backlight, OUTPUT);				//	set the pin for the backlight as an output
	analogWrite(backlight, backlightLevel);	//	write the backlightlevel to the pin for the backlight

	START_SCREEN();		//  call the start up screen function

	RelayStatusDisplay(0, 3);				//	call the relay status display function
	RelayToggle(RelayState, 1);				//	Turn on the relays according to the AlarmState byte

	//Serial.println("Checking EEPROM");
	//eraseEEPROM();
	//int wait = 0;
	//while (!wait);

	/*lcd.setCursor(0,2);
	lcd.write(byte(2));
	lcd.write(byte(3));
	lcd.write(byte(4));
	lcd.write(byte(5));
	lcd.write(byte(6));
	lcd.write(byte(7));*/

	//  SETUP THE DS18B20 SENSORS
	//  Check to see how many sensors are on the busses
	for (uint16_t i = 0; i < NUMBER_OF_BUS; i++)   //search each bus one by one
	{
		oneWire[i] = new OneWire(ONE_WIRE_BUS[i]);
		sensors[i] = new DallasTemperature(oneWire[i]);
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();

		if ((serialDebug & 1) == 1)
		{
			Serial.println();
			Serial.print("Locating devices ");
			Serial.print("Found ");
			Serial.print(numberOfDevices[i], DEC);
			Serial.print(" devices on port ");
			Serial.println(ONE_WIRE_BUS[i], DEC);
			Serial.println();
		}
		for (uint16_t j = 0; j < numberOfDevices[i]; j++)
		{
			// Search the wire for address
			if (sensors[i]->getAddress(tempDeviceAddress[j], j))
			{

				if ((serialDebug & 1) == 1)
				{
					Serial.print("Found device ");
					Serial.print(j, DEC);
					Serial.print(" with address ");
					printAddress(tempDeviceAddress[j]);
					Serial.println();
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
					Serial.println();
				}
			}
		}
		delay(200);
	}

	//	INITIALIZE THE LEDS
	pinMode(redledpin, OUTPUT);
	pinMode(greenledpin, OUTPUT);
	pinMode(blueledpin, OUTPUT);

	digitalWrite(redledpin, LOW);
	digitalWrite(greenledpin, LOW);
	digitalWrite(blueledpin, LOW);

	//	INITIALIZE THE SD CARD
	//Serial.println("Initializing the SD Card...");
	//pinMode(53, OUTPUT);		//	the chipselect line of the SD Card. always configure it to an output

	//if (!SD.begin(chipSelect))
	//{
	//	Serial.println("Card Failed, or Card is not present");
	//	SDexist = 0;
	//	digitalWrite(redledpin, HIGH);
	//}
	//else
	//{
	//	Serial.println("SD card initialized.");
	//	Serial.println();
	//	SDexist = 1;
	//}
	//Serial.println(SDexist);
	////	CREATE A NEW FILE
	//if (SDexist == 1)
	//{
	//	char filename[] = "LOGGER00.CSV";
	//	for (uint8_t i = 0; i < 100; i++)
	//	{
	//		filename[6] = i / 10 + '0';
	//		filename[7] = i % 10 + '0';
	//		if (!SD.exists(filename))
	//		{
	//			logfile = SD.open(filename, FILE_WRITE);	//	only open a new file if it doesn't exist
	//			break;
	//		}
	//	}
	//	if (!logfile){ error("Could not create a file"); }		//	if there is an error call the error function with the error

	//	//	print the filename that was created in the code above
	//	Serial.print("Logging to: ");
	//	Serial.println(filename);
	//	Serial.println();

	//	//	CREATE A HEADER FOR THE LOGFILE
	//	logfile.println("millis, stamp, time, ,temptype, temp1, temp2, temp3, temp4, relaystate");
	//}
		//	TAKE A TEMP READING AND START THE LOOP
		if ((serialDebug & 1) == 1){ Serial.println(); }
		DS18B20_Read();
		Serial.print("Starting Loop :");
		Serial.print(millis());
		Serial.println();
		//RelayToggleALL();		//**********NICE SPOT TO TEST RELAYS**************
//////////
}

void loop()
{
	//	Serial.println(freeRam());		//	Used to check the ammount of free RAM on the micro
	if (menuMode == 1)
	{
		Serial.println("Entering Menu");		//  calls the MenuTitle as long as menuMode = 1
		MenuTitle();
		Serial.println("Exiting Menu");
	}

	if (RTC_Status == 1){ LCDDateDisplay(0, 0, 1); }						//  only calls LCDDateDisplay if the RTC has been set

	//	adjusts for 12 or 24 hour and if the seconds are to be displayed on the LCD screen
	switch (secondsDisplay)
	{
	case 0:
		if (timeFormat == 0){ LCDTimeDisplay(0, 2, 0, hour(), minute(), second(), 0); }
		else { LCDTimeDisplay(0, 1, 0, hour(), minute(), second(), 0); }
		break;
	case 1:
		if (timeFormat == 0){ LCDTimeDisplay(0, 1, 0, hour(), minute(), second(), 0); }
		else { LCDTimeDisplay(0, 0, 0, hour(), minute(), second(), 0); }
		break;
	}

	//	delay for the ammount of time selected between readings
	Alarm.delay(((LOOP_INTERVAL - 1) - (millis() % LOOP_INTERVAL)));	//  uses the Alarm.delay to use the timer
}

void AlarmON()
{
	uint8_t id;
	id = Alarm.getTriggeredAlarmId();
	id = ((id - 1) / 2);
	Serial.print("Alarm ");
	Serial.print(id);
	Serial.print(" ON @ ");
	Serial.print(hour());
	Serial.print(":");
	Serial.println(minute());

	RelayToggle(AlarmRelay[id], 1);

	if ((serialDebug & 4) == 4)
	{
		uint8_t trigger;
		trigger = Alarm.getNextTrigger();
		Serial.print("Next ");
		Serial.println(trigger);
		Serial.print("Alarm State ");
		Serial.println(AlarmState, BIN);
		Serial.println();
	}
}
void AlarmOFF()
{
	uint8_t id;
	id = Alarm.getTriggeredAlarmId();
	id = ((id - 2) / 2);
	Serial.print("Alarm ");
	Serial.print(id);
	Serial.print(" OFF @ ");
	Serial.print(hour());
	Serial.print(":");
	Serial.println(minute());
	RelayToggle(AlarmRelay[id], 0);

	if ((serialDebug & 4) == 4)
	{
		uint8_t trigger;
		trigger = Alarm.getNextTrigger();
		Serial.print("Next ");
		Serial.println(trigger);
		Serial.print("Alarm State ");
		Serial.println(AlarmState, BIN);
		Serial.println();
	}
}
void RelayToggleALL()
{
	for (uint8_t relay = 0; relay <= relayCount; relay++)
	{
		digitalWrite(relayPins[relay], LOW);
		byte bit = 1 << relay;						//	sets bit for the correct id's bit position
		RelayStatusDisplay(0, 3);
		delay(500);
	}
	for (uint8_t relay = 0; relay <= relayCount; relay++)
	{		
		digitalWrite(relayPins[relay], HIGH);
		byte bit = 1 << relay;						//	sets bit for the correct id's bit position
		RelayStatusDisplay(0, 3);
		delay(500);
	}
}
void RelayToggle(uint8_t state, uint8_t onoff)
{
	Serial.print("Before state ");
	Serial.print(state, BIN);
	Serial.print(" RelayState ");
	Serial.println(RelayState, BIN);

	for (uint8_t i = 0; i <= relayCount; i++)
	{
		byte rl;
		rl = (state & (1 << i));		//	isolate a single relay to see if its bit is set to 1 or 0
		lcd.setCursor(i, 3);			//	set the cursor position to the current relay to print a + or -
		
		//	switch to turn on or off the relay
		switch (onoff)
		{
		case 0:			//  turns off the relay
			if (rl > 0)
			{
				if ((RelayState & (1 << i)) != 0)	//	Only changes if there is not a 0 for that bit.  this prevents changing the relaystate if there it is alread on
				{
					RelayState = RelayState ^ (1 << i);		//	toggles the bit held in RelayState
				}
				digitalWrite(relayPins[i], HIGH);		//	turns the relay off
				lcd.print("-");							//	sets the relay to display a - on the LCD screen
			}
			break;
		case 1:
			if (rl > 0)
			{
				if ((RelayState & (1 << i)) != (1 << i))	//	Only changes if there is not a 1 for that bit.  this prevents changing the relaystate if there it is alread on
				{
					RelayState = RelayState ^ (1 << i);		//	toggles the bit held in RelayState
				}
				digitalWrite(relayPins[i], LOW);		//	turns the relay on
				lcd.print("+");							//	sets the relay to display a - on the LCD screen
			}
			break;
		}
		delay(200);
	}
	Serial.print("After state ");
	Serial.print(state, BIN);
	Serial.print(" RelayState ");
	Serial.println(RelayState, BIN);

	writeEEPROM(150, RelayState);
	RelayState = readEEPROM(150);
}

void RelayStatusDisplay(uint8_t col, uint8_t row)
{
	lcd.setCursor(col, row);
	for (uint8_t i = 0; i <= relayCount; i++)
		{
			lcd.setCursor((col + i), row);
			if ((RelayState & (1 << i)) == (1 << i)){ lcd.print("+"); }
			else lcd.print("-");
		}
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
	lcd.print(".");
	lcd.print(subbuild);
	delay(1000);
	lcd.setCursor(0,0);
	lcd.clear();
}
	
void LCDTimeDisplay(byte disp, uint8_t col, uint8_t row, uint8_t hour, uint8_t min, uint8_t sec, uint8_t space)
//	disp is used for options
//		0 = No options
//		1 = force seconds display off.
//		2 = force seconds display on.
//	space can be used to add space between the time and AM/PM, or to add space elsewhere if needed.
//	if sec == 99 then dont print seconds, if sec == 98 print seconds and AM/PM, if sec != 99 then print the seconds only
{
	//	display the hour
	uint8_t realhour = hour;
	switch (timeFormat)		//	use timeFormat to determine where to put the cursor for the hour if set for 12 hour time and print the hour and AM/PM
	{
	case 0:		//	if 24 hour set the cursor to use 2 digits
		if (hour <= 9){ lcd.setCursor(col + 1, row); }	//  set cursor for single digits
		else { lcd.setCursor(col, row); }				//	set cursor for double digits
		lcd.print(hour);
		break;
	case 1:		//	if 12 hour set the cursor to account for # of hour digits
		//  convert hour to 12 hour time
		if (hour == 0){ hour = 12; }					//	if hour is midnight, add 12 to the display of the hour to make it 12 AM
		else if (hour >= 13) { hour = hour - 12; }		//	if hour is 1pm or greater convert to 12 hour

		//	set the cursor to print the hour digits
		lcd.setCursor(col, row);
		if (hour <= 9){ lcd.print(" ");	}		//	add a leading space for the hour if <= 9
		
		//	print the hour
		lcd.print(hour);
		break;
	}

	//	display the minutes
	col = col + 2;
	lcd.setCursor(col, row);		//	set cursor for the colon
	lcd.print(":");						//	print the colon for the minutes

	if (min <= 9){ lcd.print("0"); }	//	if the minutes is 1 digit pad a 0 to the single digit
	lcd.print(min);

	col = col + 3;
	lcd.setCursor(col, row);		//	set the cursor for the either the seconds or AMPM printing

	//	display the seconds
	if ((secondsDisplay == 1) || (disp == 2))		//  only display seconds if disp forces it or they are set to display
	{
		if ((disp & 1) != 1)		//	if the disp byte is not set to force seconds digits off, print the seconds
		{
			lcd.print(":");
			if (second() <= 9)				//	print the 1 digit second padded with a 0
			{
				lcd.print("0");
				lcd.print(second());
			}
			else { lcd.print(second()); }	//	print the 2 digit second
			col = col + 3;
		}
	}
	//  display AM/PM
	if (timeFormat == 1)
	{
		//	if ((disp & 1) != 1){ lcd.setCursor(col + 3, row); }
		col = col + space;
		lcd.setCursor(col, row);
		if (realhour >= 12){ lcd.print("PM"); }
		else if (realhour <= 11){ lcd.print("AM"); }
	}
}

void LCDDateDisplay(byte display, uint8_t col, uint8_t row)
//	display - 0 to not change the date if it is the same day.
//	display - 1 to enable 0 padding on the day

{
	if(display == 0 && day() == today){ return; }		//	if the day hasn't changed, dont refresh it	
	else
	{
		uint8_t addcol = 0;
		uint8_t tempcol = 0;

		lcd.setCursor(col, row);
		lcd.print("           ");					//	erase the current line

		//	display and shift the cursor according to the month
		if (month() < 10){ addcol = 1; }
		lcd.setCursor(col + addcol, row);
		lcd.print(month());
		lcd.print("/");
		col = col + 3;
		addcol = 0;

		//	display and shift the cursor according to the day
		if (display == 1 && day() < 10)
		{
			lcd.print("0");
			addcol = 1;
		}
		lcd.setCursor(col + addcol , row);
		lcd.print(day());
		lcd.print("/");
		col = col + 3;
		addcol = 0;

		//	display the year
		lcd.print(year());
		today = day();					//	set the day = to today so that it doesn't refresh the display with it until tomorrow
	}
}

void DS18B20_Read()
{
	uint8_t c;
	//  Read the DS sensors found in void setup
	for (uint8_t i = 0; i < NUMBER_OF_BUS; i++)   // poll every bus
	{
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();
		sensors[i]->requestTemperatures();
		// print the device information
		for (uint8_t j = 0; j < numberOfDevices[i]; j++)    // poll devices connect to the bus
		{
			sensors[i]->getAddress(tempDeviceAddress[j], j);
			if ((serialDebug & 1) == 1)
			{
				printAddress(tempDeviceAddress[j]);      //print ID
				Serial.print(";");
			}

			float tempC = sensors[i]->getTempC(tempDeviceAddress[j]);
			float tempF = sensors[i]->getTempF(tempDeviceAddress[j]);

			tempReadC[j] = tempC;	//	store the current reading to be logged in Celcius
			tempReadF[j] = tempF;	//	store the current reading to be logged in Fahrenheit


			if ((serialDebug & 1) == 1)
			{
				Serial.print(j);
				Serial.print(" ");
			}
			switch (tempPrecision)
			{
			case 0:
				lcd.setCursor(13, j);	//  Set the lcd cursor depending on what sensor your are reading
				lcd.print("S");
				lcd.print(j + 1);		//  print the sensor number
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
	//if(SDexist == 1){ logger(); }
}

//void logger()
//{
//	time_t t;
//
//	digitalWrite(greenledpin, HIGH);
//
//	if ((serialDebug & 1) == 1)
//	{Serial.println("Preparing Data");}
//
//	// log the millis since starting
//	uint32_t m = millis();
//	logfile.print(m);
//	logfile.print(", ");
//
//	if ((serialDebug & 1) == 1)
//	{
//		Serial.print(m);
//		Serial.print(", ");
//	}
//
//	t = now();		//	fetch the current time
//
//	//	print the log time
//	logfile.print(t);
//	logfile.print(", ");
//	logfile.print(year(t), DEC);
//	logfile.print("/");
//	logfile.print(month(t), DEC);
//	logfile.print("/");
//	logfile.print(day(t), DEC);
//	logfile.print(" ");
//	logfile.print(hour(t), DEC);
//	logfile.print(":");
//	logfile.print(minute(t), DEC);
//	logfile.print(":");
//	logfile.print(second(t), DEC);
//	//	echo to the serial port
//	if ((serialDebug & 1) == 1)
//	{
//		Serial.print(t);
//		Serial.print(", ");
//		Serial.print(year(t), DEC);
//		Serial.print("/");
//		Serial.print(month(t), DEC);
//		Serial.print("/");
//		Serial.print(day(t), DEC);
//		Serial.print(" ");
//		Serial.print(hour(t), DEC);
//		Serial.print(":");
//		Serial.print(minute(t), DEC);
//		Serial.print(":");
//		Serial.print(second(t), DEC);
//	}
//	//	log the data
//	switch (tempType)
//	{
//	case 0:
//		logfile.print(", ");
//		logfile.print("C");
//		logfile.print(", ");
//		logfile.print(tempReadC[0], 2);
//		logfile.print(", ");
//		logfile.print(tempReadC[1], 2);
//		logfile.print(", ");
//		logfile.print(tempReadC[2], 2);
//		logfile.print(", ");
//		logfile.print(tempReadC[3], 2);
//		logfile.print(", ");
//		logfile.println(RelayState, BIN);
//		if ((serialDebug & 1) == 1)
//		{
//			Serial.print(", ");
//			Serial.print("C");
//			Serial.print(", ");
//			Serial.print(tempReadC[0], 2);
//			Serial.print(", ");
//			Serial.print(tempReadC[1], 2);
//			Serial.print(", ");
//			Serial.print(tempReadC[2], 2);
//			Serial.print(", ");
//			Serial.print(tempReadC[3], 2);
//			Serial.print(",");
//			Serial.println(RelayState, BIN);
//		}
//		break;
//	case 1:
//		logfile.print(", ");
//		logfile.print("F");
//		logfile.print(", ");
//		logfile.print(tempReadF[0], 2);
//		logfile.print(", ");
//		logfile.print(tempReadF[1], 2);
//		logfile.print(", ");
//		logfile.print(tempReadF[2], 2);
//		logfile.print(", ");
//		logfile.print(tempReadF[3], 2);
//		logfile.print(", ");
//		logfile.println(RelayState, BIN);
//		if ((serialDebug & 1) == 1)
//		{
//			Serial.print(", ");
//			Serial.print("F");
//			Serial.print(", ");
//			Serial.print(tempReadF[0], 2);
//			Serial.print(", ");
//			Serial.print(tempReadF[1], 2);
//			Serial.print(", ");
//			Serial.print(tempReadF[2], 2);
//			Serial.print(", ");
//			Serial.print(tempReadF[3], 2);
//			Serial.print(",");
//			Serial.println(RelayState, BIN);
//		}
//		break;
//	}
//
//	digitalWrite(greenledpin, LOW);
//
//	//  Write the data to disk if the millis are more than the write interval
//	if ((millis() - syncTime) < SDCARD_WRITE_INTERVAL) { return; }
//	syncTime = millis();
//
//	//	write the logfiles data to the SD Card
//	if ((serialDebug & 1) == 1)
//	{
//		Serial.println("Writing log to the SD Card");
//		Serial.println("");
//	}
//	digitalWrite(blueledpin, HIGH);
//	logfile.flush();
//	digitalWrite(blueledpin, LOW);
//}

void FlowSensorRead()
{
	float flowRate = 0;					//	calculated flow rate of the sensor
	byte flowReadTime = 1;				//	time to read the flow sensor in ms
	float flowPulseFactor = 2.25;		//	number to multiply the number of pulses by to get the flow rate
	uint32_t flowStartTime;				//	time in millis that the flow rate sample started
	uint32_t flowEndTime;				//	time in millis that the flow rate sample ended

	//	convert the flowReadTime stored in the EEPROM to milliseconds
	uint16_t rdtime = ((flowReadTime * 1000) - 1);

	//	turn on the flow sensor, take the flow reading, and turn off the flow sensor
	Serial.print("Flow ON @ ");
	Serial.println(now());
	flowStartTime = millis();											//	set the current millis to the start time
	attachInterrupt(flowSensorInterrupt, FlowSensorCounter, FALLING);	//	enable the interrupt to start the reading
	Alarm.delay(rdtime);												//	delay for the ammount of time in millis
	detachInterrupt(flowSensorInterrupt);								//	disable the interrupt for the flow sensor
	flowEndTime = millis();												//	set the current millis to the end time
	Serial.print("Flow OFF @ ");
	Serial.println(now());
	Serial.println();

	//	store and calculate the average of the last 5 variables
	flowPulseTotal = flowPulseTotal - flowPulseReading[flowPulseIndex];	//	Subtract the array position to be added to below
	flowPulseReading[flowPulseIndex] = flowPulseCount;					//	set the current reading to the array variable
	flowPulseTotal += flowPulseReading[flowPulseIndex];					//  add the current reading to the total
	flowPulseIndex++;													//	increase the index by 1
	if (flowPulseIndex > 4) { flowPulseIndex = 0; }						//	if index is at the 5th variable, reset to 0

	//	print the flow status to the LCD
	//	can use this area to determine an alarm alert to the user.
	if (flowRateMax > 0)
	{
		lcd.setCursor(0, 2);
		if (flowPulseTotal / 5 <= flowRateMax * (0.01 * flowRateMin)) { lcd.print("Flow Alarm"); }
		else lcd.print("Flow Good ");
	}
	//	in case the millis counter overflows to 0 after approx 50 days, recalculate the time for the next flow reading
	if (flowEndTime < flowStartTime)
	{
		flowEndTime = (4294967295 - flowStartTime) + 1 + flowEndTime;	//	set flowEndTime to the total duration
		flowStartTime = 0;												//	set the start time to 0 so that the endtime is the total duration
	}

	//	calculate the flow rate in mL based on the flowPulseCounter
	flowRate = (flowPulseFactor * flowPulseCount) / ((flowEndTime - flowStartTime) / 1000);		//	in mL

	//	print the stats to the Serial port
	if ((serialDebug & 64) == 64)
	{
		Serial.print("Duration ");
		Serial.print(flowEndTime);
		Serial.print("-");
		Serial.print(flowStartTime);
		Serial.print("=");
		Serial.println(flowEndTime - flowStartTime);
		Serial.print("PulseCount ");
		Serial.println(flowPulseCount);
		Serial.print("PulseTotal ");
		Serial.print(flowPulseTotal);
		Serial.print(", Avg ");
		Serial.println(flowPulseTotal / 5);
		Serial.print("Rate ");
		Serial.println(flowRate);

		//  print the flow rate in L/h and G/h

		flowRate = flowRate / 10;

		Serial.print("Rate ");
		Serial.print(flowRate * 60);
		Serial.print("L/h");

		flowRate = flowRate * 0.264172;		//	convert to gallons

		Serial.print(", ");
		Serial.print(flowRate * 60);
		Serial.println("G/h");
		Serial.println();
	}

	//	reset the pulse counter
	flowPulseCount = 0;
}

void FlowSensorCounter()
{
	flowPulseCount++;
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

void writeEEPROM(uint16_t address, byte data)
{
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.write(data);							//  writes the byte data to the EEPROM at the address specified above
	Wire.endTransmission();						//  stops the write communication on the I2C
	delay(10);
	if ((serialDebug & 8) == 8)
	{
		Serial.print("Write ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(data);
	}
}

byte readEEPROM(uint16_t address)
{
	byte result;									//  returned value
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((uint16_t)(address >> 8));		//  writes the first byte of the pointer address to the device
	Wire.write((uint16_t)(address & 0xFF));		//  writes the second byte of the pointer address to the device
	Wire.endTransmission(); 					//  stops the write communication on the I2C
	Wire.requestFrom(EEPROM_DEV_ADDR, 1);		//  gets 1 byte of data from the device
	result = Wire.read();							//  sets the value read to data
	if ((serialDebug & 8) == 8)
	{
		Serial.print("Read ");
		Serial.print(address);
		Serial.print(",");
		Serial.println(result);
	}
	return result;								//  returns data to the previous call
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
	writeEEPROM(5, 76);		//	writes the serialDebug value = 1 or OFF

	//  User Settings
	writeEEPROM(20, 1);		//  writes the tempType value = 1 or Fahrenheit
	writeEEPROM(21, 1);		//	writes the tempPrecision value = 0 or No Decimal
	writeEEPROM(22, 10);	//	writes the tempReadDelay value = 10 or 10 Seconds
	writeEEPROM(23, 1);		//	writes the timeFormat value =  1 or 12 hour
	writeEEPROM(24, 1);		//	writes the secondsDisplay value to 1 or Display seconds
	writeEEPROM(25, 100);	//	writes the backlightLevel value = 100 or half
	writeEEPROM(26, 30);	//  writes the backlightTimeout to be 30 seconds
	writeEEPROM(27, 0);		//	writes the flowSensorEnable to be disabled
	writeEEPROM(28, 55);	//	writes the 1st bit of the flow sensor 100% value to 0
	writeEEPROM(29, 75);	//  writes the flow sensor user level to 75 or 75% 
	writeEEPROM(30, 0);
	writeEEPROM(32, 0);

	//  Alarm Settings
	writeEEPROM(100, 255);	//  writes alarms enable flag to off
	writeEEPROM(101, 0);	//  writes the alarm state flag to 0 or Off
	writeEEPROM(150, 0);	//	writes the relayState flag to all 0's or OFF

	for (int i = 0; i <= relayCount; i++)	//	loop through all 8 alarms
	{
		writeEEPROM(102 + (i * 6), 0);			//  writes the alarm type to 0, Day Lights
		int bit = 1 << i;
		writeEEPROM(103 + (i * 6), (0 ^ bit));	//	writes the relay trigger to relay to match the id
		writeEEPROM(104 + (i * 6), 0+i);		//  writes the alarm on hour 12
		writeEEPROM(105 + (i * 6), 29+i);		//  writes the alarm on minute 1
		writeEEPROM(106 + (i * 6), 0+i);		//  writes the alarm off hour 23
		writeEEPROM(107 + (i * 6), 31+i);		//  writes the alarm off minute 11
	}
	Serial.println("Factory Defaults Restored");
}

void eraseEEPROM()
{
	Serial.println("Erasing EE2");
	uint16_t address = 0;
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

uint16_t freeRam()
{
	extern int32_t __heap_start, *__brkval;
	int32_t v;
	return (int32_t)&v - (__brkval == 0 ? (int32_t)&__heap_start : (uint32_t)__brkval);
}

byte decToBcd(byte val)
{
	return ((val / 10 * 16) + (val % 10));
}

byte bcdToDec(byte val)
{
	return ((val / 16 * 10) + (val % 16));
}

void error(char*str)
{
	Serial.print("Error: ");
	Serial.println(str);
	digitalWrite(redledpin, HIGH);
	//while (1);
}