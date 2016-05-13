
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
	menuTimeout = 0;
	if (mLevel == 3)		//	menu printing is not necessary if its the final level so it just calls the do function
		{MenuDo();
		return;}
		
	lcd.clear();					//	clear the menu in preparation for setting the new title
	lcd.setCursor(0,0);				//	set the lcd cursor back to 0,0
	if (mLevel <= 0){mLevel = 0;}	//	keeps the menu level from going negative

//	switches necessary to print the title on the menu page and to set the max number of lines for the given menu
	switch (mLevel)
	{
		case 0:
			lcd.setCursor(5, 0);
			lcd.print("Main Menu");
			miMax = 4;
			break;
		case 1:
			switch (m1Sel)
			{
				case 0:
					lcd.setCursor(5, 0);
					lcd.print("User Setup");
					miMax = 8;
					break;
				case 1:
					lcd.setCursor(3, 0);
					lcd.print("Timer Setup");
					miMax = 7;
					break;
				case 2:
					lcd.setCursor(1, 0);
					lcd.print("Sensor Addr Setup");
					miMax = 3;
					break;
				case 3:
					lcd.setCursor(4, 0);
					lcd.print("Sensor Setup");
					miMax = 4;
					break;
				case 4:
					lcd.setCursor(4, 0);
					lcd.print("System Setup");
					miMax = 2;
					break;
			}
			break;
		case 2:
			switch (m1Sel)
			{
			case 0:
				switch (m2Sel)
				{
					case 0:
						lcd.setCursor(5, 0);
						lcd.print("Temp Type");
						miMax = 1;
						break;
					case 1:
						lcd.setCursor(3, 0);
						lcd.print("Temp Precision");
						miMax = 1;
						break;
					case 2:
						lcd.setCursor(2, 0);
						lcd.print("Temp Read Delay");
						miMax = 0;
						break;
					case 3:
						lcd.setCursor(2, 0);
						lcd.print("B Light Brightness");
						miMax = 0;
						break;
					case 4:
						lcd.setCursor(4, 0);
						lcd.print("Time Format");
						miMax = 1;
						break;
					case 5:
						lcd.setCursor(2, 0);
						lcd.print("Display Seconds");
						miMax = 1;
						break;
					case 6:
						lcd.setCursor(3, 0);
						lcd.print("Set Date/Time");
						miMax = 1;
						break;
					case 7:
						lcd.setCursor(4, 0);
						lcd.print("Flow Sensor");
						miMax = 1;
						break;
					case 8:
						lcd.print("XXXXXX");
						miMax = 1;
						break;
				}
				break;
			case 1:
				//	its going to call the same function no matter what case, so no switch is needed
				AlarmSetDisplay(m2Sel);
				break;
			case 2:
				switch (m2Sel)
				{
					case 0:
						lcd.setCursor(2, 0);
						lcd.print("Temp Sens 1 Addr");
						miMax = 1;
						break;
					case 1:
						lcd.setCursor(2, 0);
						lcd.print("Temp Sens 2 Addr");
						miMax = 1;
						break;
					case 2:
						lcd.setCursor(2, 0);
						lcd.print("Temp Sens 3 Addr");
						miMax = 1;
						break;
					case 3:
						lcd.setCursor(2, 0);
						lcd.print("Temp Sens 4 Addr");
						miMax = 1;
						break;
				}
				break;
			case 3:
				switch (m2Sel)
					{
					case 0:
						lcd.setCursor(1, 0);
						lcd.print("Temp Sens 1");
						miMax = 1;
						break;
					case 1:
						lcd.setCursor(1, 0);
						lcd.print("Temp Sens 2");
						miMax = 1;
						break;
					case 2:
						lcd.setCursor(1, 0);
						lcd.print("Temp Sens 3");
						miMax = 1;
						break;
					case 3:
						lcd.setCursor(1, 0);
						lcd.print("Temp Sens 4");
						miMax = 1;
						break;
					case 4:
						lcd.setCursor(1, 0);
						lcd.print("Flow Sensor");
						miMax = 3;
						break;
					}
					break;
			case 4:
				switch (m2Sel)
				{
					case 0:
						lcd.setCursor(2, 0);
						lcd.print("Serial Debugging");
						miMax = 8;
						break;
					case 1:
						lcd.setCursor(4, 0);
						lcd.print("Erase EEPROM");
						miMax = 1;
						break;
					case 2:
						lcd.setCursor(2, 0);
						lcd.print("Restore Defaults");
						miMax = 1;
						break;
				}
				break;
			}
			break;
	}

	//	sets the menu navigation pointers up for the line printing for statement
	switch (mLevel)
	{	
		case 0:
			if (m0Start >= miMax){m0Start = miMax;}
			if (m0Start < 0){m0Start = 0;}
			mStart = m0Start;
			break;
		case 1:
			if (m1Start >= miMax){m1Start = miMax;}
			if (m1Start < 0){m1Start = 0;}
			mStart = m1Start;
			break;
		case 2:
			if (m2Start >= miMax){m2Start = miMax;}
			if (m2Start < 0){m2Start = 0;}
			mStart = m2Start;
			break;
		case 3:
			if (m3Start >= miMax){m3Start = miMax;}
			if (m3Start < 0){m3Start = 0;}
			mStart = m3Start;
			break;
	}

	uint8_t mmax = 1;		//	sets mmax = 1 for use to only print 3 lines.
	mPoint = mStart;		//	set the starting position of the pointer
	mmax = mPoint+3;		//  sets the ending position of the pointer to only print 3 lines
	lcd.setCursor(0, 2);	//  moves the lcd cursor to line 3
	lcd.write(byte(2));		//	prints the right arrow to indicate the current menu selection
	mCur = 1;				//	sets the lcd cursor to start on line 1

	//	this for loop actually prints the lines.  it runs this loop 3 times to print each line
	for (; mPoint < mmax; mPoint++)
	{
		lcd.setCursor(1,mCur);		//	move the lcd cursor to the lines location
		
	//	determines what menu array to use when printing each line.
		switch (mLevel)
		{
			case 0:
				lcd.print(m0Items[mPoint]);				//	prints 1st level menu items
				break;
			case 1:
				switch (m1Sel)
				{
					case 0:								//  prints 2nd level System Config items
						lcd.print(m1Items0[mPoint]);
						break;
					case 1:								//	prints 2nd level Timer Setup items
						lcd.print(m1Items1[mPoint]);
						LCDTimeDisplay(1, 13, 1, AlarmHourOn[mPoint-2], AlarmMinOn[mPoint-2], 0, 0);			//	prints the alarms on time
						LCDTimeDisplay(1, 13, 2, AlarmHourOff[mPoint - 2], AlarmMinOff[mPoint - 2], 0, 0);	//	prints the alarms off time
						break;
					case 2:								//	prints 2nd level Sensor Addr Config items
						lcd.print(m1Items2[mPoint]);
						break;
					case 3:								//	prints 2nd level Calibration items
						lcd.print(m1Items3[mPoint]);
						break;
					case 4:								//	prints 2nd level System config items
						lcd.print(m1Items4[mPoint]);
				}
				break;
			case 2:
				switch (m1Sel)
				{
					case 0:										//	prints 3rd level System Config Items
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items00[mPoint]);
								lcd.setCursor(10, 1);
								if (tempType == 0){ lcd.print("   Celcius"); }
								else { lcd.print("Fahrenheit"); }
								break;
							case 1:
								lcd.print(m2Items01[mPoint]);
								lcd.setCursor(15, 1);
								if (tempPrecision == 0){ lcd.print(" None"); }
								else { lcd.print("1 Dec"); }
								break;
							case 2:
								lcd.print(m2Items02[mPoint]);
								lcd.setCursor(13, 1);
								lcd.print(tempReadDelay);
								lcd.print(" Sec");
								break;
							case 3:
								lcd.print(m2Items03[mPoint]);
								lcd.setCursor(17, 1);
								lcd.print(backlightLevel);
								break;
							case 4:
								lcd.print(m2Items04[mPoint]);
								lcd.setCursor(13, 1);
								if (timeFormat == 0){ lcd.print("24 Hour"); }
								else { lcd.print("12 Hour"); }
								break;
							case 5:
								lcd.print(m2Items05[mPoint]);
								lcd.setCursor(17, 1);
								if (secondsDisplay == 0){ lcd.print(" No"); }
								else { lcd.print("Yes"); }
								break;
							case 6:
								lcd.print(m2Items06[mPoint]);
								break;
							case 7:
								lcd.print(m2Items07[mPoint]);
								lcd.setCursor(12, 1);
								if (flowSensorEnable == 0){ lcd.print("Disabled"); }
								else { lcd.print(" Enabled"); }
								break;
							case 8:
								break;
						}
						break;
					case 1:										//	prints 3rd level Timer Setup Items
						//	its going to print the same string no matter what case, so no switch is needed
						lcd.print(m2Items10[mPoint]);
						break;
					case 2:										//	prints 3rd level Sensor Addr Config Items
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items20[mPoint]);
								break;
							case 1:
								lcd.print(m2Items20[mPoint]);
								break;
							case 2:
								lcd.print(m2Items20[mPoint]);
								break;
							case 3:
								lcd.print(m2Items20[mPoint]);
								break;
						}
						break;
					case 3:										//	prints 3rd level Sensor Items
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items30[mPoint]);
								break;
							case 1:
								lcd.print(m2Items30[mPoint]);
								break;
							case 2:
								lcd.print(m2Items30[mPoint]);
								break;
							case 3:
								lcd.print(m2Items30[mPoint]);
								break;
							case 4:
								lcd.print(m2Items34[mPoint]);	//	prints flow sensor calibration items
								break;
						}
						break;
					case 4:										//	prints 3 level System Setup items
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items40[mPoint]);
								lcd.setCursor(12, 1);
								lcd.print(serialDebug, BIN);
								break;
							case 1:
								lcd.print(m2Items41[mPoint]);
								break;
							case 2:
								lcd.print(m2Items42[mPoint]);
								break;
						}
				break;
				}
			default:
				break;
		}
		mCur++;		//	increase the cursor and return to the begining of the for loop 
	}
	
	//	after the for loop prints the 3 lines it sets the new starting position for the pointer
	switch (mLevel)
	{
		case 0:
			mPoint = m0Start;
			break;
		case 1:
			mPoint = m1Start;
			break;
		case 2:
			mPoint = m2Start;
			break;
	}
	delay(150);
	
//  If it just entered the menu system it will call the MenuLoop function and reset the interrupt in the MCP chip.  Otherwise it will use the return to fall
//  back to the MenuLoop.  This prevents an endless loop with no return and keeps the stack from piling up.  When the
//	menu is called from the main loop it sets mRet to 0 to make sure it calls the MenuLoop the first time through.
	if (mRet == 0){					//	will only run the first time through
		mRet = 1;					//	set to 1 to cause subsequent runs to not do the following 2 lines
		mcpA.readByte(INTCAPA);		//	clear the interrupt in the mcpA
		MenuLoop();}				//	start the MenuLoop
		else{return;}				//	return to the MenuLoop
}
void MenuLoop()
{
	Alarm.delay(100);
	mcpA.readByte(GPIOA);		//	clear the interrupt from the MCP

	while (menuMode == 1)		//  scans for a button press to do the appropriate action
	{
		uint8_t Down = mcpA.readBit(menubuttonbank, downButton);
		uint8_t Up = mcpA.readBit(menubuttonbank, upButton);
		uint8_t Right = mcpA.readBit(menubuttonbank, rightButton);
		uint8_t Left = mcpA.readBit(menubuttonbank, leftButton);
		
		if (Up == 1){MenuUp();}
		if (Down == 1){MenuDown();}
		if (Right == 1){MenuSelect();}
		if (Left == 1){MenuBack();}
		
		menuTimeout++;
		if (menuTimeout == 10000){ menuMode = 0; }		//	this will exit the menu system after approx 20 seconds after a button has not been pushed
	}
	//	reset all pointers to 0 in preparation for the next time the menu is run
	mLevel = 0;
	m0Start = 0;
	m1Start = 0;
	m2Start = 0;
	mRet = 0;
	lcd.clear();			//  clear the screen
	today = 0;				//  set today to 0 so that the date function gets called
	Alarm.delay(100);
	mcpA.readByte(GPIOA);	//	clear the interrupt from MCP
	RelayStatusDisplay(0, 3);
	ReadTempSensors();			//  read the temp sensors so that the display has them
}

void MenuUp()
{
	switch (mLevel)		//  switches to adjust the appropriate pointer
	{
		case 0:
			m0Start--;
			break;
		case 1:
			m1Start--;
			break;
		case 2:
			m2Start--;
			break;
	}
	MenuTitle();
}

void MenuDown()
{
	switch (mLevel)		//  switches to adjust the appropriate pointer
	{
		case 0:
			m0Start++;
			break;
		case 1:
			m1Start++;
			break;
		case 2:
			m2Start++;
			break;
	}
	MenuTitle();
}

void MenuSelect()
{
	mLevel++;			//  increments the menu level
	switch (mLevel)		//	switches for selecting the appropriate title and menu lines
	{
		case 0:
			m0Start = 0;
			break;
		case 1:
			m1Sel = mStart;		//	sets the current position to the selected position
			m1Start = 0;
			break;
		case 2:
			m2Sel = mStart;		//	sets the current position to the selected position
			m2Start = 0;
			break;
		case 3:
			if (mLevel > 3){mLevel = 3;}	//  keeps the MLevel from overflowing
			m3Sel = mStart;		//	sets the current position to the selected position
			m3Start = 0;
			break;
	}
	MenuTitle();
}

void MenuBack()	//  function for going back 1 level in the menu system
{
	if (mLevel > 0) { mLevel--; }	//  decrements the menu level back 1 level if the level isnt 0 already.
	MenuTitle();
}

void MenuDo()	//  function for doing the currently selected menu item at the final level
{
	if ((serialDebug & 2) == 2)
	{
		Serial.print("Doing selection, ");
		Serial.print(m1Sel);
		Serial.print(", ");
		Serial.print(m2Sel);
		Serial.print(", ");
		Serial.println(m3Sel);
	}
	
	lcd.clear();
	lcd.setCursor(0,1);
	
	switch (m1Sel)
	{	case 0:		//  system configuration menu items
			switch (m2Sel)
			{
				case 0:
					lcd.setCursor(2, 0);
					lcd.print("Temperature Type");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							eeprom.write(20,0);
							tempType = eeprom.read(20);
							lcd.setCursor(3, 0);
							lcd.print("Set to Celsius");
							break;
						case 1:
							eeprom.write(20,1);
							tempType = eeprom.read(20);
							lcd.setCursor(2, 0);
							lcd.print("Set to Fahrenheit");
							break;
					}
					break;
				case 1:
					lcd.setCursor(3, 0);
					lcd.print("Temp Precision");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							eeprom.write(21,0);
							tempPrecision = eeprom.read(21);
							lcd.setCursor(1, 0);
							lcd.print("Set to No Decimal");
							break;
						case 1:
							eeprom.write(21,1);
							tempPrecision = eeprom.read(21);
							lcd.setCursor(2, 0);
							lcd.print("Set to 1 Decimal");
							break;
					}
					break;
				case 2:
					lcd.setCursor(2, 0);
					lcd.print("Temp Read Delay");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						tempReadDelay = eeprom.read(22);
						to = MenuNumSel(0, 22, tempReadDelay, 1, 60, 1, 9, 2, 200);
						if (to == 32767){ return; }
						tempReadDelay = eeprom.read(22);
						Alarm.write(tempReadID, tempReadDelay);
						Alarm.disable(tempReadID);
						Alarm.enable(tempReadID);
						break;
					}
					break;
				case 3:
					lcd.print("Backlight Brightness");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						backlightLevel = eeprom.read(25);
						to = MenuNumSel(0, 25, backlightLevel, 0, 255, 5, 9, 2, 250);
						if (to == 32767){ return; }
						backlightLevel = eeprom.read(25);
						analogWrite(backlight, backlightLevel);
						break;
					}
					break;
				case 4:
					lcd.setCursor(3, 0);
					lcd.print("Time Format");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							eeprom.write(23,0);
							timeFormat = eeprom.read(23);
							lcd.setCursor(3, 0);
							lcd.print("Set to 24 Hour");
							break;
						case 1:
							eeprom.write(23,1);
							timeFormat = eeprom.read(23);
							lcd.setCursor(3, 0);
							lcd.print("Set to 12 Hour");
							break;
					}
					break;
				case 5:
					lcd.setCursor(0, 0);
					lcd.setCursor(2, 0);
					lcd.print("Display Seconds");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						eeprom.write(24, 0);
						secondsDisplay = eeprom.read(24);
						lcd.setCursor(1, 0);
						lcd.print("Display Seconds Off");
						break;
					case 1:
						eeprom.write(24, 1);
						secondsDisplay = eeprom.read(24);
						lcd.setCursor(1, 0);
						lcd.print("Display Seconds On");
						break;
					}
					break;
				case 6:
					lcd.setCursor(1, 0);
					lcd.print("Set Date and Time");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							lcd.setCursor(6, 1);
							lcd.print((String)strExiting);
							break;
						case 1:
							uint8_t setyear;
							uint8_t setmonth;
							uint8_t setday;
							uint8_t setweekday;
							uint8_t sethour;
							uint8_t setminutes;
							uint8_t setseconds;
							uint8_t tmp = 99;
							uint8_t column;

							//	set up the lcd screen for setting the date
							lcd.clear();
							lcd.setCursor(4, 0);
							lcd.print("Set the Date");

							//	print the current date
							LCDDateDisplay(1, 5, 1);

							//	set the month
							lcd.setCursor(5, 2);
							lcd.write(byte(3));		//  print the up arrow
							lcd.write(byte(3));		//  print the up arrow
							setmonth = MenuNumSel(192, 255, month(), 1, 12, 1, 5, 1, 250);
							lcd.setCursor(5, 2);
							lcd.print("  ");

							// set the day
							lcd.setCursor(8, 2);
							lcd.write(byte(3));		//  print the up arrow
							lcd.write(byte(3));		//  print the up arrow
							setday = MenuNumSel(192, 255, day(), 1, 31, 1, 8, 1, 250);
							lcd.setCursor(8, 2);
							lcd.print("  ");

							//	set the year
							lcd.setCursor(13, 2);
							lcd.write(byte(3));		//  print the up arrow
							lcd.write(byte(3));		//  print the up arrow
							tmp = year() - 2000;
							setyear = MenuNumSel(192, 255, tmp, 1, 99, 1, 13, 1, 250);
							lcd.setCursor(13, 2);
							lcd.print("  ");

							//	set the day of the week
							lcd.clear();
							lcd.setCursor(1, 0);
							lcd.print("Set the Day of Week");
							setweekday = MenuNumSel(208, 255, weekday(), 1, 7, 1, 6, 2, 250);
							
							//	setup the lcd for the time setting
							lcd.clear();
							lcd.setCursor(4, 0);
							lcd.print("Set the Time");
							
							//	print the current time
							if (timeFormat == 1) { column = 4; }
							if (secondsDisplay == 0) { column = 4; }
							else { column = 6; }
							LCDTimeDisplay(2, column, 1, hour(), minute(), second(), 1);
							
							//	set the hour
							lcd.setCursor(column, 2);
							lcd.write(byte(3));
							lcd.write(byte(3));
							sethour = MenuNumSel(197, 255, hour(), 0, 23, 1, column, 1, 250);
							lcd.setCursor(column, 2);
							lcd.print("  ");

							//	set the minutes
							lcd.setCursor(column + 3, 2);
							lcd.write(byte(3));
							lcd.write(byte(3));
							setminutes = MenuNumSel(192, 255, minute(), 0, 59, 1, column + 3, 1, 250);
							lcd.setCursor(column + 3, 2);
							lcd.print("  ");

							//	set the seconds
							lcd.setCursor(column + 6, 2);
							lcd.write(byte(3));
							lcd.write(byte(3));
							setseconds = MenuNumSel(192, 255, second(), 0, 59, 1, column + 6, 1, 250);
							lcd.setCursor(column + 6, 2);
							lcd.print("  ");

							//	verify to set the time and date
							lcd.clear();
							lcd.setCursor(0, 0);
							lcd.print("Set Date and Time to");
							lcd.setCursor(6, 1);

							switch (setweekday)
							{
							case 0:
								lcd.print("Invalid number");
								break;
							case 1:
								lcd.print("Sunday   ");
								break;
							case 2:
								lcd.print("Monday   ");
								break;
							case 3:
								lcd.print("Tuesday  ");
								break;
							case 4:
								lcd.print("Wednesday");
								break;
							case 5:
								lcd.print("Thursday ");
								break;
							case 6:
								lcd.print("Friday   ");
								break;
							case 7:
								lcd.print("Saturday ");
								break;
							default:
								lcd.setCursor(1, 0);
								lcd.print("Invalid Selection");
								break;
							}

							lcd.setCursor(2, 2);
							lcd.print(setmonth);
							lcd.print("/");
							lcd.print(setday);
							lcd.print("/");
							lcd.print(setyear);
							lcd.print(" ");
							if (timeFormat == 1)
							{
								if (sethour == 0) {tmp = 12; }
								else if (sethour >= 13){ tmp = sethour - 12; }
							}
							else tmp = sethour;
							lcd.print(tmp);
							lcd.print(":");
							if (setminutes <= 9){ lcd.print("0"); }
							lcd.print(setminutes);
							lcd.print(":");
							if (setseconds <= 9){ lcd.print("0"); }
							lcd.print(setseconds);
							if (timeFormat == 1)
							{
								if (sethour >= 12){ lcd.print("PM"); }
								else if (sethour <= 11){ lcd.print("AM"); }
							}
							
							tmp = MenuNumSel(194, 255, 0, 0, 1, 1, 8, 3, 250);
							
							//	write the date and time to the RTC
							if (tmp == 0){ break; }
							else
							{
								Wire.beginTransmission(DS1307RTC);
								Wire.write(byte(0));
								Wire.write(decToBcd(setseconds));
								Wire.write(decToBcd(setminutes));
								Wire.write(decToBcd(sethour));
								Wire.write(decToBcd(setweekday));
								Wire.write(decToBcd(setday));
								Wire.write(decToBcd(setmonth));
								Wire.write(decToBcd(setyear));
								Wire.write(byte(0));
								Wire.endTransmission();

								//  GET THE TIME FROM THE RTC
								setSyncProvider(RTC.get);		//  this function get the time from the RTC
								if (timeStatus() != timeSet)		//  checks to see if it can read the RTC
								{
									RTC_Status = 0;
									Serial.println("Unable to get the RTC");
									Serial.println();
								}
								else{ Serial.println("RTC system time"); }
							}
							break;
					}
					break;
				case 7:
					lcd.setCursor(0, 0);
					lcd.print(" Flow Sensor Enable");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						eeprom.write(27, 0);
						flowSensorEnable = eeprom.read(27);
						Alarm.disable(flowReadID);
						lcd.print("Flow Sensor Disabled");
						break;
					case 1:
						eeprom.write(27, 1);
						flowSensorEnable = eeprom.read(27);
						Alarm.enable(flowReadID);
						lcd.print("Flow Sensor Enabled");
						break;
					}
					break;
				case 8:
					break;
			}
			break;
		case 1:		//  timer setup menu items
			switch (m2Sel)
			{
				case 0:		//	Timer 1 editing
					switch (m2Start)
					{
						case 0:
							AlarmSet(0);
							break;
						case 1:
							lcd.setCursor(6, 1);
							lcd.print((String)strExiting);
							break;
					}
					break;
				case 1:		//	Timer 2 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(1);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 2:		//	Timer 3 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(2);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 3:		//	Timer 4 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(3);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 4:		//	Timer 5 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(4);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 5:		//	Timer 6 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(5);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 6:		//	Timer 7 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(6);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
				case 7:		//	Timer 8 editing
					switch (m2Start)
					{
					case 0:
						AlarmSet(7);
						break;
					case 1:
						lcd.setCursor(6, 1);
						lcd.print((String)strExiting);
						break;
					}
					break;
			}
			break;
		case 2:		//  sensor addressing menu items
			break;
		case 3:		//  calibration menu items
			switch (m2Sel)
			{
			case 4:
				switch (m2Start)
				{
				case 0:
					uint8_t rd;
					lcd.setCursor(0, 1);
					lcd.print("Calibrate Flow Sens");
					rd = MenuNumSel(66, 28, 0, 0, 1, 1, 8, 3, 250);
					if (rd == 0){ break; }
					Alarm.disable(tempReadID);
					Alarm.disable(flowReadID);
					lcd.clear();
					lcd.setCursor(2, 0);
					lcd.print("Taking Readings");
					lcd.setCursor(1, 2);
					lcd.print("Sensor Reading #");
					for (uint8_t i = 0; i <= 4; i++)
					{
						flowRateMax = 0;		//	using this variable to supress the LCD display for Flow Good/Alarm
						FlowSensorRead();
						lcd.setCursor(17, 2);
						lcd.print(i+1);
						delay(1500);
					}
					lcd.clear();
					lcd.setCursor(2, 0);
					lcd.print("Avg Flow = ");
					lcd.print(flowPulseTotal / 5);
					lcd.setCursor(0, 1);
					lcd.print("Set As Normal Flow?");
					rd = MenuNumSel(66, 28, 0, 0, 1, 1, 8, 2, 250);
					if (rd == 0)
					{
						lcd.clear();
						lcd.setCursor(5, 1);
						lcd.print("Not Saving");
					}
					else
					{
						eeprom.write(28, (flowPulseTotal / 5));
						flowRateMax = eeprom.read(28);
						eeprom.write(27, 1);
						flowSensorEnable = eeprom.read(27);
						lcd.clear();
						lcd.setCursor(2, 1);
						lcd.print("Flow Rate Normal");
						lcd.setCursor(7, 2);
						lcd.print("Saved");
					}
					Alarm.enable(tempReadID);
					Alarm.enable(flowReadID);
					break;
				case 1:
					lcd.setCursor(2, 1);
					lcd.print("Set Minimum Flow");
					flowRateMin = eeprom.read(29);
					lcd.setCursor(11, 2);
					lcd.print("%");
					to = MenuNumSel(0, 29, flowRateMin, 5, 100, 5, 8, 2, 250);
					if (to == 32767){ return; }
					flowRateMin = eeprom.read(29);
					break;
				case 2:
					eeprom.write(27, 0);
					flowSensorEnable = eeprom.read(27);
					lcd.print("Flow Sensor Disabled");
					break;
				case 3:
					lcd.print("      Exiting");
					break;
				}
				break;
			}
			break;
		case 4:
			switch (m2Sel)
			{
			case 0:
				lcd.setCursor(2, 0);
				lcd.print("Serial Debugging");
				lcd.setCursor(0, 2);
				switch (m2Start)
				{
					byte readee;

				case 0:
					eeprom.write(5, 0);
					serialDebug = eeprom.read(5);
					lcd.setCursor(2, 2);
					lcd.print("All Debugging OFF");
					break;
				case 1:
					eeprom.write(5, 255);
					serialDebug = eeprom.read(5);
					lcd.setCursor(2, 2);
					lcd.print("ALL Debugging ON");
					break;
				case 2:
					readee = eeprom.read(5);
					if ((readee & 1) == 1)			//  see if the 1st bit flag is set.
					{
						readee = readee - 1;		//  if it is set, turn it off
						lcd.print("Disabled Tmp Sns Dbg");
					}
					else
					{
						readee = readee + 1;		//  if it is not set, turn it on
						lcd.print("Enabled Tmp Sns Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 3:
					readee = eeprom.read(5);
					if ((readee & 2) == 2)			//  see if the 2nd bit flag is set.
					{
						readee = readee - 2;		//  if it is set, turn it off
						lcd.print("Disabled Menu Dbg");
					}
					else
					{
						readee = readee + 2;		//  if it is not set, turn it on
						lcd.print("Enabled Menu Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 4:
					readee = eeprom.read(5);
					if ((readee & 4) == 4)			//  see if the 3rd bit flag is set.
					{
						readee = readee - 4;		//  if it is set, turn it off
						lcd.print("Disabled Alarm Dbg");
					}
					else
					{
						readee = readee + 4;		//  if it is not set, turn it on

						lcd.print("Enabled Alarm Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 5:
					readee = eeprom.read(5);
					if ((readee & 8) == 8)			//  see if the 4th bit flag is set.
					{
						readee = readee - 8;		//  if it is set, turn it off
						lcd.print("Disabled EEPROM Dbg");
					}
					else
					{
						readee = readee + 8;		//  if it is not set, turn it on
						lcd.print("Enabled EEPROM Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 6:
					readee = eeprom.read(5);
					if ((readee & 16) == 16)			//  see if the 4th bit flag is set.
					{
						readee = readee - 16;		//  if it is set, turn it off
						lcd.print("Disabled Relay Dbg");
					}
					else
					{
						readee = readee + 16;		//  if it is not set, turn it on
						lcd.print("Enabled Relay Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 7:
					readee = eeprom.read(5);
					if ((readee & 16) == 16)		//  see if the 5th bit flag is set.
					{
						readee = readee - 16;		//  if it is set, turn it off
						lcd.print("Disabled System Dbg");
					}
					else
					{
						readee = readee + 16;		//  if it is not set, turn it on
						lcd.print("Enabled System Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				case 8:
					readee = eeprom.read(5);
					if ((readee & 64) == 64)			//  see if the 4th bit flag is set.
					{
						readee = readee - 64;		//  if it is set, turn it off
						lcd.print("Disabled Flow Dbg");
					}
					else
					{
						readee = readee + 64;		//  if it is not set, turn it on
						lcd.print("Enabled Flow Dbg");
					}
					eeprom.write(5, readee);
					serialDebug = eeprom.read(5);
					break;
				}
				break;

			case 1:
				lcd.setCursor(0, 0);
				lcd.print("    Erase EEPROM");
				lcd.setCursor(0, 2);
				switch (m2Start)
				{
				case 0:
					lcd.setCursor(6, 2);
					lcd.print(strExiting);
					break;
				case 1:
					lcd.setCursor(3, 2);
					lcd.print("Erasing EEPROM");
					eeprom.eraseAll();
					lcd.clear();
					lcd.setCursor(3, 2);
					lcd.print("Erase Complete");
					break;
				}
				break;
			case 2:
				lcd.setCursor(2, 0);
				lcd.print("Restore Defaults");
				switch (m2Start)
				{
				case 0:
					lcd.setCursor(6, 2);
					lcd.print("Exiting");
					break;
				case 1:
					lcd.setCursor(1, 2);
					lcd.print("Restoring Defaults");
					serialDebug = (serialDebug | 8);
					factoryDefaultset();
					lcd.clear();
					lcd.setCursor(1, 2);
					lcd.print("Defaults Restored");
					break;
				}
				break;
			}
			break;
	}
	delay(1000);
	mLevel = mLevel-2;
	MenuTitle();
	return;
}
uint16_t MenuNumSel(uint16_t type, uint16_t addr, uint16_t start, uint16_t min, uint16_t max, uint16_t step, uint16_t col, uint16_t row, uint16_t dmicro)
//  type of display, EEprom addr, # to start on, minimum number to select, maximum number to select, step size, cursor column, cursor row, speed to run through the selection
//  If you set the max to 59, it will pad a 0 in front of the 1's digit if it is < 10
//  If you set the max to 23 and you have timeformat == 1 (12 hour), it will add AMPM display to the hours
//  types are 0=normal numbers, 1=time, 2=yes/no, ***4=enable/disable 8=+/-, 16=day of the week.  Add 64 to disable the Saving message or 128 to disable writing to eeprom
//	*** if time type and enable/disable setting are both set, this will force AM/PM to be displayed as if seconds were enabled.
//	If you are only displaying 1 digit, you need to set the col to -1 because all displays in this function are set to the 10's digit
{
	uint8_t loopNumSel = 1;		//	variable to stay in the loop
	uint8_t apm = 0;			//	variable for storing whether or not am or pm has rolled over

	delay(250);

	//	set the cursor depending on the number of digits and print the starting number

	if ((start == 0) && (max == 23) && (timeFormat == 1))
	{
		lcd.setCursor(col, row);
		start = start + 12;
		lcd.print(start);
		start = 0;
	}
	else if ((type & 8) == 8)				//	if +/- do not print start
	{
		lcd.setCursor(col - 2, row + 2);	//	set the cursor for the bottom row to print a ^ under the number
		lcd.print(" ");
		lcd.write(byte(3));
		lcd.setCursor(col - 1, row);
		if (start == 0){ lcd.print("-");}
		else { lcd.print("+"); }
	}
	else
	{
		if (start >= 100){ lcd.setCursor(col - 1, row); }
		else if (start >= 10){ lcd.setCursor(col, row); }
		else if (start < 10){ lcd.setCursor(col + 1, row); }
		lcd.print(start);
	}

	//  Main loop for all of the buttons
	while (loopNumSel == 1)
	{
		uint8_t Down = mcpA.readBit(menubuttonbank, downButton);
		uint8_t Up = mcpA.readBit(menubuttonbank, upButton);
		uint8_t Right = mcpA.readBit(menubuttonbank, rightButton);
		uint8_t Left = mcpA.readBit(menubuttonbank, leftButton);

		//  gets rid of leading digits for the below cases

		//	if +/- do not print any leading digits
		if ((type & 8) == 8){ lcd.setCursor(col + 1, row); }

		//  3 digits
		else if (start >= 100){ lcd.setCursor(col - 1, row); }

		//  2 digits and it is >=10
		else if (start >= 10)
		{
			lcd.setCursor(col - 1, row);
			if (max >= 100){ lcd.print(" "); }
			lcd.setCursor(col, row);
		}

		//	1 digit
		else if (start < 10)
		{
			lcd.setCursor(col - 1, row);
			if (max >= 100){ lcd.print("  "); }			//	pads 2 digits
			else
			{
				lcd.setCursor(col, row);
				if (max == 59){ lcd.print("0"); }		//  this will account for minutes and pad a 0
				else if (max != 59){ lcd.print(" "); }	//  if normal digits will pad a space
			}
			lcd.setCursor(col + 1, row);
		}

		//	Time type selection, timeFormat == 1 and other if otption
		if ((type & 1) == 1)
		{
			if (timeFormat == 1)
			{
				if (start == 0)					//	if start is 12 am add 12 to print a 12
				{
					start = start + 12;
					lcd.setCursor(col, row);	//	start thinks its a 1 so we have to adjust the cursor to the left 1 digit
					lcd.print(start);
					start = 0;
					apm = 0;
				}
				else if (start == 12)			//	needed to print PM for noon and not subtract 12
				{
					apm = 1;
					lcd.print(start);
				}
				else if ((start >= 13) && (start != 0))		//	subtract 12 from anything higher than noon
				{
					start = start - 12;
					apm = 1;
					if (start >= 10){ lcd.print(start);	}
					else if (start <= 9)
					{
						lcd.print(" ");
						lcd.setCursor(col + 1, row);
						lcd.print(start);
					}
					start = start + 12;
				}
				else if (start <= 11)						//	<=11 need to account for a single digit while also setting to AM
				{
					apm = 0;
					if (start >= 10){ lcd.print(start); }
					else if (start <= 9)
					{
						lcd.print(" ");
						lcd.setCursor(col + 1, row);
						lcd.print(start);
					}
				}
			}
			//	adds the AM/PM printing to the display
			if (timeFormat == 1)
			{
				//	it type is also set to 4 it will print as if the seconds were displayed.
				if ((secondsDisplay == 1) || ((type & 4) == 4) ){ lcd.setCursor(col + 9, row); }
				else { lcd.setCursor(col + 6, row); }

				switch (apm)
				{
				case 0:
					lcd.print("AM");
					break;
				case 1:
					lcd.print("PM");
					break;
				}
			}
			else { lcd.print(start); }			//	print start if timeFormat == 24 hour
		}
		
		//	Yes/No type selection
		else if ((type & 2) == 2)
		{
			switch (start)
			{
			case 0:
				lcd.setCursor(col, row + 1);
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.setCursor(col, row);
				lcd.print("No ");
				break;
			case 1:
				lcd.setCursor(col, row + 1);
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.setCursor(col, row);
				lcd.print("Yes");
				break;
			}
		}

		//	Enable/Disable type selection
		else if ((type & 4) == 4)
		{
			switch (start)
			{
			case 0:
				lcd.setCursor(col + 1, row + 1);
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.setCursor(col - 1, row);
				lcd.print("Disable");
				break;
			case 1:
				lcd.setCursor(col + 1, row + 1);
				lcd.write(byte(3));
				lcd.write(byte(3));
				lcd.setCursor(col - 1, row);
				lcd.print("Enable ");
				break;
			}
		}

		//	+/- type selection
		else if ((type & 8) == 8)
		{
			switch (start)
			{
			case 0:
				lcd.setCursor(col - 2, row + 2);	//	set the cursor for the bottom row to print a ^ under the number
				lcd.print(" ");
				lcd.write(byte(3));
				lcd.setCursor(col - 1, row);
				lcd.print("-");
				break;
			case 1:
				lcd.setCursor(col - 2, row + 2);	//	set the cursor for the bottom row to print a ^ under the number
				lcd.print(" ");
				lcd.write(byte(3));
				lcd.setCursor(col - 1, row);
				lcd.print("+");
				break;
			}
		}

		//	Weekday selection
		else if ((type & 16) == 16)
		{
			if (start > 7)
			{
				lcd.setCursor(col, row);
				lcd.print("Invalid number");
				break;
			}
			switch (start)
			{
			case 0:
				lcd.setCursor(col, row);
				lcd.print("Invalid number");
				break;

			case 1:
				lcd.setCursor(col, row);
				lcd.print("Sunday   ");
				break;
			case 2:
				lcd.setCursor(col, row);
				lcd.print("Monday   ");
				break;
			case 3:
				lcd.setCursor(col, row);
				lcd.print("Tuesday  ");
				break;
			case 4:
				lcd.setCursor(col, row);
				lcd.print("Wednesday");
				break;
			case 5:
				lcd.setCursor(col, row);
				lcd.print("Thursday ");
				break;
			case 6:
				lcd.setCursor(col, row);
				lcd.print("Friday   ");
				break;
			case 7:
				lcd.setCursor(col, row);
				lcd.print("Saturday ");
				break;

			}
		}




		else { lcd.print(start); }				//	print start if no other options
		
		//	Directional button code
		
		if (Up == 1)
			{
				menuTimeout = 0;
				if (start < max){ start = start + step; }		//	add the step size to start to increment
				else{ start = min; }			//	reset to min if the max has been reached

			}
		if (Down == 1)
			{
				menuTimeout = 0;
				if (start > min){ start = start - step; }		//	add the step size to start to increment
				else{ start = max; }						//	reset to min if the max has been reached
			}
		if (Right == 1)
			{
				menuTimeout = 0;
				if ((type & 64) != 64)									//	doesnt display saving if the 7th bit of type is set
				{
					lcd.setCursor(0, 2);
					lcd.print("                    ");
					lcd.setCursor(0, 3);
					lcd.print("       Saving       ");
				}
				if ((type & 128) != 128){ eeprom.write(addr, start); }	//	doesnt write to EEPROM if the 8th bit of type is set
					delay(50);
					loopNumSel = 0;
			}
		if (Left == 1)
			{
				menuTimeout = 0;
				if ((type & 64) != 64)									//	doesnt display saving if the 7th bit of type is set
				{
					lcd.setCursor(0, 2);
					lcd.print("                    ");
					lcd.setCursor(0, 3);
					lcd.print(" Exit Without Save  ");
				}
					delay(50);
					loopNumSel = 0;
			}
		menuTimeout++;
		if (menuTimeout == 50)		//	this will exit the menu system after approx 10 seconds after a button has not been pushed
		{
			loopNumSel = 0;
			menuMode = 0;
			menuTimeout = 0;
			start = 32767;
		}

		delay(dmicro);
	}
	return start;
}
