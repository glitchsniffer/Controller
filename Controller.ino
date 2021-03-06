#include <Adafruit_MCP23008.h>
#include <Stepper.h>
#include <UTFT.h>
#include <UTouch.h>
#include <SD.h>
#include <SPI.h>
#include <TimeLib.h>
#include <TimeAlarms.h>				//TimeAlarms Library
#include <OneWire.h>				//OneWire Library for the DS Sensors
#include <DallasTemperature.h>		//Dallas Temperature library
#include <Wire.h>					//I2C Wire library
#include <DS1307RTC.h>
#include "EEprom.h"
#include "MCPExpander.h"
#include "TouchMenu.h"
#include "Controller.h"
#include <RF24.h>


//	VERSIONING VARIABLES
//	***********************************************
byte version = 0;			//  Sets the version number for the current program
byte build = 41;			//  Sets the build number for the current program
byte subbuild = 2;			//	Sets the sub build number between major version releases

//	DEFINES
//	***********************************************
#define EEPROM_DEV_ADDR 0x50	//  Set the address of the EEPROM
#define DS1307RTC 0x68			//	Set the address of the DS1307 RTC
#define TFT_B_LIGHT_PIN 9		//	the pin used for the backlight of the lcd screen
#define MCP17A 0x20				//	Set the address of the MCP17A IO Expander
#define TEMP_SENSOR_PIN 8		//	define which pin for the Due
#define RF24_CE	15				//	RF24 CE Line
#define RF24_CSN 14				//	RF24 CSN Line

//  INITIALIZE THE EEPROM
//  ***********************************************
EEprom eeprom(EEPROM_DEV_ADDR);	//	start an instance of the EEprom class

//  CONFIGURATION SETUP
//  ***********************************************
byte e2Empty;				//	initializes the byte e2Emtpy to determine if factory defaults are ran at startup
byte configID;				//  initializes the byte configID
byte serialDebug;			//	initializes the byte serialDebug
byte timeFormat;			//	initializes the byte timeFormat
byte RTC_Status = 1;		//	initializes the byte for the RTC status
byte backlightLevel;		//	initializes the byte backlightLevel
byte backlightTimeout;		//  initializes the byte backlighttimeout
byte secondsDisplay;		//	initializes the byte secondsDisplay
uint8_t today = 0;			//  Sets the today to the current date to display on the RTC
String timestr = "";		//	initialize the string for assembling a time to be displayed and manipulated

//  INITIALIZE THE 4.3" TFT TOUCHSCREEN
//  ***********************************************
UTFT TFT(ITDB43, 25, 26, 27, 28);	//	start an instance of the UTFT class using the display model and the pins used
UTouch Touch(6, 5, 32, 3, 2);		//	start an instance of the UTouch class using the pins used
TouchMenu Menu;					    //	start an instance of the TouchMenu class using a dummy variable
uint16_t menuTimeout;				//	variable to count for a menu system timeout

//	set the fonts that we will be using for the 4.3" TOUCHSCREEN
extern uint8_t GroteskBold16x32[];
extern uint8_t GroteskBold24x48[];
extern unsigned short gear[0x400];
extern unsigned short arrow_up[0x400];
extern unsigned short arrow_down[0x400];
//extern unsigned short arrow_left[0x400];
//extern unsigned short arrow_right[0x400];


//  DEFINE THE MCP23017 IO EXPANDER
//  ***********************************************
MCPExpander mcpA(MCP17A);
MCPExpander mcpB(MCP17A);

byte menuEnterInterrupt = 4;		//	interrupt to trigger to enter the menu
//define menuEnterIntPin = 19		//	pin on the arduino to use for the interrupt
byte menuEnterIntPin = 19;			//	pin on the arduino to use for the interrupt
uint32_t debouncing_time = 250;		//	debouncing time in millis
volatile uint32_t last_micros;		//	placeholder variable to store when the interrrupt was triggered
volatile uint8_t menuMode = 0;		//  set the variable that will be change to enable the menu in the interrupt

//  TEMP NODE SETTINGS
//  ***********************************************
//byte nodeAddress = 0;				//	set the node address as the Master
unsigned long startTime = 0;			//	long to store the timeout time
unsigned long currentTime = 0;		//	long to store the current time

//  INITIALIZE THE NRF2401 RADIO SETTINGS
//  ***********************************************
RF24 radio(RF24_CE, RF24_CSN);						// CE, CSN for the RFN2401
const byte addresses[][6] = { "00001", "00002" };	//	array for the addresses of the pipes

//	COMMAND PACKET STRUCT **MAX 32 BYTES**
struct Command_Package {
	byte com_address;	//	intended address for the command
	byte command;		//	the command to be issued
};
Command_Package comm;	//	create a variable for the above structure

//  DATA PACKAGE STRUCT **MAX 32 BYTES**
struct Data_Package {
	byte origin_addr;	//	originating address
	byte temp_1I;		//	1 bytes
	byte temp_1D;		//	1 bytes
	byte temp_2I;		//	1 bytes
	byte temp_2D;		//	1 bytes
};
Data_Package data;		//	create a variable for the above structure

//  REMOTE TEMPS STRUCT
struct Remote_Sensors {
	byte temp_1_int;	//	Temp 1 integer portion
	byte temp_1_dec;	//	Temp 1 decimal portion
	byte temp_2_int;	//	Temp 2 integer portion
	byte temp_2_dec;	//	Temp 2 decimal portion
	byte temp_3_int;	//	Temp 3 integer portion
	byte temp_3_dec;	//	Temp 3 decimal portion
	byte temp_4_int;	//	Temp 4 integer portion
	byte temp_4_dec;	//	Temp 4 decimal portion
	byte temp_5_int;	//	Temp 5 integer portion
	byte temp_5_dec;	//	Temp 5 decimal portion
	byte temp_6_int;	//	Temp 6 integer portion
	byte temp_6_dec;	//	Temp 6 decimal portion
};
Remote_Sensors remoteTemp;

//  INITIALIZE THE DS18B20 TEMPERATURE SENSORS
//  ***********************************************
//	Set the pin that the temp sensors are connected to
#define TEMP_SENSOR_PRECISION 11		//	set the temperature precision to 11 bits.  this is needed to get the proper precision for 1 decimal
OneWire oneWire(TEMP_SENSOR_PIN);			//  Setup oneWire instances to communicate with any OneWire Devices
DallasTemperature tempSensors(&oneWire);	//	pass onewire reference to Dallas temperatures.
DeviceAddress tempSensorAddr[4];			//  arrays to hold device addresses for 4 sensors
byte numberOfSensors = 0;	//	define the byte to store the number of sensors that is found on the pin
byte tempType;				//  initializes the byte tempType
byte tempPrecision;			//	initializes the byte tempPrecision
byte tempReadDelay;			//	initializes the byte tempReadDelay
float tempRead[4];			//	array to hold the temperature readings taken
//	set the strings for the sensor names
char* tempSensorNameGraph[] = { "Desk R", "Comp A", "Desk L", "Comp C", "Temp 5" };		//	local sensor names for the graphic display
char* remoteSensorNameGraph[] = { "5.5g", "20g ", "2.5W", "2.5E", "6.6g", "" };		//  remote sensor names for the graphic display

//  INITIALIZE THE ALARM Variables
//  ***********************************************
#define LOOP_INTERVAL 1000	//	millis between log readings
byte AlarmEnable;			//  byte for storing all 8 alarm's enable flags as bits
byte AlarmState;			//  byte for storing all 8 alarm's state flags as bits
byte AlarmType[8];			//  type of alarm 0=Day Lights, 1=Night Lights, ""room to expand""
byte AlarmRelay[8];			//  # of the relay this alarm will trigger
byte AlarmHourOn[8];		//  hour time the alarm will come on at
byte AlarmMinOn[8];			//	minute time the alarm will come on at
byte AlarmHourOff[8];		//	hour time the alarm will go off at
byte AlarmMinOff[8];		//  minute time the alarm will go off at
byte RelayState;			//	byte for storing the state of all 8 relays
AlarmID_t tempReadID;		//  alarm ID for the temperature sensors
AlarmID_t tempRemoteReadID;	//  alarm ID for the remote temperature sensors
AlarmID_t flowReadID;		//	alarm ID for the reading of the flow sensor
AlarmID_t AlarmIDOn[8];		//  alarm IDs for each alarm's On event
AlarmID_t AlarmIDOff[8];	//	alarm IDs for each alarm's Off event


//  INITIALIZE THE FLOW SENSOR
//  ***********************************************
byte flowSensorInterrupt = 5;			//	interrupt to trigger for the flow sensor
byte flowSensorIntPin = 18;				//	pin on the arduino to use for the interrupt
byte flowSensorEnable;					//	initializes the byte flowSensorEnable
byte flowReadDelay = 30;				//	initializes the byte flowReadDelay (currently in seconds)
volatile uint16_t flowPulseCount = 0;	//	number of pulses trigger in the interrupt
uint16_t flowRateMax;					//	maximum flow rate as calibrated by the user
uint16_t flowRateMin;					//  minimum flow rate before an alarm is triggered, this is a percentage of flowRateMax
uint16_t flowPulseReading[5];			//	the array to store the last 5 flow pulse readings in
byte flowPulseIndex = 0;				//	the number that selects the number of the flowPulseReadings array to write to
uint16_t flowPulseTotal;				//	the variable used to store the running total of the last 5 flowPulseCount samples


//  INITIALIZE THE RELAYS
//  ***********************************************
byte relayPins[] = { 22, 23, 24, 25, 26, 27, 28, 29 };		//  Initialize the relay pins
byte relayCount = 7;		//  Set the number of relays


//  INITIALIZE THE SD CARD
//  ***********************************************
#define	SDCARD_WRITE_INTERVAL 60000		//	millis between SD Card writes
//#define SD_CHIP_SELECT 11		//	external sd card cs
#define SD_CHIP_SELECT 53		//	sd card slot on the touchscreen cs
uint32_t SDLastSyncTime = 0;			//	time of the last sync
uint8_t SDexist = 0;					//	variable to determine if there is a problem with the sd card.  If there is then dont use the SD card.
File SDLogfile;							//	initialize the file to log to


//  INITIALIZE THE LEDS
//  ***********************************************
#define SD_LED_BANK 0		//	the sdcard leds are on bank 0 of the MCP23017
#define RED_LED_PIN 0		//  located on port 0.0 of the MCP23017
#define GREEN_LED_PIN 1		//  located on port 0.1 of the MCP23017
#define BLUE_LED_PIN 2		//  located on port 0.2 of the MCP23017


void setup()
{
	delay(1000);	//	wait 1 seconds for things to settle down

//  SETUP THE SERIAL PORT
//  ***********************************************
	Wire.begin();
	Serial.begin(115200);			//  start the serial port

	//  SETUP THE RTC
	setSyncProvider(RTC.get);		//  this function get the time from the RTC

	if (timeStatus() != timeSet) {	//  checks to see if it can read the RTC
		RTC_Status = 0;
		Serial.println("Unable to get the RTC");
		Serial.println();
	}
	else { Serial.println("RTC system time"); }

	//  READ THE VARIABLES OUT OF THE EEPROM

	//  READ NON USER SETTINGS FROM EEPROM
	e2Empty = eeprom.read(0);			//	reads the e2Empty out to determine if it needs to set defaults
	if (e2Empty == 0) { factoryDefaultset(); }
	configID = eeprom.read(0);			//  reads the configID out

	serialDebug = eeprom.read(5);		//	reads out user setting to turn serial debugging on for each type of debugging

	//  READ USER SETTINGS FROM EEPROM
	tempType = eeprom.read(20);			//  reads out user setting to selects between 0 = Celsius or 1 = Fahrenheit
	tempPrecision = eeprom.read(21);		//	reads out user setting to selece the decimal precision of the temp sensors 0 = No Decimal or 1 = 1 Decimal
	tempReadDelay = eeprom.read(22);		//	reads out user setting for the amount of time in seconds between reading the temp sensors
	timeFormat = eeprom.read(23);		//	reads out user setting for the time format 0 = 24 hour and 1 = 12 hour
	secondsDisplay = eeprom.read(24);	//	reads out user setting to display seconds 0 = No and 1 = Yes
	backlightLevel = eeprom.read(25);	//	reads out the user setting to control the backlight level
	backlightTimeout = eeprom.read(26);	//	reads out the user setting to control when the backlight level times out
	flowSensorEnable = eeprom.read(27);	//	reads out the user setting to determine if the flow sensor is enabled
	flowRateMax = eeprom.read(28);		//	reads out the user setting that is set for the max flow rate as an integer
	flowRateMin = eeprom.read(29);		//	reads out the user setting for the minimum flow rate

	//  READ ALARM SETTINGS FROM EEPROM AND SETUP THE ALARMS IN THE TIMEALARMS LIBRARY
	tempReadID = Alarm.timerRepeat(tempReadDelay, ReadTempSensors);		//	sets an alarm to read the temp sensors at the specified delay and returns the Alarm_ID to tempReadID
	tempRemoteReadID = Alarm.timerRepeat(tempReadDelay, GetRemoteSensors);		//	sets an alarm to read the temp sensors at the specified delay and returns the Alarm_ID to tempReadID
	AlarmEnable = eeprom.read(100);		//	reads out the byte for the enable flags for all 8 alarms
	AlarmState = eeprom.read(101);		//	reads out the byte for the state flags for all 8 alarms
	RelayState = eeprom.read(150);

	if ((serialDebug & 4) == 4)	{
		Serial.println();
		Serial.println("ALARM EEPROM SETTINGS");
		Serial.println("ID ON  OFF En  Typ  ON     OFF   Relay");
	}

	for (uint8_t id = 0; id <= relayCount; id++) {		//  read each of the alarms values out of the EEPROM
		if ((serialDebug & 8) == 8) { serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop			
		AlarmType[id] = eeprom.read(102 + (id * 6));
		AlarmRelay[id] = eeprom.read(103 + (id * 6));
		AlarmHourOn[id] = eeprom.read(104 + (id * 6));
		AlarmMinOn[id] = eeprom.read(105 + (id * 6));
		AlarmHourOff[id] = eeprom.read(106 + (id * 6));
		AlarmMinOff[id] = eeprom.read(107 + (id * 6));
		AlarmIDOn[id] = Alarm.alarmRepeat(AlarmHourOn[id], AlarmMinOn[id], 0, AlarmON);
		AlarmIDOff[id] = Alarm.alarmRepeat(AlarmHourOff[id], AlarmMinOff[id], 30, AlarmOFF);

		if ((serialDebug & 4) == 4)	{
			//			   ID   ON   OFF
			Serial.printf("%d  %-2d  %-2d  ", id, AlarmIDOn[id], AlarmIDOff[id]);
			if ((AlarmEnable & (1 << id)) == (1 << id)) { Serial.print("ON "); }
			else { Serial.print("OFF"); }
			//			   TYP    ON        OFF
			Serial.printf("  %d  %2d:%02d  %2d:%02d  ", AlarmType[id], AlarmHourOn[id], AlarmMinOn[id], AlarmHourOff[id], AlarmMinOff[id]);
			Serial.println(AlarmRelay[id], BIN);
		}
	}

	flowReadID = Alarm.timerRepeat(flowReadDelay, FlowSensorRead);		//	sets an alarm to read the flow sensor at the specified dalay and returns the Alarm_IS to flowReadID
	if (flowSensorEnable == 0) {
		Alarm.disable(flowReadID);
		Serial.println("Flow Readings Disabled");
	}
	else {
		Alarm.enable(flowReadID);
		Serial.println("Flow Readings Enabled");
	}

	serialDebug = eeprom.read(5);		//	read out the serial debug again in case it was disable during the alarm print

	if ((serialDebug & 4) == 4)	{
		//uint8_t rd;
		Serial.println();
		Serial.print("AlarmEnable ");
		Serial.println(AlarmEnable, BIN);
		Serial.print("AlarmState ");
		Serial.println(AlarmState, BIN);
		Serial.print("RelayState ");
		Serial.println(RelayState, BIN);
		Serial.printf("# of Alarms %d\n", Alarm.count());
		Serial.printf("tempReadID %d, %d\n", tempReadID, Alarm.read(tempReadID));
		Serial.printf("flowReadID %d, %d\n", flowReadID, Alarm.read(flowReadID));
		Serial.println();
	}

//	SETUP THE MCP23017 PINS
//  ***********************************************
	mcpA.writeByte(IOCON, 0x80);		//	Switches the IOCON to use BANK = 1 register addresses
	mcpA.writeByte(GPPUA, 0xF8);		//	turn on the 100k pullups
	mcpA.writeByte(GPINTENA, 0x80);		//	enable the interrupt on GPIO pin
	mcpA.writeByte(IODIRA, 0xF8);		//	configure the pins for A0-2 (SD card LEDs) as outputs and A3-7 (Menu Buttons) as inputs.
	mcpA.writeByte(IODIRB, 0x00);		//	configure the pins for B0-7 as outputs  This is for the Relays
	mcpA.writeByte(DEFVALA, 0x80);		//	set the default value of the interrupt pin to be high
	mcpA.writeByte(INTCONA, 0x80);		//	set the interrupt to use the value of DEFVAL
	mcpA.writeByte(GPIOB, 0xFF);		//	write the relays to off
	mcpA.readByte(INTCAPA);				//	read from the interrupt port to clear any interrupts that happened
	mcpA.readByte(INTCAPB);				//	read from the interrupt port to clear any interrupts that happened

	pinMode(menuEnterIntPin, INPUT_PULLUP);		//	set the pin mode for the interrupt pin on the arduino
	attachInterrupt(digitalPinToInterrupt(menuEnterIntPin), MenuButtonPress, FALLING);		//  Attaches int.4, pin 19(RX1) on the Mega and Due and sets it to trigger on a low input from the menu button

	//  SETUP THE FLOW SENSOR
	pinMode(flowSensorIntPin, INPUT);		//	set the flow sensor as an input to the pin number in the variable
	digitalWrite(flowSensorIntPin, HIGH);	//	write the pin high to be active low

//	SETUP THE TFT LCD
//  ***********************************************
	// Set the backlight for the LCD
	pinMode(TFT_B_LIGHT_PIN, OUTPUT);
	analogWrite(TFT_B_LIGHT_PIN, backlightLevel);

	TFT.InitLCD(LANDSCAPE);
	TFT.clrScr();
	TFT.setFont(GroteskBold16x32);
	TFT.fillScr(VGA_BLUE);
	TFT.setBackColor(VGA_BLUE);
	TFT.setColor(VGA_WHITE);

	Touch.InitTouch();
	Touch.setPrecision(PREC_MEDIUM);

	StartScreen();		//  call the start up screen function

//  SETUP THE DS18B20 SENSORS
//  ***********************************************
//  Check to see how many sensors are on the pin and put the addresses into the tempSensorAddr[] array

	//	unknown why, but i have to add these again to get it working correctly
	new OneWire(TEMP_SENSOR_PIN);		//	setup a new instance of OneWire
	new DallasTemperature(&oneWire);	//	setup a new instance of DallasTemperature
	delay(1000);						//	small delay to allow it to get the number of sensors
	tempSensors.begin();				//	start the DallasTemperature library

	//	try 3 times to get the device count of the onewire pin
	for (byte j = 0; j < 3; j++)	{
		numberOfSensors = tempSensors.getDeviceCount();		//	get the number of devices found on the bus

		//	if there were no sensors found, delay, print if needed, and repeat
		if (numberOfSensors == 0) {
			delay(500);
			if ((serialDebug & 1) == 1) { Serial.printf("Did not find any sensors! Retry #%d", j); }		//	print the retry message if sensor debuggint is on
		}
		//	this is for debugging, if it shows 5 sensors, it means that none were detected
		if (numberOfSensors == 0) { numberOfSensors = 4; }
	}
	//	Print the number of sensors on the pin to the serial port
	if ((serialDebug & 1) == 1)	{
		Serial.printf("Found %d temp sensors on pin %d", numberOfSensors, TEMP_SENSOR_PIN);
		Serial.println();
	}
	// Search the wire for address
	for (byte j = 0; j < numberOfSensors; j++) {
		
		//	print the address to the serial port and set the resolution of the sensor
		if (tempSensors.getAddress(tempSensorAddr[j], j)) {
			//	print the found sensors address to the serial port
			if ((serialDebug & 1) == 1)	{
				String addr;	//	variable to store the address string
				Serial.printf("Found %s with address ", tempSensorNameGraph[j]);
				addr = convertTempSensorAddress(tempSensorAddr[j]);		//	convert the address to a string
				Serial.println(addr);		//	print the address string
			}
			tempSensors.setResolution(tempSensorAddr[j], TEMP_SENSOR_PRECISION);	// set the resolution to TEMPERATURE_PRECISION bit
		}
		else {
			//	if there isnt an address, print the following
			if ((serialDebug & 1) == 1)	{
				Serial.printf("Found ghost device at %d but could not detect address. Check power and cabling", j);
			}
		}
	}
	if ((serialDebug & 1) == 1) { Serial.println(); }		//	Print a line space for the next function

//	SETUP THE NRF24LO1+ RADIO
//  ***********************************************
	radio.begin();
	radio.openWritingPipe(addresses[0]);	// "00002"	TX
	radio.openReadingPipe(1, addresses[1]); // "00001"	RX
	Serial.println("Master Set");
	radio.setPALevel(RF24_PA_HIGH);			//	set the radio PA to High

//	INITIALIZE THE SD CARD
//  ***********************************************
	Serial.println("Initializing the SD Card...");
	pinMode(SD_CHIP_SELECT, OUTPUT);		//	the chipselect line of the SD Card. always configure it to an output

	//	Determine if the SD card is present or not
	if (!SD.begin(SD_CHIP_SELECT)) {
		Serial.println("Card Failed, or Card is not present");
		SDexist = 0;
		mcpA.writeBit(SD_LED_BANK, RED_LED_PIN, 1);
	}
	else {
		Serial.println("SD card initialized.");
		SDexist = 1;
	}
	//	CREATE A NEW FILE IF THE SD CARD EXISTS
	if (SDexist == 1) {
		char filename[] = "LOGGER00.CSV";
		for (uint8_t i = 0; i < 100; i++) {
			filename[6] = i / 10 + '0';
			filename[7] = i % 10 + '0';
			if (!SD.exists(filename)) {
				SDLogfile = SD.open(filename, FILE_WRITE);	//	only open a new file if it doesn't exist
				break;
			}
		}
		if (!SDLogfile){ error("Could not create a file"); }		//	if there is an error call the error function with the error

		//	print the filename that was created in the code above
		Serial.printf("Logging to: %s\n", filename);

		//	CREATE A HEADER FOR THE LOGFILE
		SDLogfile.println("millis, stamp, time, ,temptype, temp1, temp2, temp3, temp4, relaystate");
	}

	//	CLEAR THE TFT SCREEN
	//  ***********************************************
		TFT.clrScr();
		TFT.fillScr(VGA_BLUE);
		TFT.drawBitmap(10, 240, 32, 32, gear);
		TFT.drawBitmap(45, 240, 32, 32, arrow_left);
		TFT.drawBitmap(80, 240, 32, 32, arrow_up);
		TFT.drawBitmap(115, 240, 32, 32, arrow_down);
		TFT.drawBitmap(150, 240, 32, 32, arrow_right);

	//	SETUP THE RELAYS TO DISPLY AND TURN THEM ON IF NEEDED
	//  ***********************************************
	RelayStatusDisplay(0, 3);				//	call the relay status display function
	RelayToggle(RelayState, 1);				//	Turn on the relays according to the AlarmState byte

	//	TAKE A TEMP READING AND START THE LOOP
	//  ***********************************************
	if ((serialDebug & 1) == 1){ Serial.println(); }
	ReadTempSensors();
	Serial.printf("Starting Loop :%u\n", millis());

	//	RESET MENU MODE
	menuMode = 0;						//	set the menuMode variable to make sure it is set to 0 when the loop starts

	RelayToggleALL();		//**********NICE SPOT TO TEST RELAYS**************
	TestSDLEDS();			//**********NICE SPOT TO TEST SDLEDS**************
}

void loop()
{
	//  calls the MenuTitle as long as menuMode = 1
	if (menuMode == 1) {
		Serial.println("Entering Menu");
		//CharMenuTitle();
		Serial.println("Exiting Menu");
	}

	//  only calls LCDDateDisplay if the RTC has been set
	if (RTC_Status == 1){ LCDDateDisplay(0, 0, 1, 0); }

	//	update the time
	TimeDisplay();

	//	check to see if the settings icon was touched
	if (Touch.dataAvailable()) { ReadTouchScreen();	}	//	read the data from the touchscreen

	//	delay for the ammount of time selected between readings
	Alarm.delay(((LOOP_INTERVAL - 1) - (millis() % LOOP_INTERVAL)));	//  uses the Alarm.delay to use the timer
}

void AlarmON()
{
	String alarmstring = "";
	uint8_t id;
	id = Alarm.getTriggeredAlarmId();
	id = ((id - 1) / 2);

	Serial.printf("Alarm %d ON @ %d:%02d\n", id, hour(), minute());
	RelayToggle(AlarmRelay[id], 1);

	if ((serialDebug & 4) == 4)	{
		uint8_t trigger;
		trigger = Alarm.getNextTrigger();
		Serial.printf("Next %d\nAlarm State ", trigger);
		Serial.println(AlarmState, BIN);
		Serial.println();
	}
}

void AlarmOFF()
{
	uint8_t id;
	id = Alarm.getTriggeredAlarmId();
	id = ((id - 2) / 2);
	Serial.printf("Alarm %d OFF @ %d:%02d\n", id, hour(), minute());
	RelayToggle(AlarmRelay[id], 0);

	if ((serialDebug & 4) == 4)	{
		uint8_t trigger;
		trigger = Alarm.getNextTrigger();
		Serial.printf("Next %d\nAlarm State ", trigger);
		Serial.println(AlarmState, BIN);
		Serial.println();
	}
}

void RelayToggleALL()
//	toggle all the relays on then toggle them off
//	this function is used as a test for the relays.
//	NOTE:  this will not toggle the relay display on the LCD as I do not do anything with the RELAYSTATE
{
	for (uint8_t relay = 0; relay <= relayCount; relay++) {	//	loop through the relays and turn each one on
		mcpA.writeBit(1, relay, 0);			//	writes the selected port low to turn the relay on
		delay(200);
	}
	for (uint8_t relay = 0; relay <= relayCount; relay++) {	//	loop through the relays and turn each one off
		mcpA.writeBit(1, relay, 1);			//	writes the selected port low to turn the relay off
		delay(200);
	}
}

void RelayToggle(uint8_t relay, uint8_t onoff)
//	turns on or off the selected relays if the state passed differs from 
//	relay = the relay that should be toggled
//	onoff = should be a 0 for off or 1 for on
{
	if ((serialDebug & 16) == 16) {
		Serial.print("Before state ");
		Serial.print(relay, BIN);
		Serial.print(" RelayState ");
		Serial.println(RelayState, BIN);
	}
	//	read all relays and isolate them 1 by 1 to determine if it needs turned on or off
	for (uint8_t i = 0; i <= relayCount; i++) {
		uint8_t rl;		//	variable to store the single relay
		rl = (relay & (1 << i));		//	isolate a single relay to see if its bit is set to 1 or 0
		
		//	switch to turn on or off the relay
		switch (onoff) {
		case 0:		//  turns the relay off
			if (rl > 0)	{
				//	Only changes if there is not a 0 for that bit.  this prevents changing the relaystate if there it is alread on
				if ((RelayState & (1 << i)) != 0) { RelayState = RelayState ^ (1 << i); }	//	toggles the bit held in RelayState
				mcpA.writeBit(1, i, 1);			//	turns the relay off
			}
			break;
		case 1:		//	turns the relay on
			if (rl > 0)	{
				//	Only changes if there is not a 1 for that bit.  this prevents changing the relaystate if there it is alread on
				if ((RelayState & (1 << i)) != (1 << i)) { RelayState = RelayState ^ (1 << i); }	//	toggles the bit held in RelayState
				mcpA.writeBit(1, i, 0);			//	turns the relay on
			}
			break;
		}
		delay(200);
	}
	if ((serialDebug & 16) == 16) {
		Serial.print("After state ");
		Serial.print(relay, BIN);
		Serial.print(" RelayState ");
		Serial.println(RelayState, BIN);
	}

	eeprom.write(150, RelayState);		//	write the relaystate to the eeprom
	RelayState = eeprom.read(150);		//	read the relaystate out of the eeprom
	RelayStatusDisplay(0, 3);			//	call the RelayStatusDisplay to write to the displays
}

void RelayStatusDisplay(uint8_t col, uint8_t row)
//	write the status of the relays to the LCD screen
//	col = the column of the character display that you want to print to
//	row = the row of the character display that you want to print to
{
	String relayString = "";	//	variable used to store to string to print to the display
	TFT.setFont(GroteskBold24x48);	//	set the font for the 4.3" touchscreen

	//	read RelayState to get all relays assembled into a strin
	for (uint8_t i = 0; i <= relayCount; i++) {
		//	isolate each relay and determine whether it is on or off
		if ((RelayState & (1 << i)) == (1 << i)) { relayString = relayString + "+"; }
		else relayString = relayString + "-";;
		}

	//	Print to the 4.3" LCD touch screen
	//	***************************************
	int x = (col * 24);
	int y = ((row + 1) * 48) + 35;
	TFT.print(relayString, RIGHT, y);
}

void ReadTouchScreen()
{
	Touch.read();	//	request the coordinates from the touch screen
	int x = Touch.getX();	//	get the x coordinate from the touch screen
	int y = Touch.getY();	//	get the y coordinate from the touch screen

	//	if the settings button location was touched, run the menu
	if ((y >= 240) && (y <= 272)); {	//	y location of the button
		if ((x >= 10) && (x <= 42)) {	//	x location of the button
										//	Test code to change the color of the screen
			TFT.clrScr();
			Menu.EnterMenu();
			readEepromVariables();
			TFT.fillScr(VGA_BLUE);
			TFT.setBackColor(VGA_BLUE);
			TFT.drawBitmap(10, 240, 32, 32, gear);
			TFT.drawBitmap(45, 240, 32, 32, arrow_left);
			TFT.drawBitmap(80, 240, 32, 32, arrow_up);
			TFT.drawBitmap(115, 240, 32, 32, arrow_down);
			TFT.drawBitmap(150, 240, 32, 32, arrow_right);
		}
	}
}

void StartScreen()
//	print the startup screen to the selected LCD screen
{
	int y = 0;							//	y variable for rows
	String versionString;				//	create a string for the version
	char buffer[16];					//	create a buffer for the time string
	sprintf(buffer, "Version %d.%02d.%02d", version, build, subbuild);	//	prepare the string to print for the version
	versionString = String(buffer);		//	create a string for the version

	//	Print to the 4.3" LCD touch screen
	//	***************************************
	TFT.setFont(GroteskBold24x48);	//	set the font
	TFT.print("GLITCHSNIFFER'S", CENTER, 31, 0);
	TFT.print("AQUARIUM", CENTER, 85, 0);
	TFT.print("CONTROLLER", CENTER, 139, 0);
	TFT.print(versionString, CENTER, 193, 0);
}

void TimeDisplay()	//	MUST ASSEMBLE THE TIME STRING USING	TimeString() BEFORE CALLING THIS FUNCTION
{
	uint8_t col = 0;
	//	adjusts for 12 or 24 hour
	switch (secondsDisplay)		//	if seconds are to be displayed the add some space to the colums
	{
	case 0:
		if (timeFormat == 0) { col = 2; }
		else { col = 1; }
		break;
	case 1:
		if (timeFormat == 0) { col = 1; }
		else { col = 0; }
		break;
	}
	timestr = TimeString(0, hour(), minute(), second());
	PrintTimeDisplay(timestr, col, 0, 10);
}
	
void PrintTimeDisplay(String timestring, uint8_t col, uint8_t row, uint8_t maxlength)
//	timestring = the string to pring
//	col and row are the absolute start of the print.  this will be modified to center the print later
//	hour, min, sec are passed to the function as either the time of the time you want displayed.
//	maxlength is the max lenght you would expect the string to be.  this allow for a variable ammount of spaces needed to clear the previous line
{
	uint8_t length = timestring.length();		//	get the length of the time string to use in determining where to start the cursor

	int x = (col * 24);					//	use col variable to decide where to start
	int y = (row * 48) + 5;				//	use row variable to decide where to start + 5 to pad the rows
	x = ((240 - (length * 24)) / 2);	//	determine where to start printing to center the date

	// 4 is the minimum ammount of digits to display.  clear preceeding digits when appropriate.
	TFT.setFont(GroteskBold24x48);	//	set the font
	TFT.print(timestring, x, y);		//	print the date string to x and y coordinates
}

String TimeString(byte disp, uint8_t hour, uint8_t min, uint8_t sec)
//	function to return a formatted string to represent the time
//	disp is used for options
//		0 = No options
//		1 = force seconds display off.
//		2 = force seconds display on.
//		3 = force seconds display off and add a space between mins and AMPM
//	hour, min, sec are passed to the function as the time you want displayed.
{
	uint8_t realhour = hour;	//	stores the hour to be used for the AMPM time if needed
	String temp;				//	string to store and manipulate the individual data
	char buffer[12];			//	create a buffer for the time string
	char indbuf[3];				//	create a buffer for the individual items to print

	//	assemble the hour string
	switch (timeFormat)		
	{
	case 0:		//	if 24 hour use 2 digits
		sprintf(indbuf, "%02d", hour);	//	0 pad the hours
		temp = String(indbuf);		//	store the hour string
		break;
	case 1:		//	if 12 hour set the cursor to account for # of hour digits
		if (hour > 12) { hour = hour - 12; }	//	adjust for PM hours
		if (hour == 0) { hour = 12; }	//	adjust for midnight
		sprintf(indbuf, "%2d", hour);	//	store the hour string
		temp = String(indbuf);		//	store the hour string in the buffer
		break;
	}
	timestr = temp + ":";	//	add the hour and : to the timestring

	//	assemble the minute string
	sprintf(indbuf, "%02d", min);	//	0 pad the minutes
	temp = String(indbuf);		//	store the min string in the buffer
	timestr = timestr + temp;	//	add the mins to the timestring

	//	assemble the seconds string if they are set to display
	if ((secondsDisplay == 1) || (disp == 2))	{		//  only display seconds if disp forces it or they are set to display in the config
		if ((disp & 1) != 1) {		//	if the disp byte is not set to force seconds digits off, print the seconds
			sprintf(indbuf, "%02d", sec);	//	0 pad the seconds
			temp = String(indbuf);		//	store the secs string in the buffer
			timestr = timestr + ":" + temp;	//	add the seconds to the timestring
		}
	}

	//	add a space to the string if disp = 3
	if (disp == 3) { timestr = timestr + " "; }

	//  assemble the am/pm string if it is set to display
	if (timeFormat == 1) {	//	only display AMPM if it is set to 12 hour time
		if (realhour >= 12) { temp = "PM"; }
		else if (realhour <= 11) { temp = "AM"; }
		timestr = timestr + temp;		//	add ampm to the timestring
	}
	
	return timestr;	//	return the fully assembled time string
}

void LCDDateDisplay(byte display, uint8_t col, uint8_t row, uint8_t pad)
//	display = 0 to not change the date if it is the same day
//	col = the column to start the print on the character display
//	row = the row to start the print on the character display
//	pad = 1 to zero pad the date to be displayed
{
	if (display == 0 && day() == today) { return; }		//	if the day hasn't changed, dont refresh it	
	else {
		//	prepare the date string
		//	***************************************
		int m = month();			//	convert the month to an integer to combine in the string
		int d = day();				//	convert the day to an integer to combine in the string
		int y1 = year();				//	convert the year to an integer to combine in the string
		int length = 0;				//	variable to store the length of the string
		String datestring = "";		//	create a string for the date
		char buffer[12];			//	create a buffer to hold the date string

		if (pad = 1) { sprintf(buffer, "%d/%02d/%d", m, d, y1); }
		else { sprintf(buffer, "%d/%d/%d", m, d, y1); }	//	assemble the date as a string
		datestring = String(buffer);			//	convert the char array to a string
		length = datestring.length();			//	get the length of the date string to use in determining where to start the cursor

		////	Print to the 4.3" LCD touch screen
		////	***************************************
		int x = (col * 24);					//	use col variable to decide where to start
		int y = (row * 48) + 5;				//	use row variable to decide where to start + 5 to pad the rows
		x = ((240 - (length * 24)) / 2);	//	determine where to start printing to center the date

		TFT.print("          ", x, y);	//	this will clear the current spot for the date and allow an shorter date to be written clearly
		TFT.setFont(GroteskBold24x48);	//	set the font
		TFT.print(datestring, x, y);		//	print the date string to x and y coordinates
		today = day(); //	set the day = to today so that it doesn't refresh the display with it until tomorrow
	}
}

void ReadTempSensors()
//  Read the DS18B20 sensors that are attached to the oneWire bus on the TEMP_SENSOR_PIN and print to the appropriate device
{
	//	have all the sensors on the bus start a temperature conversion
	tempSensors.requestTemperatures();

	//	cycle through each one of the sensors
	for (uint8_t j = 0; j < numberOfSensors; j++) {   // loop through the number of sensors found
		String addrString;				//	string to print the address
		uint8_t sensorConnected = 0;	//	int to compare if the current sensor is connected and readable
		int i = 0;	//	integer to store the integer portion of the sensor reading
		int d = 0;	//	integer to store the decimal portion of the sensor reading
		int nodec = 0;

		//	convert the address of the sensor to a string
		addrString = convertTempSensorAddress(tempSensorAddr[j]);

		// determine if there is a sensor connected to the current address
		if (tempSensors.isConnected(tempSensorAddr[j]) == true) {
			sensorConnected = 1;	//	see if there is a sensor at the given address
			if (tempType == 0) tempRead[j] = tempSensors.getTempC(tempSensorAddr[j]);	//	request the temp data from the sensor in C
			else tempRead[j] = tempSensors.getTempF(tempSensorAddr[j]);					//	request the temp data from the sensor in F
		}
		else sensorConnected = 0;

		//	convert the float to 2 integer parts.  this is necessary because the mega board cannot handle a printf with a float
		i = int(tempRead[j]);		//	get the integer portion of the reading
		d = round((tempRead[j] - i) * 10);	//	round the decimal portion of the reading and make it an integer
		if (d == 10) { d = 1; }				//	prevent the overflow of the decimal if it was equal to 1 before the int conversion

		//	Print to the 4.3" LCD touch screen
		//	***************************************
		int y = (j * 32) + 3;		//	use row variable to decide where to start + 5 to pad the rows
		String tempString;			//	string to store the completed temp string to print to the screen
		char buffer[20];			//	buffer to store the sprintf data

		switch (tempPrecision) {	//	decide what temp precision to print
		case 0:	//	0 decimal
			if (sensorConnected == 1) {	//	if there is a sensor connected then print the following
				if (tempType == 0)	sprintf(buffer, "  %s%3d`C", tempSensorNameGraph[j], round(tempRead[j]));	//	assemble the string to print if C and no decimal point
				else  sprintf(buffer, "  %s%3d`F", tempSensorNameGraph[j], round(tempRead[j]));					//	assemble the string to print if F and no decimal point
			}
			else sprintf(buffer, "  %s ----", tempSensorNameGraph[j]);	//	print nothing if there isnt a sensor connected
			break;
		case 1:	//	1 decimal
			if (sensorConnected == 1) {	//	if there is a sensor connected then print the following
				if (tempType == 0)	sprintf(buffer, "%s%3d.%d`C", tempSensorNameGraph[j], i, d);	//	assemble the string to print if C and 1 decimal point
				else  sprintf(buffer, "%s%3d.%d`F", tempSensorNameGraph[j], i, d);				//	assemble the string to print if F and 1 decimal point
			}
			else sprintf(buffer, "%s ------", tempSensorNameGraph[j]);	//	print nothing if there isnt a sensor connected
			break;
		}

		tempString = buffer;	//	save the buffer

		TFT.setFont(GroteskBold16x32);	//	set the font
		TFT.print(tempString, RIGHT, y);	//	print the date string to x and y coordinates

		//	Print to the serial port
		//	***************************************
		if ((serialDebug & 1) == 1) {
			if (sensorConnected == 1) {			//	if there is a sensor connected then print the following
				Serial.print(addrString);		//	print the string for the address
				Serial.printf(", %s %3d.%d", tempSensorNameGraph[j], i, d);  //	print the name of the sensor and the reading
				if (tempType == 0) Serial.println("C");		//	print the type in C
				else Serial.println("F");					//	print the type in F
			}
			else {
				Serial.print(addrString);		//	print the string for the address
				Serial.printf(", %s No Sensor Found", tempSensorNameGraph[j]);	//	print that no sensor is found
				Serial.println();
			}
		}
	}
	if ((serialDebug & 1) == 1)	Serial.println();	//	print a line break
	if(SDexist == 1){ logger(); }	//	log the data if the SD card is present
}

void GetRemoteSensors()
//	This function will send the command to all nodes to send in their temperatures
{
	TFT.setFont(GroteskBold16x32);	//	set the font

	char buffer[20];			//	buffer to store the sprintf data
	

	for (byte i = 1; i < 4; i++)
	{
		int timeoutFlag = 0;
		comm.com_address = i;
		comm.command = 1;
		//	Send the command
		radio.stopListening();
		radio.write(&comm, sizeof(Command_Package));
		Serial.printf("Command %d Sent to Address %d\n", comm.command, comm.com_address);

		radio.startListening();
		Serial.print("Waiting\n");

		// Check whether we keep trying to recieve data or we timeout
		startTime = millis();				// set the time for a timeout comparison

		while (!radio.available()) {
			//	if it has been more than second since we started listening break from the while loop
			currentTime = millis();
			if (currentTime - startTime > 700) {
				Serial.print("Timout Occured\n\n");
				timeoutFlag = 1;
				break;
			}
		}

		Serial.printf("i = %d, t/o = %d\n", i, timeoutFlag);

		if (radio.available()) {
			radio.read(&data, sizeof(Data_Package));
			Serial.print("Recieved\n");
			Serial.printf("Add = %d, T1 = %d.%d: T2 = %d.%d\n", data.origin_addr, data.temp_1I, data.temp_1D, data.temp_2I, data.temp_2D);
		}

		switch (i)	//	use the node orgin address to determine the variables to store the data
		{
		case 1:		//	node 1 sensor readings
			remoteTemp.temp_1_int = data.temp_1I;
			remoteTemp.temp_1_dec = data.temp_1D;
			remoteTemp.temp_2_int = data.temp_2I;
			remoteTemp.temp_2_dec = data.temp_2D;
			if (timeoutFlag == 1) {
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[0], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, LEFT, 135);
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[1], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, LEFT, 170);
			}
			else {
				Serial.printf("Addr %d, T1 %d.%d, T2 %d.%d\n\n", data.origin_addr, remoteTemp.temp_1_int, remoteTemp.temp_1_dec, remoteTemp.temp_2_int, remoteTemp.temp_2_dec);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[0], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, LEFT, 135);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[1], remoteTemp.temp_2_int, remoteTemp.temp_2_dec);
				TFT.print(buffer, LEFT, 170);
			}
			break;
		case 2:		//	node 2 sensor readings
			remoteTemp.temp_3_int = data.temp_1I;
			remoteTemp.temp_3_dec = data.temp_1D;
			remoteTemp.temp_4_int = data.temp_2I;
			remoteTemp.temp_4_dec = data.temp_2D;
			if (timeoutFlag == 1) {
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[2], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, LEFT, 203);
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[3], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, RIGHT, 135);
			}
			else {
				Serial.printf("Addr %d, T1 %d.%d, T2 %d.%d\n\n", data.origin_addr, remoteTemp.temp_3_int, remoteTemp.temp_3_dec, remoteTemp.temp_4_int, remoteTemp.temp_4_dec);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[2], remoteTemp.temp_3_int, remoteTemp.temp_3_dec);
				TFT.print(buffer, LEFT, 203);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[3], remoteTemp.temp_4_int, remoteTemp.temp_4_dec);
				TFT.print(buffer, RIGHT, 135);
			}
			break;
		case 3:		//	node 3 sensor readings
			remoteTemp.temp_5_int = data.temp_1I;
			remoteTemp.temp_5_dec = data.temp_1D;
			remoteTemp.temp_6_int = data.temp_2I;
			remoteTemp.temp_6_dec = data.temp_2D;
			if (timeoutFlag == 1) {
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[4], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, RIGHT, 170);
				sprintf(buffer, "%s 00.0`F", remoteSensorNameGraph[5], remoteTemp.temp_1_int, remoteTemp.temp_1_dec);
				TFT.print(buffer, RIGHT, 203);
			}
			else {
				Serial.printf("Addr %d, T1 %d.%d, T2 %d.%d\n\n", data.origin_addr, remoteTemp.temp_5_int, remoteTemp.temp_5_dec, remoteTemp.temp_6_int, remoteTemp.temp_6_dec);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[4], remoteTemp.temp_5_int, remoteTemp.temp_5_dec);
				TFT.print(buffer, RIGHT, 170);
				sprintf(buffer, "%s %d.%d`F", remoteSensorNameGraph[5], remoteTemp.temp_6_int, remoteTemp.temp_6_dec);
				TFT.print(buffer, RIGHT, 203);
			}
			break;
		default:
			break;
		}
	}
	radio.stopListening();
}

String convertTempSensorAddress(DeviceAddress deviceAddress)
//	this function will convert the DS18B20 sensor address from a 64bit integer to a string that can be printed easily
//	devAddress = the address of the sensor that this function will print
{
	String addrString;		//	string to hold the address

	//	cycle through all 8 bytes of the sensor address
	for (uint8_t i = 0; i < 8; i++)	{
		if (deviceAddress[i] < 16) addrString = addrString + "0";	//	zero pad the address if necessary
		addrString = addrString + String(deviceAddress[i], HEX);	//	add the current byte to the string
	}
	addrString.toUpperCase();	//	convert the string to all uppercase
	return addrString;			//	return the string
}

void logger()
{
	time_t t;	//	variable to store the time

	mcpA.writeBit(SD_LED_BANK,GREEN_LED_PIN, 1);	//	turn on the green led

	if ((serialDebug & 1) == 1)
	{Serial.println("Preparing Log Data");}

	// log the millis since starting
	uint32_t m = millis();		//	set the current millis to m
	SDLogfile.print(m);
	SDLogfile.print(", ");

	if ((serialDebug & 1) == 1) { Serial.printf("%d, ", m); }

	t = now();		//	fetch the current time

	//	print the log time
	SDLogfile.print(t);
	SDLogfile.print(", ");
	SDLogfile.print(year(t), DEC);
	SDLogfile.print("/");
	SDLogfile.print(month(t), DEC);
	SDLogfile.print("/");
	SDLogfile.print(day(t), DEC);
	SDLogfile.print(" ");
	SDLogfile.print(hour(t), DEC);
	SDLogfile.print(":");
	SDLogfile.print(minute(t), DEC);
	SDLogfile.print(":");
	SDLogfile.print(second(t), DEC);
	//	echo to the serial port
	if ((serialDebug & 1) == 1)	{
		Serial.print(t);
		Serial.print(", ");
		Serial.print(year(t), DEC);
		Serial.print("/");
		Serial.print(month(t), DEC);
		Serial.print("/");
		Serial.print(day(t), DEC);
		Serial.print(" ");
		Serial.print(hour(t), DEC);
		Serial.print(":");
		Serial.print(minute(t), DEC);
		Serial.print(":");
		Serial.println(second(t), DEC);
	}
	//	log the data
	switch (tempType)
	{
	case 0:
		SDLogfile.print(", ");
		SDLogfile.print("C");
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[0], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[1], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[2], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[3], 2);
		SDLogfile.print(", ");
		SDLogfile.println(RelayState, BIN);
		if ((serialDebug & 1) == 1) {
			Serial.print("C");
			Serial.print(", ");
			Serial.print(tempRead[0], 2);
			Serial.print(", ");
			Serial.print(tempRead[1], 2);
			Serial.print(", ");
			Serial.print(tempRead[2], 2);
			Serial.print(", ");
			Serial.print(tempRead[3], 2);
			Serial.print(",");
			Serial.println(RelayState, BIN);
		}
		break;
	case 1:
		SDLogfile.print(", ");
		SDLogfile.print("F");
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[0], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[1], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[2], 2);
		SDLogfile.print(", ");
		SDLogfile.print(tempRead[3], 2);
		SDLogfile.print(", ");
		SDLogfile.println(RelayState, BIN);
		if ((serialDebug & 1) == 1)	{
			Serial.print("F");
			Serial.print(", ");
			Serial.print(tempRead[0], 2);
			Serial.print(", ");
			Serial.print(tempRead[1], 2);
			Serial.print(", ");
			Serial.print(tempRead[2], 2);
			Serial.print(", ");
			Serial.print(tempRead[3], 2);
			Serial.print(",");
			Serial.println(RelayState, BIN);
		}
		break;
	}

	mcpA.writeBit(SD_LED_BANK, GREEN_LED_PIN, 0);	//	turn the green led off

	//  Write the data to disk if the millis are more than the write interval
	if ((millis() - SDLastSyncTime) < SDCARD_WRITE_INTERVAL) { return; }
	SDLastSyncTime = millis();

	//	write the logfiles data to the SD Card
	if ((serialDebug & 1) == 1)	{
		Serial.println("Writing log to the SD Card");
		Serial.println();
	}
	mcpA.writeBit(SD_LED_BANK, BLUE_LED_PIN, 1);	//	turn the blue led on
	SDLogfile.flush();	//	flush the log to the SD card
	mcpA.writeBit(SD_LED_BANK, BLUE_LED_PIN, 1);	//	turn the blue led off
}

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
	Serial.printf("Flow ON @ %d\n", now());
	flowStartTime = millis();											//	set the current millis to the start time
	attachInterrupt(flowSensorInterrupt, FlowSensorCounter, FALLING);	//	enable the interrupt to start the reading
	Alarm.delay(rdtime);												//	delay for the ammount of time in millis
	detachInterrupt(flowSensorInterrupt);								//	disable the interrupt for the flow sensor
	flowEndTime = millis();												//	set the current millis to the end time
	Serial.printf("Flow OFF @ %d\n", now());
	Serial.println();

	//	store and calculate the average of the last 5 variables
	flowPulseTotal = flowPulseTotal - flowPulseReading[flowPulseIndex];	//	Subtract the array position to be added to below
	flowPulseReading[flowPulseIndex] = flowPulseCount;					//	set the current reading to the array variable
	flowPulseTotal += flowPulseReading[flowPulseIndex];					//  add the current reading to the total
	flowPulseIndex++;													//	increase the index by 1
	if (flowPulseIndex > 4) { flowPulseIndex = 0; }						//	if index is at the 5th variable, reset to 0

	//	print the flow status to the LCD
	//	can use this area to determine an alarm alert to the user.
	//////if (flowRateMax > 0) {
	//////	lcd.setCursor(0, 2);
	//////	if (flowPulseTotal / 5 <= flowRateMax * (0.01 * flowRateMin)) { lcd.print("Flow Alarm"); }
	//////	else lcd.print("Flow Good ");
	//////}
	//	in case the millis counter overflows to 0 after approx 50 days, recalculate the time for the next flow reading
	if (flowEndTime < flowStartTime) {
		flowEndTime = (4294967295 - flowStartTime) + 1 + flowEndTime;	//	set flowEndTime to the total duration
		flowStartTime = 0;												//	set the start time to 0 so that the endtime is the total duration
	}

	//	calculate the flow rate in mL based on the flowPulseCounter
	flowRate = (flowPulseFactor * flowPulseCount) / ((flowEndTime - flowStartTime) / 1000);		//	in mL

	//	print the stats to the Serial port
	if ((serialDebug & 64) == 64)
	{
		Serial.printf("Duration %d-%d=%d\n", flowEndTime, flowStartTime, (flowEndTime - flowStartTime));

		//Serial.print("Duration ");
		//Serial.print(flowEndTime);
		//Serial.print("-");
		//Serial.print(flowStartTime);
		//Serial.print("=");
		//Serial.println(flowEndTime - flowStartTime);
		Serial.printf("PulseCount %d, PulseTotal %d, Avg %d\n", flowPulseCount, flowPulseTotal, (flowPulseTotal / 5) );
		Serial.printf("Flow Rate = %d\n", flowRate);
		//Serial.print("PulseCount ");
		//Serial.println(flowPulseCount);
		//Serial.print("PulseTotal ");
		//Serial.print(flowPulseTotal);
		//Serial.print(", Avg ");
		//Serial.println(flowPulseTotal / 5);
		//Serial.print("Rate ");
		//Serial.println(flowRate);

		//  print the flow rate in L/h and G/h
		flowRate = flowRate / 10;

		Serial.printf("Rate %dL/h", (flowRate * 60));
		//Serial.print("Rate ");
		//Serial.print(flowRate * 60);
		//Serial.print("L/h");

		flowRate = flowRate * 0.264172;		//	convert to gallons

		Serial.printf(", %dG/h\n", (flowRate * 60));
		//Serial.print(", ");
		//Serial.print(flowRate * 60);
		//Serial.println("G/h");
		Serial.println();
	}
	//	reset the pulse counter
	flowPulseCount = 0;
}

void FlowSensorCounter()
//	the interrupt routine to count the ammout of pulses of the flow meter
{
	flowPulseCount++;
}

void MenuButtonPress()
{
	if ((long)(micros() - last_micros) >= debouncing_time * 1000)
	{
		menuMode = !menuMode;		//  handler toggle for the menuMode after a button press interrupt
		last_micros = micros();
	}
}

void readEepromVariables()
//  READ THE VARIABLES OUT OF THE EEPROM
{
//  READ NON USER SETTINGS FROM EEPROM

	serialDebug = eeprom.read(5);		//	reads out user setting to turn serial debugging on for each type of debugging

	//  READ USER SETTINGS FROM EEPROM
	tempType = eeprom.read(20);			//  reads out user setting to selects between 0 = Celsius or 1 = Fahrenheit
	tempPrecision = eeprom.read(21);		//	reads out user setting to selece the decimal precision of the temp sensors 0 = No Decimal or 1 = 1 Decimal
	tempReadDelay = eeprom.read(22);		//	reads out user setting for the amount of time in seconds between reading the temp sensors
	timeFormat = eeprom.read(23);		//	reads out user setting for the time format 0 = 24 hour and 1 = 12 hour
	secondsDisplay = eeprom.read(24);	//	reads out user setting to display seconds 0 = No and 1 = Yes
	backlightLevel = eeprom.read(25);	//	reads out the user setting to control the backlight level
	backlightTimeout = eeprom.read(26);	//	reads out the user setting to control when the backlight level times out
	flowSensorEnable = eeprom.read(27);	//	reads out the user setting to determine if the flow sensor is enabled
	flowRateMax = eeprom.read(28);		//	reads out the user setting that is set for the max flow rate as an integer
	flowRateMin = eeprom.read(29);		//	reads out the user setting for the minimum flow rate

	//  READ ALARM SETTINGS FROM EEPROM AND SETUP THE ALARMS IN THE TIMEALARMS LIBRARY
	tempReadID = Alarm.timerRepeat(tempReadDelay, ReadTempSensors);		//	sets an alarm to read the temp sensors at the specified delay and returns the Alarm_ID to tempReadID
	AlarmEnable = eeprom.read(100);		//	reads out the byte for the enable flags for all 8 alarms
	AlarmState = eeprom.read(101);		//	reads out the byte for the state flags for all 8 alarms
	RelayState = eeprom.read(150);

	if ((serialDebug & 4) == 4) {
		Serial.println();
		Serial.println("ALARM EEPROM SETTINGS");
		Serial.println("ID ON  OFF En  Typ  ON     OFF   Relay");
	}

	for (uint8_t id = 0; id <= relayCount; id++) {		//  read each of the alarms values out of the EEPROM
		if ((serialDebug & 8) == 8) { serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop			
		AlarmType[id] = eeprom.read(102 + (id * 6));
		AlarmRelay[id] = eeprom.read(103 + (id * 6));
		AlarmHourOn[id] = eeprom.read(104 + (id * 6));
		AlarmMinOn[id] = eeprom.read(105 + (id * 6));
		AlarmHourOff[id] = eeprom.read(106 + (id * 6));
		AlarmMinOff[id] = eeprom.read(107 + (id * 6));
		AlarmIDOn[id] = Alarm.alarmRepeat(AlarmHourOn[id], AlarmMinOn[id], 0, AlarmON);
		AlarmIDOff[id] = Alarm.alarmRepeat(AlarmHourOff[id], AlarmMinOff[id], 30, AlarmOFF);

		if ((serialDebug & 4) == 4) {
			//			   ID   ON   OFF
			Serial.printf("%d  %-2d  %-2d  ", id, AlarmIDOn[id], AlarmIDOff[id]);
			if ((AlarmEnable & (1 << id)) == (1 << id)) { Serial.print("ON "); }
			else { Serial.print("OFF"); }
			//			   TYP    ON        OFF
			Serial.printf("  %d  %2d:%02d  %2d:%02d  ", AlarmType[id], AlarmHourOn[id], AlarmMinOn[id], AlarmHourOff[id], AlarmMinOff[id]);
			Serial.println(AlarmRelay[id], BIN);
		}
	}

	flowReadID = Alarm.timerRepeat(flowReadDelay, FlowSensorRead);		//	sets an alarm to read the flow sensor at the specified dalay and returns the Alarm_IS to flowReadID
	if (flowSensorEnable == 0) {
		Alarm.disable(flowReadID);
		Serial.println("Flow Readings Disabled");
	}
	else {
		Alarm.enable(flowReadID);
		Serial.println("Flow Readings Enabled");
	}

	serialDebug = eeprom.read(5);		//	read out the serial debug again in case it was disable during the alarm print

	if ((serialDebug & 4) == 4) {
		//uint8_t rd;
		Serial.println();
		Serial.print("AlarmEnable ");
		Serial.println(AlarmEnable, BIN);
		Serial.print("AlarmState ");
		Serial.println(AlarmState, BIN);
		Serial.print("RelayState ");
		Serial.println(RelayState, BIN);
		Serial.printf("# of Alarms %d\n", Alarm.count());
		Serial.printf("tempReadID %d, %d\n", tempReadID, Alarm.read(tempReadID));
		Serial.printf("flowReadID %d, %d\n", flowReadID, Alarm.read(flowReadID));
		Serial.println();
	}
}

void factoryDefaultset()
{
	Serial.println("Writing Factory Defaults to EEPROM");

	//  Non User Settings
	eeprom.write(0, 1);			//	writes the e2Empty value = 1 or Set
	eeprom.write(1, 1);			//  determines if this is the first run or not
	eeprom.write(2, 14);		//  writes the build year
	eeprom.write(3, 10);		//  writes the build month
	eeprom.write(4, 12);		//  writes the build day
	eeprom.write(5, 76);		//	writes the serialDebug value = 1 or OFF

	//  User Settings
	eeprom.write(20, 1);		//  writes the tempType value = 1 or Fahrenheit
	eeprom.write(21, 1);		//	writes the tempPrecision value = 0 or No Decimal
	eeprom.write(22, 10);		//	writes the tempReadDelay value = 10 or 10 Seconds
	eeprom.write(23, 1);		//	writes the timeFormat value =  1 or 12 hour
	eeprom.write(24, 1);		//	writes the secondsDisplay value to 1 or Display seconds
	eeprom.write(25, 128);		//	writes the backlightLevel value = 128 or half
	eeprom.write(26, 30);		//  writes the backlightTimeout to be 30 seconds
	eeprom.write(27, 0);		//	writes the flowSensorEnable to be disabled
	eeprom.write(28, 55);		//	writes the 1st bit of the flow sensor 100% value to 0
	eeprom.write(29, 75);		//  writes the flow sensor user level to 75 or 75% 
	eeprom.write(30, 0);
	eeprom.write(32, 0);

	//  Alarm Settings
	eeprom.write(100, 255);	//  writes alarms enable flag to off
	eeprom.write(101, 0);	//  writes the alarm state flag to 0 or Off
	eeprom.write(150, 0);	//	writes the relayState flag to all 0's or OFF

	for (int i = 0; i <= relayCount; i++)	//	loop through all 8 alarms
	{
		eeprom.write(102 + (i * 6), 0);			//  writes the alarm type to 0, Day Lights
		int bit = 1 << i;
		eeprom.write(103 + (i * 6), (0 ^ bit));	//	writes the relay trigger to relay to match the id
		eeprom.write(104 + (i * 6), 0);			//  writes the alarm on hour 12
		eeprom.write(105 + (i * 6), 28+i);		//  writes the alarm on minute 1
		eeprom.write(106 + (i * 6), 00);		//  writes the alarm off hour 23
		eeprom.write(107 + (i * 6), 32+i);		//  writes the alarm off minute 11
	}
	Serial.println("Factory Defaults Restored");
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
	mcpA.writeBit(SD_LED_BANK, RED_LED_PIN, 1);
}

void TestSDLEDS()
{
	//TEST THE LEDS
	mcpA.writeBit(SD_LED_BANK, RED_LED_PIN, 1);
	delay(500);
	mcpA.writeBit(SD_LED_BANK, GREEN_LED_PIN, 1);
	delay(500);
	mcpA.writeBit(SD_LED_BANK, BLUE_LED_PIN, 1);
	delay(500);
	mcpA.writeBit(SD_LED_BANK, RED_LED_PIN, 0);
	delay(500);
	mcpA.writeBit(SD_LED_BANK, GREEN_LED_PIN, 0);
	delay(500);
	mcpA.writeBit(SD_LED_BANK, BLUE_LED_PIN, 0);
	delay(500);
}
