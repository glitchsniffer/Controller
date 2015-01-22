void AlarmSet(byte id)
{
	int t = 0;
	if (timeFormat == 0){ t = 3; }

	int rdon;
	rdon = Alarm.read(AlarmIDOn[id]);
	Serial.print("Read On Time from Alarmlib = ");
	Serial.println(rdon);

	int rdoff;
	rdoff = Alarm.read(AlarmIDOff[id]);
	Serial.print("Read Off Time from Alarmlib = ");
	Serial.println(rdoff);
	Serial.println();

	int trigger;
	// trigger = Alarm.getNextTrigger();
	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger before changing anything = ");
	Serial.println(trigger);
	Serial.println();

	//  set the LCD screen up for the Hour ON edit
	lcd.clear();
	lcd.setCursor(1, 0);
	lcd.print("Timer ");
	lcd.print(id);
	lcd.print(" is ");
	//	check to see if the alarm is enabled or disabled
	if ((AlarmEnable & (1 << id)) == (1 << id)){ lcd.print("Enabled"); }
	else{ lcd.print("Disabled"); }

	lcd.setCursor(0, 2);
	lcd.print("Set");
	if (timeFormat == 0){ lcd.setCursor(7,2); }
	else { lcd.setCursor(6, 2); }
	lcd.write(byte(3));
	lcd.write(byte(3));
	
	if (timeFormat == 0){ AlarmLCDTime(8, 1, AlarmHourOn[id], AlarmMinOn[id]); }
	else { AlarmLCDTime(7, 1, AlarmHourOn[id], AlarmMinOn[id]); }
	//
	//

	//  call the number selection menu to select the hour
	MenuNumSel(104, AlarmHourOn[id], 0, 23, 1, 7, 1, 220);
	AlarmHourOn[id] = readEEPROM(104);

	/*

	//  set the LCD screen up for the Hour OFF edit
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("    Timer 1 Edit");
	lcd.setCursor(0, 1);
	lcd.print("Set Timer 1 Hour OFF");
	MenuNumSel(105, AlarmHourOff_0, 0, 24, 1, 0, 0, 220);	//  call the number selection menu
	AlarmHourOff_0 = readEEPROM(106);


	//  set the LCD screen up for the Minute OFF edit
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("    Timer 1 Edit");
	lcd.setCursor(0, 1);
	lcd.print(" Set Timer 1 Min OFF");
	MenuNumSel(106, AlarmMinOff_0, 0, 59, 1, 0, 0, 220);
	AlarmMinOff_0 = readEEPROM(107);

						//  read the new alarm times from the EEPROM

	AlarmHourOn_0 = readEEPROM(103);	//  reads out alarm setting for the hour on
	AlarmMinOn_0 = readEEPROM(104);	//  reads out alarm setting for the mins off
	AlarmHourOff_0 = readEEPROM(105); //  reads out alarm setting for the hour off
	AlarmMinOff_0 = readEEPROM(106);	//  reads out alarm setting for the mins off
	*/
	//  write the alarms to the TimeAlarms library
	time_t setAlarmTimeOn = AlarmHMS(AlarmHourOn_0, AlarmMinOn_0, 0);
	time_t setAlarmTimeOff = AlarmHMS(AlarmHourOff_0, AlarmMinOff_0, 30);
	time_t currentTime = AlarmHMS(hour(), minute(), second() - 2);


	//AlarmIDOn_0 = Alarm.alarmRepeat(AlarmHourOn_0, AlarmMinOn_0, 0, AlarmAON);
	//AlarmIDOff_0 = Alarm.alarmRepeat(AlarmHourOff_0, AlarmMinOff_0, 30, AlarmAOFF);


	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger Time Before writing = ");
	Serial.println(trigger);
	Serial.println();

	Serial.print("My On: ");
	Serial.print(setAlarmTimeOn);
	Alarm.free(AlarmIDOn_0);
	AlarmIDOn_0 = Alarm.alarmRepeat(AlarmHourOn_0, AlarmMinOn_0, 0, AlarmAON);

	rdon = Alarm.read(AlarmIDOn_0);
	Serial.print(" - Set On: ");
	Serial.println(rdon);
	Serial.println();

	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger After Freeing and Enabling= ");
	Serial.println(trigger);

	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger Time After On write = ");
	Serial.println(trigger);
	Serial.println();

	Serial.println();
	Serial.print("My Off: ");
	Serial.print(setAlarmTimeOff);
	Alarm.disable(AlarmIDOff_0);
	Alarm.write(AlarmIDOff_0, setAlarmTimeOff);
	Alarm.enable(AlarmIDOff_0);

	rdoff = Alarm.read(AlarmIDOff_0);
	Serial.print(" - Set Off: ");
	Serial.println(rdoff);

	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger Time After Off write = ");
	Serial.println(trigger);

}

void AlarmSetDisplay(int id)
{
	int t = 0;			//  time format modifier for 12 or 24 hour clocks

	//	read all variables for the timer id from the EEPROM
	AlarmEnable = readEEPROM(100);
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
	//	if the minutes is 2 digits pad a 0 to the single digit
	if (min < 10){ lcd.print("0"); }
	lcd.print(min);


}