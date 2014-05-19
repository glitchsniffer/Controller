#include <Time.h>               //Time Library
#include <TimeAlarms.h>			//TimeAlarms Library
#include <DS1307RTC.h>          //DS RTC library
#include <OneWire.h>            //OneWire Library for the DS Sensors
#include <DallasTemperature.h>  //Dallas Temperature library
#include <LiquidCrystal_I2C.h>  //LCD I2C library
#include <Wire.h>               //I2C Wire library


//  INITIALIZE THE EEPROM AND RETRIEVE USER SETTINGS
//  ***********************************************

#define EEPROM_DEV_ADDR 0x50	//  Set the address of the EEPROM

int pointer = 20;			//  Sets the pointer location to 0 initially
byte data = 0;				//  Sets the value of data to be written to 0 initially


//  FACTORY DEFAULTS SETUP
//  ***********************************************


//  CONFIGURATION SETUP
//  ***********************************************

int Serial_Debug = 10;	
int Temp_Type = 1;			//  Selects between 0 = Celsius or 1 = Fahrenheit
int Version = 0;			//  Sets the version number for the current program
int Build = 5;				//  Sets the build number for the current program
int Temp_Read_Delay = 10;	//  Sets the amount of time between reading the temp sensors
int today = 0;				//  Sets the today to the current date to display on the RTC

//  INITIALIZE THE LCD
//  ***********************************************

//#define LCD_DEV_ADDR 0x3F	//  Set the address of the LCD screen
#define LCD_DEV_ADDR 0x27	//  Set the address of the LCD screen

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
#define UpButton 53			//  set the up button to pin 53
#define DownButton 50		//  set the down button to pin 50
#define RightButton 52		//  set the right button to pin 52
#define LeftButton 51		//  set the left button to pin 51

//#define MenuButton 19

int LEDPin = 13;			//  set the onboard led on pin 13
volatile int MenuMode = 0;	//  allow the variable state to change the led on and off

const int Button[]={42,43,44,45,46,47,48,49};	//  sets the pins for Button0 - Button 7 respectively


//  INITIALIZE THE MENU VARIABLES
//  ***********************************************
int MPoint = 0;			//  current main menu pointer position
int MCur = 1;			//  current position of the main menu cursor
int MStart = 0;			//  current starting cursor
int MLevel = 0;			//  current menu level.  main menu level=0, menu items=1, menu item selected=2
int MLevelMax = 3;
int M0Sel = 0;
int M1Sel = 0;
int M2Sel = 0;
int M3Sel = 0;
int M0Start = 0;
int M1Start = 0;		//	current menu item starting cursor
int M2Start = 0;
int M3Start = 0;
int MIMax = 0;			//  current selected menu item for purposes of up and down movement
int mRet = 0;

char* M0Items[]={"", "System Config", "Timers Setup", "Sensor Addr Config","Calibration",""};  //  setup menu items here  Min Cursor = 0 and Max Cursor = 3
	char* M1Items0[]={"", "Temp Type", "Temp Precision", "Time Format", "B Light Brightness", "Set Date/Time", "Serial Debugging", ""};  //  setup menu item 1 for System Config Min 0 Max 6
		char* M2Items00[]={"", "Celsius", "Fahrenheit", ""};
		char* M2Items01[]={"", "No Decimal", "1 Decimal", ""};
		char* M2Items02[]={"", "12 Hour", "24 Hour", ""};
		char* M2Items03[]={"", "B Light Brightness", ""};
		char* M2Items04[]={"", "Need Date/Time Here", ""};
		char* M2Items05[]={"", "On", "Off", ""};
	char* M1Items1[]={"", "Set Timer 1", "Set Timer 2", "Set Timer 3", "Set Timer 4", ""};  //  setup menu item 2 for Timer Setup Min 0 Max 3
		char* M2Items10[]={"", "S Timer 1", "Exit", ""};
		char* M2Items11[]={"", "S Timer 2", "Exit", ""};
		char* M2Items12[]={"", "Set Timer 3", "Exit", ""};
		char* M2Items13[]={"", "Set Timer 4", "Exit", ""};
	char* M1Items2[]={"", "Temp Sens 1 Add", "Temp Sens 2 Add", "Temp Sens 3 Add", "Temp Sens 4 Add", ""};  //  setup menu item 3 for Timer Setup Min 0 Max 3
		char* M2Items20[]={"", "Set Sens 1 Address", "", ""};
		char* M2Items21[]={"", "Set Sens 2 Address", "Exit", ""};
		char* M2Items22[]={"", "Set Sens 3 Address", "Exit", ""};
		char* M2Items23[]={"", "Set Sens 4 Address", "Exit", ""};
	char* M1Items3[]={"", "Temp 1 Calib", "Temp 2 Calib", "Temp 3 Calib", "Temp 4 Calib", "Flow Calib", ""};  //  setup menu item 4 for Timer Setup Min 0 Max 4
		char* M2Items30[]={"", "Calibrate Sensor 1", "Exit", ""};
		char* M2Items31[]={"", "Calibrate Sensor 2", "Exit", ""};
		char* M2Items32[]={"", "Calibrate Sensor 3", "Exit", ""};
		char* M2Items33[]={"", "Calibrate Sensor 4", "Exit", ""};
		char* M2Items34[]={"", "Calibrate Flow Sens", "Exit", ""};

//  INITIALIZE THE DS18B20 TEMPERATURE SENSORS
//  ***********************************************

// define the DS18B20 global variables
const int ONE_WIRE_BUS[]={12};		// the array to define which pins you will use for the busses ie {2,3,4,5};
#define TEMPERATURE_PRECISION 9		// temperature precision 9-12 bits
#define NUMBER_OF_BUS 1				// how many busses will you use for the sensors

OneWire *oneWire[NUMBER_OF_BUS];  //  Setup oneWire instances to communicate with any OneWire Devices

DallasTemperature *sensors[NUMBER_OF_BUS];  // Pass onewire reference to Dallas temperatures.

DeviceAddress tempDeviceAddress[8];  //  arrays to hold device addresses

int numberOfDevices[NUMBER_OF_BUS];  //  define the variable to store the number of busses

int RTC_Status=1;


//  INITIALIZE THE ALARMS
//  ***********************************************
	
int AlarmAState = 1;
int AlarmHourA_ON = 6;
int AlarmMinA_ON = 30;
int AlarmSecA_ON = 0;
int AlarmHourA_OFF = 19;
int AlarmMinA_OFF = 0;
int AlarmSecA_OFF = 0;


//  INITIALIZE THE RELAYS
//  ***********************************************

int Relay_Pins[] = {23,25,27,29,31,33,35,37};	//  Initialize the relay pins
int Relay_Count = 8;	//  Set the number of relays


//  VOID SETUP
//  ***********************************************
void setup(void)
{
	Serial_Debug = readEEPROM(5);
	
	//  SETUP THE SERIAL PORT
	if (Serial_Debug == 0){Serial.begin(115200);}  //  start the serial port if debugging is on
	
	//  SETUP THE BUTTONS
	pinMode(UpButton, INPUT);		//  sets the UpButton to an input
	pinMode(DownButton, INPUT);		//  sets the DownButton to an input
	pinMode(LeftButton, INPUT);		//  sets the LeftButton to an input
	pinMode(RightButton, INPUT);	//  sets the RightButton to an input
	
	for(int b = 0; b < 8; b++){pinMode(Button[b], INPUT);}	//  sets Button0-7 pins as inputs
		
	pinMode(LEDPin, OUTPUT);						//  configures led 13 to an output for testing the MenuButton
	attachInterrupt(4, MenuButtonPress, FALLING);		//  Attaches int.4, pin 19 and sets it to trigger on a low input from the menu button
		
	//  SETUP THE RELAYS OUTPUTS
	for(int relay = 0; relay < Relay_Count; relay++){pinMode(Relay_Pins[relay], OUTPUT);}
	for(int relay = 0; relay < Relay_Count; relay++){digitalWrite(Relay_Pins[relay], HIGH);}

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
	
	
	//  SETUP THE RTC
	setSyncProvider(RTC.get);		//  this function get the time from the RTC
	if (timeStatus()!=timeSet){
		RTC_Status = 0;
		Serial.println("Unable to get the RTC");}
	else{Serial.println("RTC has set the system time");}

	readEEPROM(6);		// read the first run data from the eeprom
	if 	(data = 0)
	{
		pointer = 6;
		data = 1;
		writeEEPROM(pointer,data);
		factoryDefaultset();
	}	
	data = 9;
	data = readEEPROM(5);
	Serial.print("Reading Serial out of EEPROM: ");
	Serial.println(data);
	delay(500);
	
	data = 9;
	data = readEEPROM(20);
	Serial.print("Reading Temp out of EEPROM: ");
	Serial.println(data);
	delay(500);
	
	//  SETUP ALARMS
	
	Alarm.timerRepeat(Temp_Read_Delay,DS18B20_Read);
	Alarm.alarmRepeat(AlarmHourA_ON,AlarmMinA_ON,AlarmSecA_ON, AlarmAON);  //  Alarm A ON Trigger
	Alarm.alarmRepeat(AlarmHourA_OFF,AlarmMinA_OFF,AlarmSecA_OFF,AlarmAOFF);  //  Alarm A OFF Trigger
	
	//  SETUP THE DS18B20 SENSORS
	//  Check to see how many sensors are on the busses
	for(int i=0;i<NUMBER_OF_BUS; i++)   //search each bus one by one
	{
		oneWire[i] = new OneWire(ONE_WIRE_BUS[i]);
		sensors[i] = new DallasTemperature(oneWire[i]);
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();
		Serial.print("Locating devices...");
		Serial.print("Found ");
		Serial.print(numberOfDevices[i], DEC);
		Serial.print(" devices on port ");
		Serial.println(ONE_WIRE_BUS[i],DEC);
		for(int j=0;j<numberOfDevices[i]; j++)
		{
			// Search the wire for address
			if(sensors[i]->getAddress(tempDeviceAddress[j], j))
			{
				Serial.print("Found device ");
				Serial.print(j, DEC);
				Serial.print(" with address: ");
				printAddress(tempDeviceAddress[j]);
				Serial.println("");

				// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
				sensors[i]->setResolution(tempDeviceAddress[j], TEMPERATURE_PRECISION);
			}
			else{
				Serial.print("Found ghost device at ");
				Serial.print(j, DEC);
				Serial.print(" but could not detect address. Check power and cabling");
			}
		}
		delay(200);
	}
	DS18B20_Read();
}

//  VOID LOOP
//  ***********************************************
void loop()
{
	if (MenuMode == 1){Serial.println("Entering Menu");
		MLevel = 0;
		MStart = 0;
		M0Sel = 0;
		M1Sel = 0;
		M2Sel = 0;
		MenuTitle();
		Serial.println("Exiting Menu");}
	if (RTC_Status==1){Display_Date();}
	if (RTC_Status==1){LCD_Time_Display();}
	if (MenuMode == 1){digitalWrite(LEDPin, HIGH);}
		else{digitalWrite(LEDPin, LOW);}
	Alarm.delay(1000);

//	RL_Toggle();
	
}

//  ALARM A ON TOGGLE
//  ***********************************************

void AlarmAON()
{
	Serial.println("Alarm: - turn lights ON");
	digitalWrite(Relay_Pins[0], LOW);
	lcd.setCursor(0,3);
	lcd.print("+");
}

//  ALARM A OFF TOGGLE
//  ***********************************************

void AlarmAOFF()
{
	Serial.println("Alarm: - turn lights OFF");
	digitalWrite(Relay_Pins[0], HIGH);
	lcd.setCursor(0,3);
	lcd.print("-");
}
	
//  RELAY TOGGLE FUNCTION
//  ***********************************************
void RL_Toggle()
{
	for (int relay = 0; relay < Relay_Count; relay++){
		digitalWrite(Relay_Pins[relay], LOW);
		lcd.setCursor(relay,3);
		lcd.print("+");
		delay(100);
//	for (int relay = 0; relay < Relay_Count; relay++){		
		digitalWrite(Relay_Pins[relay], HIGH);
		lcd.setCursor(relay,3);
		lcd.print(" ");
		delay(100);}
//	for (int relay = 0; relay < Relay_Count; relay++){
//	digitalWrite(Relay_Pins[relay], HIGH);
//	delay(100);}
}

//  THE START UP SCREEN FOR THE DISPLAY
//  ***********************************************
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
	lcd.print(Version);
	lcd.print(".");
	if (Build < 10){lcd.print("0");}
	lcd.print(Build);
	delay(1000);
	lcd.setCursor(0,0);
	lcd.clear();
}
	
//  LCD TIME DISPLAY FUNCTION
//  ***********************************************
void LCD_Time_Display()
{
//	Serial.print(timeStatus());
	Serial.println(second());
	lcd.setCursor(0,0);
	lcd.print("           ");
	//  set cursor start depending on the number of digits in the hour
	if (hourFormat12()>=10){lcd.setCursor(0,0);}
	else {lcd.setCursor(1,0);}
	lcd.print(hourFormat12());

	lcd.print(":");

	if(minute()<10){lcd.print('0');
	lcd.print(minute());}
	else{lcd.print(minute());}
	lcd.print(":");
	
	if(second()<10){lcd.print('0');
	lcd.print(second());}
	else{lcd.print(second());}

	if(isAM()==1){lcd.print("AM");}
	else{lcd.print("PM");}
}

//  LCD DISPLAY DATE FUNCTION
//  ***********************************************
void Display_Date()
{
	if(day()==today){return;}
	else{
	lcd.setCursor(0,1);
	lcd.print("           ");
	lcd.setCursor(1,1);
	lcd.print(month());
	lcd.print("/");
	lcd.print(day());
	lcd.print("/");
	lcd.print(year());
	today = day();}
}

//  READ THE DS18B20 SENSORS FUNCTION
//  ***********************************************
void DS18B20_Read()
{
	int c;
	
	//  Read the DS sensors found in void setup
	for(int i=0;i<NUMBER_OF_BUS; i++)   // poll every bus
	{
		sensors[i]->begin();
		numberOfDevices[i] = sensors[i]->getDeviceCount();
		sensors[i]->requestTemperatures();
		// print the device information
		for(int j=0;j<numberOfDevices[i]; j++)    // poll devices connect to the bus
		{
			sensors[i]->getAddress(tempDeviceAddress[j], j);
			printAddress(tempDeviceAddress[j]);      //print ID
			Serial.print(";");
			
			int tempC = sensors[i]->getTempC(tempDeviceAddress[j]);
			int tempF = sensors[i]->getTempF(tempDeviceAddress[j]);
			
			Serial.print(j);
			Serial.print(" - ");

			lcd.setCursor(13,j);	//  Set the lcd cursor depending on what sensor your are reading
			lcd.print("S");
			lcd.print(j+1);			//  print the sensor number
			lcd.print(("   "));		//  clear the display to prevent extra characters from interfering when F rolls from 3 to 2 digits
			
			//  determine if padding is needed for temps higher than 3 digits
			if (tempC >= 100 || tempF >= 100){lcd.setCursor(15,j);} 
			else{lcd.setCursor(16,j);}
			
			//  print the temp to the LCD screen in Celsius or Fahrenheit depending on what the user set in Temp_Type	
			if(Temp_Type == 0){
				lcd.print(tempC);
				lcd.write(byte(1));
				lcd.print("C");}
			else{lcd.print(tempF);
				lcd.write(byte(1));
				lcd.print("F");}
			
			//  send the temps to the serial port
			
			Serial.print(tempC);
			Serial.print("C ");
			Serial.print(tempF);
			Serial.println("F");
		}
	}
	Serial.println();
}

//  SERIAL PRINT DS18B20 ADDRESS FUNCTION
//  ***********************************************
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

//  WRITE TO THE I2C EEPROM
//  ***********************************************
void writeEEPROM(int address, byte data)
{
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((int)(address >> 8));			//  writes the first byte of the pointer address to the device
	Wire.write((int)(address & 0xFF));			//  writes the second byte of the pointer address to the device
	Wire.write(data);							//  writes the byte data to the EEPROM at the address specified above
	Wire.endTransmission();						//  stops the write communication on the I2C
	delay(10);
	if (Serial_Debug = 0){
		Serial.print("Writing to address ");
		Serial.print(address);
		Serial.print(" - ");
		Serial.println(data);}
}

//  READ FROM THE I2C EEPROM
//  ***********************************************
byte readEEPROM(int address)
{
	Wire.beginTransmission(EEPROM_DEV_ADDR);	//  starts communication of the I2C to the I2c device address
	Wire.write((int)(address >> 8));			//  writes the first byte of the pointer address to the device
	Wire.write((int)(address & 0xFF));			//  writes the second byte of the pointer address to the device
	Wire.endTransmission(); 					//  stops the write communication on the I2C
	Wire.requestFrom(EEPROM_DEV_ADDR,1);		//  gets 1 byte of data from the device
	data = Wire.read();							//  sets the value read to data
	if (Serial_Debug = 0){
		Serial.print("Reading from address ");
		Serial.print(address);
		Serial.print(" - ");
		Serial.println(data);}
	return data;								//  returns data to the previous call
}

//  FACTORY DEFAULTS SETUP
//  ***********************************************
void factoryDefaultset()
{
	//  Set the Serial_Debug default to 0, Serial debugging ON
	pointer = 5;
	data = 0;
	writeEEPROM(pointer,data);
	
	//  Set the Temp_Type default to 1, Fahrenheit
	pointer = 20;
	data = 1;
	writeEEPROM(pointer,data);
}


//  MENU BUTTON PRESS INTERRUPT SERVICE ROUTINE
//  ***********************************************
void MenuButtonPress()
{
	MenuMode = !MenuMode;
}