#include <Time.h>               //Time Library
#include <TimeAlarms.h>			//TimeAlarms Library
#include <DS1307RTC.h>          //DS RTC library
#include <OneWire.h>            //OneWire Library for the DS Sensors
#include <DallasTemperature.h>  //Dallas Temperature library
#include <LiquidCrystal_I2C.h>  //LCD I2C library
#include <Wire.h>               //I2C Wire library


//  CONFIGURATION SETUP
//  ***********************************************

int Serial_Debug = 1;		//  Set to 1 to enable serial debugging
int Sample_Number = 5;		//  Set the number of samples to be taken from each sensor
int Sample_Delay = 500;		//  Sets the delay between the samples taken
int Temp_Type = 1;			//  Selects between 0 = Celsius or 1 = Fahrenheit
float Version = 0.01;		//  Sets the version number for the build


//  INITIALIZE THE LCD
//  ***********************************************

#define I2C_ADDR 0x3F  //  Set the address of the LCD screen

// Define the LCD Pins for the I2C
#define B_Light 3
#define En 2
#define Rw 1
#define Rs 0
#define D4 4
#define D5 5
#define D6 6
#define D7 7

LiquidCrystal_I2C lcd(I2C_ADDR,En,Rw,Rs,D4,D5,D6,D7);  // Pass the lcd pins for the LiquidCrystal_I2C library to use

byte degree[8] = {B01100,B10010,B10010,B01100,B00000,B00000,B00000,};  //  set the lcd char for the degree symbol
	

//  INITIALIZE THE DS18B20 TEMPERATURE SENSORS
//  ***********************************************

// define the DS18B20 global variables
const int ONE_WIRE_BUS[]={2};		// the array to define which pins you will use for the busses ie {2,3,4,5};
#define TEMPERATURE_PRECISION 9		// temperature precision 9-12 bits
#define NUMBER_OF_BUS 1				// how many busses will you use for the sensors
#define POLL_TIME 200				//how long the avr will poll each sensor

OneWire *oneWire[NUMBER_OF_BUS];  //  Setup oneWire instances to communicate with any OneWire Devices

DallasTemperature *sensors[NUMBER_OF_BUS];  // Pass onewire reference to Dallas temperatures.

DeviceAddress tempDeviceAddress[8];  //  arrays to hold device addresses

int numberOfDevices[NUMBER_OF_BUS];  //  define the variable to store the number of busses

int RTC_Status=1;


//  INITIALIZE THE RELAYS
//  ***********************************************

int Relay_Pins[] = {31, 33, 35, 37, 39, 41, 43, 45};	//  Initailize the relay pins
int Relay_Count = 8;	//  Set the number of relays


//  VOID SETUP
//  ***********************************************
void setup(void)
{
	for(int relay = 0; relay < Relay_Count; relay++){pinMode(Relay_Pins[relay], OUTPUT);}
	for(int relay = 0; relay < Relay_Count; relay++){digitalWrite(Relay_Pins[relay], HIGH);}

	lcd.begin(20,4);  //  setup the LCD's number of columns and rows
	
	lcd.createChar(1, degree);				//  init custom characters as numbers
	lcd.setBacklightPin(B_Light,POSITIVE);  //  set the backlight pin and polarity
	lcd.setBacklight(HIGH);					//  toggle the backlight on
	
	START_SCREEN();		//  call the start up screen function


	if (Serial_Debug == 1) Serial.begin(9600);  //  start the serial port if debugging is on
	while (!Serial) ;							//  wait untill the serial monitor opens
		
	setSyncProvider(RTC.get);		//  this function get the time from the RTC
	if (timeStatus()!=timeSet){
		RTC_Status = 0;
		Serial.println("Unable to get the RTC");}
	else{Serial.println("RTC has set the system time");}

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
		delay(500);
	}
}

//  VOID LOOP
//  ***********************************************
void loop()
{
	if (RTC_Status==1){LCD_Time_Display();}
	delay(1000);
	if (RTC_Status==1){Display_Date();}

	DS18B20_Read();	  //  call the function to read DS temp sensors
	delay(1000);
	
//	RL_Toggle();
	
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

//  SERIAL PRINT DS18B20 ADDRESS FUNCTION
//  ***********************************************rt
void printAddress(DeviceAddress deviceAddress)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		// zero pad the address if necessary
		if (deviceAddress[i] < 16) Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}

//  THE START UP SCREEN FOR THE DISPLAY
//  ***********************************************
void START_SCREEN()
{
	lcd.setCursor(2,0);
	lcd.print("GLITCHSNIFFER'S");
	lcd.setCursor(6,1);
	lcd.print("AQUARIUM");
	lcd.setCursor(5,2);
	lcd.print("CONTROLLER");
	lcd.setCursor(4,3);
	lcd.print("VERSION ");
	lcd.print(Version);
	delay(5000);
	lcd.setCursor(0,0);
	lcd.clear();
}
	
//  LCD TIME DISPLAY FUNCTION
//  ***********************************************
void LCD_Time_Display()
{
	Serial.print(timeStatus());
	Serial.println(second());
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
	lcd.setCursor(0,0);
	lcd.print("           ");
	lcd.setCursor(1,0);
	lcd.print(month());
	lcd.print("/");
	lcd.print(day());
	lcd.print("/");
	lcd.print(year());
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
			
//			if(j==0){
//				c=(j+1);
				Serial.print(j);
				Serial.print(" X ");
//				}
//			else{
//				c=(j+1);
//				lcd.setCursor(13,j);
//				Serial.print(j);
//				Serial.print(" ");}
			lcd.setCursor(13,j);
			lcd.print("S");  //  Clear the display to prevent extra characters from interfering when F rolls from 3 to 2 digits
			lcd.print(j+1);
			lcd.print(("   "));
			
			if (tempC >= 100 || tempF >= 100){lcd.setCursor(15,j);} 
			else{lcd.setCursor(16,j);}
				
			if(Temp_Type == 0)
			{
				lcd.print(tempC);
				lcd.write(byte(1));
				lcd.print("C");
			}
			else
			{
				lcd.print(tempF);
				lcd.write(byte(1));
				lcd.print("F");
			}

			Serial.print(tempC);                    //print temperature
			Serial.print(" ");
			Serial.print(tempF);
			Serial.println("");
		}
	}
	Serial.println();
	delay(POLL_TIME);
}









