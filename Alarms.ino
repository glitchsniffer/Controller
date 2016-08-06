void AlarmSet(byte id)
{
	if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop
	uint8_t t = 0;		//	variable for adding the time format to the cursor
	uint16_t rd;		//  variable for reading out variables from the EEPROM
	byte bit;			//	variable for adding the bit to the id

	if (timeFormat == 0){ t = 1; }		//  add space if 24 hour time

	if ((serialDebug & 4) == 4)	{ Serial.printf("\nSetting Alarm ID %d\n", id);	}	//	print the ID to the serial port

	//  CHANGE THE HOUR ON TIME
	//  ***********************************************

	//  set the LCD screen up for the Hour ON edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.printf("Timer %d - On Time", id + 1);
	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Hour");
	PrintArrows((6 + t), 2);

	//  print the time using t as the timeFormat variable
	timestr = TimeString(1, AlarmHourOn[id], AlarmMinOn[id], 0);
	PrintTimeDisplay(timestr, (5+t), 1, 10);

	//  call the number selection menu to select the hour
	delay(100);		//	small delay to keep from carying over the previous button push
	to = CharMenuNumSel(65, (104 + (id * 6)), AlarmHourOn[id], 0, 23, 1, (6 + t), 1, 250);
	if (to == 32767){ return; }
	AlarmHourOn[id] = eeprom.read(104 + (id * 6));

	//  CHANGE THE MINUTE ON TIME
	//  ***********************************************

	//  set the LCD screen up for the Minute On edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.printf("Timer %d - On Time", id + 1);
	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Minute");
	PrintArrows((9 + t), 2);

	//  print the time using t as the timeFormat variable
	timestr = TimeString(1, AlarmHourOn[id], AlarmMinOn[id], 0);
	PrintTimeDisplay(timestr, (5 + t), 1, 10);

	//  call the number selection menu to select the hour
	to = CharMenuNumSel(64, (105 + (id * 6)), AlarmMinOn[id], 0, 59, 1, (9 + t), 1, 175);
	if (to == 32767){ return; }
	AlarmMinOn[id] = eeprom.read(105 + (id * 6));

	//  CHANGE THE HOUR OFF TIME
	//  ***********************************************

	//  set the LCD screen up for the Hour OFF edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.printf("Timer %d - Off Time", id + 1);
	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Hour");
	PrintArrows((6 + t), 2);

	//  print the time using t as the timeFormat variable
	timestr = TimeString(1, AlarmHourOff[id], AlarmMinOff[id], 0);
	PrintTimeDisplay(timestr, (5 + t), 1, 10);

	//  call the number selection menu to select the hour
	to = CharMenuNumSel(65, (106 + (id * 6)), AlarmHourOff[id], 0, 23, 1, (6 + t), 1, 250);
	if (to == 32767){ return; }
	AlarmHourOff[id] = eeprom.read(106 + (id * 6));

	//  CHANGE THE MINUTE OFF TIME
	//  ***********************************************

	//  set the LCD screen up for the Minute OFF edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.printf("Timer %d - Off Time", id + 1);
	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Minute");
	PrintArrows((9 + t), 2);

	//  print the time using t as the timeFormat variable
	timestr = TimeString(1, AlarmHourOff[id], AlarmMinOff[id], 0);
	PrintTimeDisplay(timestr, (5 + t), 1, 10);

	//  call the number selection menu to select the hour
	to = CharMenuNumSel(64, (107 + (id * 6)), AlarmMinOff[id], 0, 59, 1, (9 + t), 1, 175);
	if (to == 32767){ return; }
	AlarmMinOff[id] = eeprom.read(107 + (id * 6));

	//  WRITE THE ALARMS TO THE TIMEALARMS LIBRARY
	//  ***********************************************
	//	you must free the alarm and rewrite it using the same alarm ID in order for the next trigger time to update

	//	free and write the alarm ON time to the TimeAlarmLibrary
	Alarm.free(AlarmIDOn[id]);
	AlarmIDOn[id] = Alarm.alarmRepeat(AlarmHourOn[id], AlarmMinOn[id], 0, AlarmON);

	//	free and write the alarm OFF Time to the TimeAlarmLibrary
	Alarm.free(AlarmIDOff[id]);
	AlarmIDOff[id] = Alarm.alarmRepeat(AlarmHourOff[id], AlarmMinOff[id], 30, AlarmOFF);

	//	CHANGE THE RELAYS ATTACHED TO THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the Hour OFF edit
	lcd.clear();
	lcd.setCursor(2, 0);
	lcd.printf("Timer %d - Relays", id + 1);
	lcd.setCursor(0, 2);
	lcd.print("Set");
	lcd.setCursor(0, 3);
	lcd.print("Relays");
	lcd.setCursor(9, 2);
	lcd.print("12345678");
	RelayState = AlarmRelay[id];
	RelayStatusDisplay(9, 1);
	RelayState = eeprom.read(150);
		
	uint8_t relaytemp = AlarmRelay[id];

	for (uint8_t i = 0; i < 8; i++)	{
		uint8_t start;
		//	Check to see if if the timer is enable or disabled
		if ((AlarmRelay[id] & (1 << i)) == (1 << i)){ start = 1; }
		else{ start = 0; }

		//  call the number selection menu to select the relay
		rd = CharMenuNumSel(200, 255, start, 0, 1, 1, (10 + i), 1, 200);
		if (rd == 32767){ return; }

		bit = 1 << i;

		if ((serialDebug & 4) == 4)	{ Serial.printf("Relay %d : En %d; Byte ", i, rd); }

		if (rd != start){ relaytemp = relaytemp ^ bit; }

		Serial.println(relaytemp, BIN);
	}
	eeprom.write(103 + (id * 6), relaytemp);
	AlarmRelay[id] = eeprom.read(103 + (id * 6));
	
	if ((serialDebug & 4) == 4)	{
		Serial.print("AlarmRelay ");
		Serial.println(AlarmRelay[id], BIN);
	}

	//	SET THE TYPE OF RELAY FOR THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the enable/disable setting
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.printf("Timer %d - Relay Type", id + 1);
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
	to = CharMenuNumSel(64, (102 + (id * 6)), AlarmType[id], 0, 3, 1, 8, 1, 250);
	if (to == 32767){ return; }
	AlarmType[id] = eeprom.read(102 + (id * 6));

	if ((serialDebug & 4) == 4) { Serial.printf("AlarmType %d", AlarmType[id]);	}

	//	SET THE ENABLE/DISABLE FLAG FOR THIS ALARM
	//  ***********************************************

	//  set the LCD screen up for the enable/disable setting
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.printf(" Timer %d is ", id + 1);

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
	uint8_t start;
	if ((AlarmEnable & (1 << id)) == (1 << id)){ start = 1; }		//	Check to see if if the timer is enable or disabled
	else{ start = 0; }

	rd = CharMenuNumSel(4, 255, start, 0, 1, 1, 8, 1, 250);	//	Call the function to edit the variable
	if (rd == 32767){ return; }

	bit = 1 << id;
	
	//	enable or disable the alarms in the timeAlarms lib
	if (rd == 0) {
		Alarm.disable(AlarmIDOn[id]);
		Alarm.disable(AlarmIDOff[id]);
	}
	else if (rd == 1) {
		Alarm.enable(AlarmIDOn[id]);
		Alarm.enable(AlarmIDOff[id]);
	}

	//	print the status to the serial port
	if ((serialDebug & 4) == 4)	{
		Serial.print("AlarmEnable ");
		Serial.println(AlarmEnable, BIN);
		Serial.println();
	}

	serialDebug = eeprom.read(5);		//	read out the serial debug againg in case it was disable during the alarm print
}

void AlarmSetDisplay(uint8_t id)
{
	uint8_t t = 0;			//  time format modifier for 12 or 24 hour clocks

	//	read all variables for the timer id from the EEPROM
	if ((serialDebug & 8) == 8){ serialDebug = serialDebug - 8; }	//	Supress the EEPROM serial prints during this loop
	AlarmEnable = eeprom.read(100);
	AlarmState = eeprom.read(101);
	RelayState = eeprom.read(150);
	AlarmType[id] = eeprom.read(102 + (id * 6));
	AlarmRelay[id] = eeprom.read(103 + (id * 6));
	AlarmHourOn[id] = eeprom.read(104 + (id * 6));
	AlarmMinOn[id] = eeprom.read(105 + (id * 6));
	AlarmHourOff[id] = eeprom.read(106 + (id * 6));
	AlarmMinOff[id] = eeprom.read(107 + (id * 6));
	
	lcd.printf(" Timer %d is ", id + 1);

	//	check to see if the alarm is enabled or disabled
	if ((AlarmEnable & (1 << id)) == (1 << id)){ lcd.print("Enabled"); }
	else{ lcd.print("Disabled"); }

	//	check the timeFormat and if it is 24 hour add 3 to the cursor postion
	if (timeFormat == 0){ t = 3; }

	// set cursor and print ON-
	lcd.setCursor(9 + t, 1);
	lcd.print("On ");

	//	print the time on the display using 24 hour or 12 hour
	timestr = TimeString(1, AlarmHourOn[id], AlarmMinOn[id], 0);
	if (timeFormat == 0) { PrintTimeDisplay(timestr, 14, 1, 10); }	//	24 hour
	else { PrintTimeDisplay(timestr, 12, 1, 10); }	//	12 hour
	
	//	Set cursor and print OFF-
	lcd.setCursor(8 + t, 2);
	lcd.print("Off ");

	//	print the time on the display using 24 hour or 12 hour
	timestr = TimeString(1, AlarmHourOn[id], AlarmMinOn[id], 0);
	if (timeFormat == 0) { PrintTimeDisplay(timestr, 14, 2, 10); }	//	24 hour
	else { PrintTimeDisplay(timestr, 12, 2, 10); }	//	12 hour

	//	set cursor and print the relay number that the alarm is set to trigger
	lcd.setCursor(6, 3);
	lcd.print("Relay");
	miMax = 1;
	serialDebug = eeprom.read(5);		//	read out the serial debug againg in case it was disable during the alarm print
	
	//	display the relays that are currently enabled
	RelayState = AlarmRelay[id];		//  Temporarily set RelayState to AlarmRelay[id]
	RelayStatusDisplay(12, 3);			//  Display the relays triggered by the alarm
	RelayState = eeprom.read(150);		//	reset RelayState to the original value from the EEPROM
}