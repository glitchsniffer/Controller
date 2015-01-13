
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
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
			lcd.print("     Main Menu");
			miMax = 3;
			break;
		case 1:
			switch (m1Sel)
			{
				case 0:
					lcd.print("   System Config");
					miMax = 8;
					break;
				case 1:
					lcd.print("    Timer Setup");
					miMax = 3;
					break;
				case 2:
					lcd.print(" Sensor Addr Config");
					miMax = 3;
					break;
				case 3:
					lcd.print("    Calibration");
					miMax = 4;
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
							lcd.print("     Temp Type");
							miMax = 1;
							break;
						case 1:
							lcd.print("   Temp Precision");
							miMax = 1;
							break;
						case 2:
							lcd.print("    Time Format");
							miMax = 1;
							break;
						case 3:
							lcd.print("  B Light Brightness");
							miMax = 0;
							break;
						case 4:
							lcd.print("   Set Date/Time");
							miMax = 1;
							break;
						case 5:
							lcd.print("  Serial Debugging");
							miMax = 1;
							break;
						case 6:
							lcd.print("  Temp Read Delay");
							miMax = 0;
							break;
						case 7:
							lcd.print("    Erase EEPROM");
							miMax = 1;
							break;
					}
					break;
				case 1:
					switch (m2Sel)
					{
					case 0:
					{		  
							int t = 0;
							lcd.print("    Set Timer 1");
							lcd.setCursor(0, 1);
							lcd.print("ON-");
							lcd.print(AlarmHourOn_0);
							lcd.print(":");
							lcd.print(AlarmMinOn_0);
							lcd.print("/");
							lcd.print("OFF-");
							lcd.print(AlarmHourOff_0);
							lcd.print(":");
							lcd.print(AlarmMinOff_0);
	
							miMax = 1;
							break;
						}
						case 1:
						lcd.print("    Set Timer 2");
						miMax = 1;
						break;
						case 2:
						lcd.print("    Set Timer 3");
						miMax = 1;
						break;
						case 3:
						lcd.print("    Set Timer 4");
						miMax = 1;
						break;
					}
					break;
				case 2:
					switch (m2Sel)
					{
						case 0:
						lcd.print("  Temp Sens 1 Addr");
						miMax = 1;
						break;
						case 1:
						lcd.print("  Temp Sens 2 Addr");
						miMax = 1;
						break;
						case 2:
						lcd.print("  Temp Sens 3 Addr");
						miMax = 1;
						break;
						case 3:
						lcd.print("  Temp Sens 4 Addr");
						miMax = 1;
						break;
					}
					break;
				case 3:
					switch (m2Sel)
					{
						case 0:
						lcd.print(" Temp Sens 1 Calib");
						miMax = 1;
						break;
						case 1:
						lcd.print(" Temp Sens 2 Calib");
						miMax = 1;
						break;
						case 2:
						lcd.print(" Temp Sens 3 Calib");
						miMax = 1;
						break;
						case 3:
						lcd.print(" Temp Sens 4 Calib");
						miMax = 1;
						break;
						case 4:
						lcd.print(" Flow Sensor Calib");
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

	int mmax = 1;			//	sets mmax = 1 for use to only print 3 lines.
	mPoint = mStart;		//	set the starting position of the pointer
	mmax = mPoint+3;		//  sets the ending position of the pointer to only print 3 lines
	lcd.setCursor(0,2);		//  moves the lcd cursor to line 3
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
				lcd.print(m0Items[mPoint]);
				break;
			case 1:
				switch (m1Sel)
				{
					case 0:
						lcd.print(m1Items0[mPoint]);
						break;
					case 1:
						lcd.print(m1Items1[mPoint]);
						break;
					case 2:
						lcd.print(m1Items2[mPoint]);
						break;
					case 3:
						lcd.print(m1Items3[mPoint]);
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
								lcd.print(m2Items00[mPoint]);
								break;
							case 1:
								lcd.print(m2Items01[mPoint]);
								break;
							case 2:
								lcd.print(m2Items02[mPoint]);
								break;
							case 3:
								lcd.print(m2Items03[mPoint]);
								break;
							case 4:
								lcd.print(m2Items04[mPoint]);
								break;
							case 5:
								lcd.print(m2Items05[mPoint]);
								break;
							case 6:
								lcd.print(m2Items06[mPoint]);
								break;
							case 7:
								lcd.print(m2Items07[mPoint]);
								break;
						}
						break;
					case 1:
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items10[mPoint]);
								break;
							case 1:
								lcd.print(m2Items11[mPoint]);
								break;
							case 2:
								lcd.print(m2Items12[mPoint]);
								break;
							case 3:
								lcd.print(m2Items13[mPoint]);
								break;
						}
						break;
					case 2:
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items20[mPoint]);
								break;
							case 1:
								lcd.print(m2Items21[mPoint]);
								break;
							case 2:
								lcd.print(m2Items22[mPoint]);
								break;
							case 3:
								lcd.print(m2Items23[mPoint]);
								break;
						}
						break;
					case 3:
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items30[mPoint]);
								break;
							case 1:
								lcd.print(m2Items31[mPoint]);
								break;
							case 2:
								lcd.print(m2Items32[mPoint]);
								break;
							case 3:
								lcd.print(m2Items33[mPoint]);
								break;
							case 4:
								lcd.print(m2Items34[mPoint]);
								break;
						}
						break;
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
	delay(250);
	
//  If it just entered the menu system it will call the MenuLoop function.  Otherwise it will use the return to fall
//  back to the MenuLoop.  This prevents an endless loop with no return and keeps the stack from piling up.  When the
//	menu is called from the main loop it sets mRet to 0 to make sure it calls the MenuLoop the first time through.
	if (mRet == 0){
		mRet = 1;
		MenuLoop();}
		else{return;}
}
void MenuLoop()
{
	while (menuMode == 1)		//  scans for a button press to do the appropriate action
	{
		int Down = digitalRead(downButton);
		int Up = digitalRead(upButton);
		int Right = digitalRead(rightButton);
		int Left = digitalRead(leftButton);
		
		if (Up == 1){MenuUp();}
		if (Down == 1){MenuDown();}
		if (Right == 1){MenuSelect();}
		if (Left == 1){MenuBack();}
		
	delay(100);		//	small delay for debounce.  will get rid of this when I have a hardware debounce in place
	}
	Serial.println("Exiting Menu loop");	//	prints this message when it exits the menu loop
//	reset all pointers to 0 in preparation for the next time the menu is run
	mLevel = 0;
	m0Start = 0;
	m1Start = 0;
	m2Start = 0;
	mRet = 0;
	lcd.clear();			//  clear the screen
	today = 0;				//  set today to 0 so that the date function gets called
	lcd.setCursor(0,3);		//	starting position of the relays.
	lcd.print("--------");	//  set the initial display for the relays
	DS18B20_Read();			//  read the temp sensors so that the display has them
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
	mLevel--;	//  decrements the menu level back 1 level
	MenuTitle();
}

void MenuDo()	//  function for doing the currently selected menu item at the final level
{
	Serial.print("Doing selection, ");
	Serial.print(m1Sel);
	Serial.print(", ");
	Serial.print(m2Sel);
	Serial.print(", ");
	Serial.println(m3Sel);
	
	lcd.clear();
	lcd.setCursor(0,1);
	
	switch (m1Sel)
	{	case 0:		//  system configuration menu items
			switch (m2Sel)
			{
				case 0:
					lcd.print("  Temperature Type");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							writeEEPROM(20,0);
							tempType = readEEPROM(20);
							lcd.print("   Set to Celsius");
							break;
						case 1:
							writeEEPROM(20,1);
							tempType = readEEPROM(20);
							lcd.print("  Set to Fahrenheit");
							break;
					}
					break;
				case 1:
					lcd.print("   Temp Precision");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							writeEEPROM(21,0);
							tempPrecision = readEEPROM(21);
							lcd.print(" Set to No Decimal");
							break;
						case 1:
							writeEEPROM(21,1);
							tempPrecision = readEEPROM(21);
							lcd.print("  Set to 1 Decimal");
							break;
					}
					break;
				case 2:
					lcd.print("   Time Format");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							writeEEPROM(23,0);
							timeFormat = readEEPROM(23);
							lcd.print("   Set to 24 Hour");
							break;
						case 1:
							writeEEPROM(23,1);
							timeFormat = readEEPROM(23);
							lcd.print("   Set to 12 Hour");
							break;
					}
					break;
				case 3:
					lcd.print("Backlight Brightness");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
//							int blBright = 0;
							backlightLevel = readEEPROM(23);
							MenuNumSel(23,backlightLevel,1,255,5,100);
							backlightLevel = readEEPROM(23);
							break;
					}
					break;
				case 4:
					lcd.print(" Set Date and Time");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							MenuTimeSet();
							break;							
					}
					break;
				case 5:
					lcd.print("  Serial Debugging");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							writeEEPROM(5,0);
							serialDebug = readEEPROM(5);
							lcd.print("     Set to ON");
							break;
						case 1:
							writeEEPROM(5,1);
							serialDebug = readEEPROM(5);
							lcd.print("     Set to OFF");
							break;
					}
					break;
				case 6:
					lcd.print("  Temp Read Delay");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
						tempReadDelay = readEEPROM(22);
						MenuNumSel(22,tempReadDelay,1,60,1,200);
						tempReadDelay = readEEPROM(22);
						Alarm.write(ReadDelay_ID, tempReadDelay);
						Alarm.disable(ReadDelay_ID);
						Alarm.enable(ReadDelay_ID);
						break;
					}
					break;
				case 7:
					lcd.print("    Erase EEPROM");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						lcd.print("      Exiting");
						break;
					case 1:
						lcd.print("   Erasing EEPROM");
						eraseEEPROM();
						lcd.print("   Erase Complete");
						break;
					}
					break;
			}
			break;
		case 1:		//  timer setup menu items
			switch (m2Sel)
			{
				case 0:
				{
					delay(250);
					AlarmHourOn_0 = readEEPROM(103);	//  reads out alarm setting for the hour on
					AlarmMinOn_0 = readEEPROM(104);	//  reads out alarm setting for the mins off
					AlarmHourOff_0 = readEEPROM(105); //  reads out alarm setting for the hour off
					AlarmMinOff_0 = readEEPROM(106);	//  reads out alarm setting for the mins off

					//  set the LCD screen up for the Hour ON edit
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("    Timer 1 Edit");
					lcd.setCursor(0, 1);
					lcd.print(" Set Timer 1 Hour ON");
					MenuNumSel(103, AlarmHourOn_0, 0, 24, 1, 220);	//  call the number selection menu
					AlarmHourOn_0 = readEEPROM(103);

					//  set the LCD screen up for the Minute ON edit
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("    Timer 1 Edit");
					lcd.setCursor(0, 1);
					lcd.print("  Set Timer 1 Min ON");
					MenuNumSel(104, AlarmMinOn_0, 0, 59, 1, 220);

					//  set the LCD screen up for the Hour OFF edit
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("    Timer 1 Edit");
					lcd.setCursor(0, 1);
					lcd.print(" Set Timer 1 Hour OFF");
					MenuNumSel(105, AlarmHourOff_0, 0, 24, 1, 220);	//  call the number selection menu


					//  set the LCD screen up for the Minute OFF edit
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("    Timer 1 Edit");
					lcd.setCursor(0, 1);
					lcd.print("  Set Timer 1 Min OFF");
					MenuNumSel(106, AlarmMinOff_0, 0, 59, 1, 220);

					//  read the new alarm times from the EEPROM

					AlarmHourOn_0 = readEEPROM(103);	//  reads out alarm setting for the hour on
					AlarmMinOn_0 = readEEPROM(104);	//  reads out alarm setting for the mins off
					AlarmHourOff_0 = readEEPROM(105); //  reads out alarm setting for the hour off
					AlarmMinOff_0 = readEEPROM(106);	//  reads out alarm setting for the mins off

					//  write the alarms to the TimeAlarms library
					time_t setAlarmTimeOn = AlarmHMS(AlarmHourOn_0, AlarmMinOn_0, 0);
					time_t setAlarmTimeOff = AlarmHMS(AlarmHourOff_0, AlarmMinOff_0, 0);

					Serial.print("setAlarmTime ON - ");
					Serial.println(setAlarmTimeOn);
					Alarm.disable(AlarmIDOn_0);
					Alarm.write(AlarmIDOn_0, AlarmHMS(AlarmHourOn_0, AlarmMinOn_0, 0));
					Alarm.enable(AlarmIDOn_0);

					Serial.print("setAlarmTime OFF - ");
					Serial.println(setAlarmTimeOff);
					Alarm.disable(AlarmIDOff_0);
					Alarm.write(AlarmIDOff_0, setAlarmTimeOff);
					Alarm.enable(AlarmIDOff_0);
					
					int rdon;
					rdon = Alarm.read(AlarmIDOn_0);
					Serial.print("Alarm A On Time = ");
					Serial.println(rdon);

					int rdoff;
					rdoff = Alarm.read(AlarmIDOff_0);
					Serial.print("Alarm A Off Time = ");
					Serial.println(rdoff);
				}
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
			}
			break;
		case 2:		//  sensor addressing menu items
			break;
		case 3:		//  calibration menu items
			break;
	}
	delay(2000);
	mLevel = mLevel-2;
	MenuTitle();
	return;
}
void MenuNumSel (int addr,int start,int min,int max,int step,int dmicro)
//  EEprom addr, # to start on, minimum number to select, maximum number to select, step size, speed to run through the selection
{
	int loopNumSel = 1;
	
	delay(100);
	lcd.setCursor(9,2);
	lcd.print(start);
	
	while (loopNumSel == 1)
	{
		int Down = digitalRead(downButton);
		int Up = digitalRead(upButton);
		int Right = digitalRead(rightButton);
		int Left = digitalRead(leftButton);
		
		if (Up == 1)
			{
				if (start < max)
					{start = start + step;}
				else{ start = max - max; }
			lcd.setCursor(9,2);
			lcd.print(start);
			lcd.print("   ");
			}
		if (Down == 1)
			{
				if (start > min)
					{start = start-step;}
				else{start = min + max;}
			lcd.setCursor(9,2);
			lcd.print(start);
			lcd.print("   ");
			}
		if (Right == 1)
			{
				lcd.setCursor(0, 3);
				lcd.print("       Saving       ");
				writeEEPROM(addr, start);
				delay(150);
				loopNumSel = 0;
			}
		if (Left == 1)
			{
				lcd.setCursor(0,3);
				lcd.print(" Exit Without Save");
				delay(150);
				loopNumSel = 0;
			}
		delay(dmicro);
	}
	return;
}

/*void MenuYesNo(int addr, int start)
{
	int loopNumSel = 1;

	delay(250);
	lcd.setCursor(9, 2);
	lcd.print(start);

	while (loopNumSel == 1)
	{
		int Down = digitalRead(downButton);
		int Up = digitalRead(upButton);
		int Right = digitalRead(rightButton);
		int Left = digitalRead(leftButton);

		if (Up == 1)
		{

		}
		if (Down == 1)
		{

		}
		if (Right == 1)
		{
			writeEEPROM(addr, start);
			lcd.setCursor(0, 3);
			lcd.print("       Saving       ");
			delay(150);
			loopNumSel = 0;
		}
		if (Left == 1)
		{
			lcd.setCursor(0, 3);
			lcd.print(" Exit Without Save");
			delay(150);
			loopNumSel = 0;
		}
		delay(200);
	}
	return;
}*/
void MenuTimeSet ()
{
	/*int loopTime = 1;
	
	delay(250);
	lcd.setCursor(0,1);
	lcd.print("");
	
	while (loopTime == 1)
	{
		int Down = digitalRead(downButton);
		int Up = digitalRead(upButton);
		int Right = digitalRead(rightButton);
		int Left = digitalRead(leftButton);
		
		if (Up == 1)
		{if (start < max)
			{
				start = start + step;
			}
			else{ start == max; }
			lcd.setCursor(9, 2);
			lcd.print(start);
			lcd.print("   ");
		}
		if (Down == 1)
		{

		}
		if (Right == 1)
		{

		}
		if (Left == 1)
		{

		}
		delay(150);
	}
	return;
	*/
}
void AlarmAON4()
{
	Serial.println("Alarm: - turn lights ON");
	digitalWrite(relayPins[0], LOW);
	lcd.setCursor(0, 3);
	lcd.print("+");
}