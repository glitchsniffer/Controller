void AlarmSet(byte id)
{
	if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop
	int t = 0;		//	variable for adding the time format to the cursor
	int rd;			//  variable for reading out variables from the EEPROM

	if (timeFormat == 0){ t = 1; }		//  variable for adding the timeFormat to the cursor

	//	read and print the next trigger time for AlarmIDOn[id]
	rd = Alarm.getNextTrigger();

	if ((serialDebug & 4) == 4)
	{
		Serial.println();
		Serial.print("Setting Alarm ID: ");
		Serial.println(id);
		Serial.println("*****************************************");
		Serial.print("Next trigger before changing anything = ");
		Serial.println(rd);
	}

	//  CHANGE THE HOUR ON TIME
	//  ***********************************************

	//  set the LCD screen up for the Hour ON edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - On Time");

	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Hour");

	lcd.setCursor((6 + t), 2);
	lcd.write(byte(3));		//  print the up arrow
	lcd.write(byte(3));		//  print the up arrow
	
	//  print the time using t as the timeFormat variable
	LCDTimeDisplay((6 + t), 1, AlarmHourOn[id], AlarmMinOn[id], 99, 0);

	//  call the number selection menu to select the hour
	MenuNumSel(65, (104 + (id * 6)), AlarmHourOn[id], 0, 23, 1, (6 + t), 1, 250);
	AlarmHourOn[id] = readEEPROM(104 + (id * 6));

	//  CHANGE THE MINUTE ON TIME
	//  ***********************************************
	//  set the LCD screen up for the Minute On edit

	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - On Time");

	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Minute");

	lcd.setCursor((9 + t), 2);
	lcd.write(byte(3));		//  print the up arrow
	lcd.write(byte(3));		//  print the up arrow

	//  print the time using t as the timeFormat variable
	LCDTimeDisplay((6 + t), 1, AlarmHourOn[id], AlarmMinOn[id], 99, 0);

	//  call the number selection menu to select the hour
	MenuNumSel(64, (105 + (id * 6)), AlarmMinOn[id], 0, 59, 1, (9 + t), 1, 175);
	AlarmMinOn[id] = readEEPROM(105 + (id * 6));

	//  CHANGE THE HOUR OFF TIME
	//  ***********************************************

	//  set the LCD screen up for the Hour OFF edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - Off Time");

	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Hour");

	lcd.setCursor((6 + t), 2);
	lcd.write(byte(3));		//  print the up arrow
	lcd.write(byte(3));		//  print the up arrow

	//  print the time using t as the timeFormat variable
	LCDTimeDisplay((6 + t), 1, AlarmHourOff[id], AlarmMinOff[id], 99, 0);

	//  call the number selection menu to select the hour
	MenuNumSel(65, (106 + (id * 6)), AlarmHourOff[id], 0, 23, 1, (6 + t), 1, 250);
	AlarmHourOff[id] = readEEPROM(106 + (id * 6));

	//  CHANGE THE MINUTE OFF TIME
	//  ***********************************************

	//  set the LCD screen up for the Minute OFF edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - Off Time");

	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Minute");

	lcd.setCursor((9 + t), 2);
	lcd.write(byte(3));		//  print the up arrow
	lcd.write(byte(3));		//  print the up arrow

	//  print the time using t as the timeFormat variable
	LCDTimeDisplay((6 + t), 1, AlarmHourOff[id], AlarmMinOff[id], 99, 0);

	//  call the number selection menu to select the hour
	MenuNumSel(64, (107 + (id * 6)), AlarmMinOff[id], 0, 59, 1, (9 + t), 1, 250);
	AlarmMinOff[id] = readEEPROM(107 + (id * 6));

	//  WRITE THE ALARMS TO THE TIMEALARMS LIBRARY
	//  ***********************************************

	//	free and write the alarm ON time to the TimeAlarmLibrary
	//	i found that you must free the alarm and rewrite it using the same alarm ID in order for the next trigger time to update
	Alarm.free(AlarmIDOn[id]);
	AlarmIDOn[id] = Alarm.alarmRepeat(AlarmHourOn[id], AlarmMinOn[id], 0, AlarmON);

	//	free and write the alarm OFF Time to the TimeAlarmLibrary
	//	i found that you must free the alarm and rewrite it using the same alarm ID in order for the next trigger time to update
	Alarm.free(AlarmIDOff[id]);
	AlarmIDOff[id] = Alarm.alarmRepeat(AlarmHourOff[id], AlarmMinOff[id], 30, AlarmOFF);

	if ((serialDebug & 4) == 4)
	{
		rd = Alarm.getNextTrigger();
		Serial.print("Next trigger Time After write = ");
		Serial.println(rd);
	}

	//	CHANGE THE RELAY ATTACHED TO THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the Hour OFF edit
	lcd.clear();
	lcd.setCursor(2, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - Relay");

	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Relay");

	lcd.setCursor(9, 2);
	lcd.write(byte(3));		//  print the up arrow

	////  print the current relay setting
	//lcd.setCursor(9, 1);
	//lcd.print(AlarmRelay[id]);

	//  call the number selection menu to select the relay
	MenuNumSel(64, (103 + (id * 6)), AlarmRelay[id], 0, 7, 1, 8, 1, 250);
	AlarmRelay[id] = readEEPROM(103 + (id * 6));
	
	if ((serialDebug & 4) == 4)
	{
		Serial.print("Relay set for ");
		Serial.println(AlarmRelay[id]);
	}

	//	SET THE TYPE OF RELAY FOR THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the enable/disable setting
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - Relay Type");

	lcd.setCursor(0, 2);
	lcd.print("0=Day");
	lcd.setCursor(13, 2);
	lcd.print("2=CO2");
	lcd.setCursor(0, 3);
	lcd.print("1=Night");
	lcd.setCursor(13, 3);
	lcd.print("3=Other");

	lcd.setCursor(9, 2);
	lcd.write(byte(3));		//  print the up arrow

	//	call the numselmenu to select the type of the relay  1-4
	MenuNumSel(64, (102 + (id * 6)), AlarmType[id], 0, 3, 1, 8, 1, 250);
	AlarmType[id] = readEEPROM(102 + (id * 6));

	if ((serialDebug & 4) == 4)
	{
		Serial.print("Type set for: ");
		Serial.println(AlarmType[id]);
	}

	//	SET THE ENABLE/DISABLE FLAG FOR THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the enable/disable setting
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print(" Timer ");
	lcd.print(id);
	lcd.print(" is ");
	//	check to see if the alarm is enabled or disabled
	if ((AlarmEnable & (1 << id)) == (1 << id)){ lcd.print("Enabled"); }
	else{ lcd.print("Disabled"); }
	
	lcd.setCursor(0, 2);
	lcd.print("Enable");
	lcd.setCursor(0, 3);
	lcd.print("Timer");

	lcd.setCursor(9, 2);
	lcd.write(byte(3));		//  print the up arrow

	//  call the number selection menu to select enabled or disabled
	if ((serialDebug & 4) == 4)
	{
		Serial.print("AlarmEnable: ");
		Serial.print(AlarmEnable, BIN);
		Serial.println(" - Original");
	}

	int start;
	if ((AlarmEnable & (1 << id)) == (1 << id)){ start = 1; }		//	Check to see if if the timer is enable or disabled
	else{ start = 0; }

	rd = MenuNumSel(4, 255, start, 0, 1, 1, 8, 1, 250);	//	Call the function to edit the variable

	byte bit;
	bit = 1 << id;

	if ((serialDebug & 4) == 4){ Serial.print("AlarmEnable: "); }

	if (start == rd)
	{
		if ((serialDebug & 4) == 4)
		{
		Serial.print(AlarmEnable, BIN);
		Serial.println(" - Unchanged");
		}
	}
	else
	{
		AlarmEnable = AlarmEnable ^ bit;
		writeEEPROM(100, AlarmEnable);
		if ((serialDebug & 4) == 4)
		{
			Serial.print(AlarmEnable, BIN);
			Serial.println(" - New");
		}
	}	
	serialDebug = readEEPROM(5);		//	read out the serial debug againg in case it was disable during the alarm print
}

void AlarmSetDisplay(int id)
{
	int t = 0;			//  time format modifier for 12 or 24 hour clocks

	//	read all variables for the timer id from the EEPROM
	if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop
		//AlarmEnable = readEEPROM(100);
		AlarmState = readEEPROM(101);
		AlarmType[id] = readEEPROM(102 + (id * 6));
		AlarmRelay[id] = readEEPROM(103 + (id * 6));
		AlarmHourOn[id] = readEEPROM(104 + (id * 6));
		AlarmMinOn[id] = readEEPROM(105 + (id * 6));
		AlarmHourOff[id] = readEEPROM(106 + (id * 6));
		AlarmMinOff[id] = readEEPROM(107 + (id * 6));
		
	lcd.print(" Timer ");
	lcd.print(id);
	lcd.print(" is ");
	//	check to see if the alarm is enabled or disabled
	if ((AlarmEnable & (1 << id)) == (1 << id)){ lcd.print("Enabled"); }
	else{ lcd.print("Disabled"); }

	//	check the timeFormat and if it is 24 hour add 3 to the cursor postion
	if (timeFormat == 0){ t = 3; }

	// set cursor and print ON-
	lcd.setCursor(9 + t, 1);
	lcd.print("On ");
	//	print the time on the display using 24 hour or 12 hour
	if (timeFormat == 0){ LCDTimeDisplay(15, 1, AlarmHourOn[id], AlarmMinOn[id], 99, 0); }		//	24 hour
	else { LCDTimeDisplay(12, 1, AlarmHourOn[id], AlarmMinOn[id], 99, 0); }						//	12 hour
	
	//	Set cursor and print OFF-
	lcd.setCursor(8 + t, 2);
	lcd.print("Off ");
	//	print the time on the display using 24 hour or 12 hour
	if (timeFormat == 0){ LCDTimeDisplay(15, 2, AlarmHourOff[id], AlarmMinOff[id], 99, 0); }		//	24 hour
	else { LCDTimeDisplay(12, 2, AlarmHourOff[id], AlarmMinOff[id], 99, 0); }					//	12 hour

	//	set cursor and print the relay number that the alarm is set to trigger
	lcd.setCursor(13, 3);
	lcd.print("Relay ");
	lcd.print(AlarmRelay[id]);
	miMax = 1;
	serialDebug = readEEPROM(5);		//	read out the serial debug againg in case it was disable during the alarm print
}