void AlarmSet(byte id)
{
	if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop
	int t = 0;		//	variable for adding the time format to the cursor
	int rd;

	if (timeFormat == 0){ t = 1; }		//  variable for adding the timeFormat to the cursor

	//	read and print the next trigger time for AlarmIDOn[id]
	rd = Alarm.getNextTrigger();
	if ((serialDebug & 4) == 4)
	{
		Serial.print("Next trigger before changing anything = ");
		Serial.println(rd);
		Serial.println();
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
	AlarmLCDTime((6 + t), 1, AlarmHourOn[id], AlarmMinOn[id]);

	//  call the number selection menu to select the hour
	MenuNumSel((104 + (id * 6)), AlarmHourOn[id], 0, 23, 1, (6 + t), 1, 250);
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
	AlarmLCDTime((6 + t), 1, AlarmHourOn[id], AlarmMinOn[id]);

	//  call the number selection menu to select the hour
	MenuNumSel((105 + (id * 6)), AlarmMinOn[id], 0, 59, 1, (9 + t), 1, 175);
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
	AlarmLCDTime((6 + t), 1, AlarmHourOff[id], AlarmMinOff[id]);

	//  call the number selection menu to select the hour
	MenuNumSel((106 + (id * 6)), AlarmHourOff[id], 0, 23, 1, (6 + t), 1, 250);
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
	AlarmLCDTime((6 + t), 1, AlarmHourOff[id], AlarmMinOff[id]);

	//  call the number selection menu to select the hour
	MenuNumSel((107 + (id * 6)), AlarmMinOff[id], 0, 59, 1, (9 + t), 1, 250);
	AlarmMinOff[id] = readEEPROM(107 + (id * 6));

	//  write the alarms to the TimeAlarms library
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

	//	CHANGE THE RELAY RELAY ATTACHED TO THIS ALARM
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
	MenuNumSel((103 + (id * 6)), AlarmRelay[id], 0, 7, 1, 8, 1, 250);
	AlarmRelay[id] = readEEPROM(103 + (id * 6));

	Serial.print("Alarm: ");
	Serial.print(id);
	Serial.print(" - ");
	Serial.print("Relay set for ");
	Serial.println(AlarmRelay[id]);
	Serial.println();

	//	SET THE ENABLE/DISABLE FLAG FOR THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the enable/disable setting
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" - Enabled");
	
	lcd.setCursor(0, 2);
	lcd.print("Enable");
	lcd.setCursor(0, 3);
	lcd.print("Timer");

	lcd.setCursor(9, 2);
	lcd.write(byte(3));		//  print the up arrow


	//  call the number selection menu to select enabled or disabled
	Serial.print("Alarm: ");
	Serial.println(id);
	Serial.print("AlarmEnable: ");
	Serial.print(AlarmEnable, BIN);
	Serial.println(" - Original");
	int start;
	if ((AlarmEnable & (1 << id)) == (1 << id)){ start = 1; }		//	Check to see if if the timer is enable or disabled
	else{ start = 0; }

	rd = MenuNumSel(255, start, 0, 1, 1, 8, 1, 250);	//	Call the function to edit the variable

	int bit;
	bit = 1 << id;

	if (start == rd)
	{
		Serial.print("AlarmEnable: ");
		Serial.print(AlarmEnable, BIN);
		Serial.println(" - Unchanged");
		Serial.println();
	}
	else
	{
		Serial.print("AlarmEnable: ");
		AlarmEnable = AlarmEnable ^ bit;
		Serial.print(AlarmEnable, BIN);
		Serial.println(" - New");
		writeEEPROM(100, AlarmEnable);
		Serial.println();
	}
	Serial.println();
	
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
	if (timeFormat == 0){ AlarmLCDTime(15, 1, AlarmHourOn[id], AlarmMinOn[id]); }		//	24 hour
	else { AlarmLCDTime(12, 1, AlarmHourOn[id], AlarmMinOn[id]); }						//	12 hour
	
	//	Set cursor and print OFF-
	lcd.setCursor(8 + t, 2);
	lcd.print("Off ");
	//	print the time on the display using 24 hour or 12 hour
	if (timeFormat == 0){ AlarmLCDTime(15, 2, AlarmHourOff[id], AlarmMinOff[id]); }		//	24 hour
	else { AlarmLCDTime(12, 2, AlarmHourOff[id], AlarmMinOff[id]); }					//	12 hour

	//	set cursor and print the relay number that the alarm is set to trigger
	lcd.setCursor(13, 3);
	lcd.print("Relay ");
	lcd.print(AlarmRelay[id]);
	miMax = 1;
	serialDebug = readEEPROM(5);		//	read out the serial debug againg in case it was disable during the alarm print
}

void AlarmLCDTime(int col, int line, int hour, int min)
{
	//  set the initial cursor position
	lcd.setCursor(col, line);

	switch (timeFormat)			//	use timeFormat to determine where to put the cursor if set for 12 hour time
	{
	case 0:
		//	if 24 hour leave set the cursor to use 2 digits
		if (hour < 10){ lcd.setCursor(col + 1, line); }	//  Set cursor for single digits
		lcd.print(hour);
		break;
	case 1:
		//	if 12 hour and less than 10 set the cursor to account for # of hour digits
		if (hour < 10 || (hour - 12) > 0){ lcd.setCursor(col + 1, line); }	//  Set cursor for single digits
		else { lcd.setCursor(col, line); }		//	set cursor for double digits
		//  determine weather to display AM or PM
		if (hour >= 13)
		{
			lcd.print(hour - 12);
			lcd.setCursor(col + 6, line);				//	set cursor for the AM/PM postion
			lcd.print("PM");
		}
		else if (hour <= 12)
		{
			lcd.print(hour);
			lcd.setCursor(col + 6, line);				//	set cursor for the AM/PM postion
			lcd.print("AM");
		}
		break;
	}
	lcd.setCursor(col + 2, line);		//	set cursor back to the minutes position
	lcd.print(":");
	//	if the minutes is 1 digit pad a 0 to the single digit
	if (min < 10){ lcd.print("0"); }
	lcd.print(min);


}