void AlarmSet(byte id)
{
	int rdon;
	rdon = Alarm.read(AlarmIDOn_0);
	Serial.print("Read On Time from Alarmlib = ");
	Serial.println(rdon);

	int rdoff;
	rdoff = Alarm.read(AlarmIDOff_0);
	Serial.print("Read Off Time from Alarmlib = ");
	Serial.println(rdoff);
	Serial.println();

	int trigger;
	trigger = Alarm.getNextTrigger();
	trigger = Alarm.getNextTrigger();
	Serial.print("Next trigger before changing anything = ");
	Serial.println(trigger);
	Serial.println();

	AlarmEnable = readEEPROM(100);
	AlarmState = readEEPROM(101);
	
	AlarmType[id] = readEEPROM(102 + (id * 6));
	AlarmRelay[id] = readEEPROM(103 + (id * 6));
	AlarmHourOn[id] = readEEPROM(104 + (id * 6));
	AlarmMinOn[id] = readEEPROM(105 + (id * 6));
	AlarmHourOff[id] = readEEPROM(106 + (id * 6));
	AlarmMinOff[id] = readEEPROM(107 + (id * 6));

	//  set the LCD screen up for the Hour ON edit
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(" Timer 1 is ");
	if ((AlarmEnable & (1 << id)) == (1 << id)){Serial.print("Enabled");}
	else{ Serial.print("Disabled"); }


	MenuNumSel(103, AlarmHourOn_0, 0, 23, 1, 0, 0, 220);	//  call the number selection menu
	AlarmHourOn_0 = readEEPROM(104);

	//  set the LCD screen up for the Minute ON edit
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("    Timer 1 Edit");
	lcd.setCursor(0, 1);
	lcd.print(" Set Timer 1 Min ON");
	MenuNumSel(104, AlarmMinOn_0, 0, 59, 1, 0, 0, 220);
	AlarmMinOn_0 = readEEPROM(105);

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

	/*					//  read the new alarm times from the EEPROM

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
	int ampm;			//  am/pm flag

	lcd.print(" Timer ");
	lcd.print(id);
	lcd.print(" is ");
	if ((AlarmEnable & (1 << id)) == (1 << id)){ lcd.print("Enabled"); }
	else{ lcd.print("Disabled"); }
	if (timeFormat == 0){ t = 3; }
	lcd.setCursor(9 + t, 1);
	lcd.print("On-");
	switch (timeFormat)			//	use timeFormat to determine where to put the cursor if set for 12 hour time
	{
	case 0:
		//	if 24 hour leave set the cursor to use 2 digits
		if (AlarmHourOn[id] < 10){ lcd.setCursor(16, 1); }
		lcd.print(AlarmHourOn[id]);
		break;
	case 1:
		//	if 12 hour and less than 10 set the cursor to account for # of hour digits
		if (AlarmHourOn[id] < 10 || (AlarmHourOn[id] - 12) > 0){ lcd.setCursor(13, 1); }	//  Set cursor for single digits
		else { lcd.setCursor(12, 1); }		//	set cursor for double digits
			if (AlarmHourOn[id] >= 13)
			{
				lcd.print(AlarmHourOn[id] - 12);
				lcd.setCursor(18, 1);
				lcd.print("PM");
			}
			else if (AlarmHourOn[id] <= 12)
			{
				lcd.print(AlarmHourOn[id]);
				lcd.setCursor(18, 1);
				lcd.print("AM");
			}
			break;
	}
	lcd.setCursor(14 + t, 1);
	lcd.print(":");
	if (AlarmMinOn[id] < 10){ lcd.print("0"); }
	lcd.print(AlarmMinOn[id]);
	lcd.setCursor(8 + t, 2);
	lcd.print("Off-");
	switch (timeFormat)			//	use timeFormat to determine where to put the cursor if set for 12 hour time
	{
	case 0:
		//	if 24 hour leave set the cursor to use 2 digits
		if (AlarmHourOff[id] < 10){ lcd.setCursor(12, 2); }
		lcd.print(AlarmHourOff[id]);
		break;
	case 1:
		//	if 12 hour and less than 10 set the cursor to account for # of hour digits
		if (AlarmHourOff[id] < 10 || (AlarmHourOff[id] - 12) > 0){ lcd.setCursor(13, 2); }
		else { lcd.setCursor(12, 2); }
		if (AlarmHourOff[id] >= 13)
		{
			lcd.print(AlarmHourOff[id] - 12);
			lcd.setCursor(18, 2);
			lcd.print("PM");
		}
		else if (AlarmHourOff[id] <= 12)
		{
			lcd.print(AlarmHourOff[id]);
			lcd.setCursor(18, 1);
			lcd.print("AM");
		}
		break;
	}
	lcd.setCursor(14 + t, 2);
	lcd.print(":");
	if (AlarmMinOff[id] < 10){ lcd.print("0"); }
	lcd.print(AlarmMinOff[id]);
	lcd.setCursor(12, 3);
	lcd.print("Relay ");
	lcd.print(AlarmRelay[id]);
	miMax = 1;
}