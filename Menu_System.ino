
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
			miMax = 4;
			break;
		case 1:
			switch (m1Sel)
			{
				case 0:
					lcd.print("     User Setup");
					miMax = 6;
					break;
				case 1:
					lcd.print("    Timer Setup");
					miMax = 7;
					break;
				case 2:
					lcd.print(" Sensor Addr Setup");
					miMax = 3;
					break;
				case 3:
					lcd.print("    Calibration");
					miMax = 4;
					break;
				case 4:
					lcd.print("    System Setup");
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
						lcd.print("     Temp Type");
						miMax = 1;
						break;
					case 1:
						lcd.print("   Temp Precision");
						miMax = 1;
						break;
					case 2:
						lcd.print("  Temp Read Delay");
						miMax = 0;
						break;
					case 3:
						lcd.print("  B Light Brightness");
						miMax = 0;
						break;
					case 4:
						lcd.print("    Time Format");
						miMax = 1;
						break;
					case 5:
						lcd.print("  Display Seconds");
						miMax = 1;
						break;
					case 6:
						lcd.print("   Set Date/Time");
						miMax = 1;
						break;
				}
				break;
			case 1:
				switch (m2Sel)
				{
					case 0:
						AlarmSetDisplay(m2Sel);
						break;
					case 1:
						AlarmSetDisplay(m2Sel);
						break;
					case 2:
						AlarmSetDisplay(m2Sel);
						break;
					case 3:
						AlarmSetDisplay(m2Sel);
						break;
					case 4:
						AlarmSetDisplay(m2Sel);
						break;
					case 5:
						AlarmSetDisplay(m2Sel);
						break;
					case 6:
						AlarmSetDisplay(m2Sel);
						break;
					case 7:
						AlarmSetDisplay(m2Sel);
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
			case 4:
				switch (m2Sel)
				{
					case 0:
						lcd.print("  Serial Debugging");
						miMax = 7;
						break;
					case 1:
						lcd.print("    Erase EEPROM");
						miMax = 1;
						break;
					case 2:
						lcd.print("  Restore Defaults");
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
						}
						break;
					case 1:										//	prints 3rd level Timer Setup Items
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
							case 4:
								lcd.print(m2Items13[mPoint]);
								break;
							case 5:
								lcd.print(m2Items13[mPoint]);
								break;
							case 6:
								lcd.print(m2Items13[mPoint]);
								break;
							case 7:
								lcd.print(m2Items13[mPoint]);
								break;
						}
						break;
					case 2:										//	prints 3rd level Sensor Addr Config Items
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
					case 3:										//	prints 3rd level Calibration Items
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
					case 4:
						switch (m2Sel)
						{
							case 0:
								lcd.print(m2Items40[mPoint]);
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
		
	delay(200);		//	small delay for debounce.  will get rid of this when I have a hardware debounce in place
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
	RelayStatusDisplay(0, 3);
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
					lcd.print("  Temp Read Delay");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						tempReadDelay = readEEPROM(22);
						MenuNumSel(0, 22, tempReadDelay, 1, 60, 1, 9, 2, 200);
						tempReadDelay = readEEPROM(22);
						Alarm.write(ReadDelay_ID, tempReadDelay);
						Alarm.disable(ReadDelay_ID);
						Alarm.enable(ReadDelay_ID);
						break;
					}
					break;
				case 3:
					lcd.print("Backlight Brightness");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						//							int blBright = 0;
						backlightLevel = readEEPROM(25);
						MenuNumSel(0, 25, backlightLevel, 1, 255, 5, 9, 2, 250);
						backlightLevel = readEEPROM(25);
						break;
					}
					break;
				case 4:
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
				case 5:
					lcd.setCursor(0, 0);
					lcd.print("  Display Seconds");
					lcd.setCursor(0, 2);
					switch (m2Start)
					{
					case 0:
						writeEEPROM(24, 0);
						secondsDisplay = readEEPROM(24);
						lcd.print(" Display Seconds Off");
						break;
					case 1:
						writeEEPROM(24, 1);
						secondsDisplay = readEEPROM(24);
						lcd.print(" Display Seconds On");
						break;
					}
					break;
				case 6:
					lcd.print(" Set Date and Time");
					lcd.setCursor(0,2);
					switch (m2Start)
					{
						case 0:
							break;
					}
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
							lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
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
						lcd.print("      Exiting");
						break;
					}
					break;
			}
			break;
		case 2:		//  sensor addressing menu items
			break;
		case 3:		//  calibration menu items
			break;
		case 4:
			switch (m2Sel)
			{
			case 0:
				lcd.print("  Serial Debugging");
				lcd.setCursor(0, 2);
				switch (m2Start)
				{
					byte readee;

				case 0:
					writeEEPROM(5, 0);
					serialDebug = readEEPROM(5);
					lcd.print("  All Debugging OFF");
					break;
				case 1:
					writeEEPROM(5, 255);
					serialDebug = readEEPROM(5);
					lcd.print("  ALL Debugging ON");
					break;
				case 2:
					readee = readEEPROM(5);
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
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
					break;
				case 3:
					readee = readEEPROM(5);
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
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
					break;
				case 4:
					readee = readEEPROM(5);
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
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
					break;
				case 5:
					readee = readEEPROM(5);
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
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
					break;
				case 6:
					readee = readEEPROM(5);
					if ((readee & 16) == 16)			//  see if the 4th bit flag is set.
					{
						readee = readee - 16;		//  if it is set, turn it off
						lcd.print("Disabled EEPROM Dbg");
					}
					else
					{
						readee = readee + 16;		//  if it is not set, turn it on
						lcd.print("Enabled EEPROM Dbg");
					}
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
					break;
				case 7:
					readee = readEEPROM(5);
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
					writeEEPROM(5, readee);
					serialDebug = readEEPROM(5);
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
					lcd.print("      Exiting");
					break;
				case 1:
					lcd.print("   Erasing EEPROM");
					eraseEEPROM();
					lcd.clear();
					lcd.setCursor(0, 0);
					lcd.print("    Erase EEPROM");
					lcd.setCursor(0, 2);
					lcd.print("   Erase Complete");
					break;
				}
				break;
			case 2:
				lcd.setCursor(0, 0);
				lcd.print("  Restore Defaults");
				lcd.setCursor(0, 2);
				switch (m2Start)
				{
				case 0:
					lcd.print("      Exiting");
					break;
				case 1:
					lcd.print(" Restoring Defaults");
					serialDebug = (serialDebug | 8);
					factoryDefaultset();
					lcd.clear();
					lcd.setCursor(0, 2);
					lcd.print(" Defaults Restored");
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
int MenuNumSel(int type, int addr, int start, int min, int max, int step, int col, int row, int dmicro)
//  type of display, EEprom addr, # to start on, minimum number to select, maximum number to select, step size, cursor column, cursor row, speed to run through the selection
//  If you set the max to 59, it will pad a 0 in front of the 1's digit if it is < 10
//  If you set the max to 23 and you have timeformat == 1 (12 hour), it will add AMPM display to the hours
//  types are 0=normal numbers, 1=time, 2=yes/no, 4=enable/disable 8=+/-.  Add 128 to any number to disable writing to eeprom
//	If you are only displaying 1 digit, you need to set the col to -1 because all displays in this function are set to the 10's digit
{
	int loopNumSel = 1;		//	variable to stay in the loop
	int apm = 0;			//	variable for storing whether or not am or pm has rolled over.

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
		int Down = digitalRead(downButton);
		int Up = digitalRead(upButton);
		int Right = digitalRead(rightButton);
		int Left = digitalRead(leftButton);

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
				switch (apm)
				{
				case 0:
					lcd.setCursor(col + 6, row);
					lcd.print("AM");
					break;
				case 1:
					lcd.setCursor(col + 6, row);
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
		else { lcd.print(start); }				//	print start if not time
		
		//	Directional button code
		
		if (Up == 1)
			{
				if (start < max){ start = start + step; }		//	add the step size to start to increment
				else{ start = (min + (max - max)); }			//	reset to min if the max has been reached

			}
		if (Down == 1)
			{
				if (start > min){ start = start - step; }		//	add the step size to start to increment
				else{ start = min + max; }						//	reset to min if the max has been reached
			}
		if (Right == 1)
			{
			if ((type & 64) != 64)									//	doesnt display saving if the 7th bit of type is set
			{
				lcd.setCursor(0, 2);
				lcd.print("                    ");
				lcd.setCursor(0, 3);
				lcd.print("      Saving        ");
			}
			if ((type & 128) != 128){ writeEEPROM(addr, start); }	//	doesnt write to EEPROM if the 8th bit of type is set
				delay(150);
				loopNumSel = 0;
			}
		if (Left == 1)
			{
			if ((type & 64) != 64)									//	doesnt display saving if the 7th bit of type is set
			{
				lcd.setCursor(0, 2);
				lcd.print("                    ");
				lcd.setCursor(0, 3);
				lcd.print("  Exit Without Save  ");
			}
				delay(150);
				loopNumSel = 0;
			}
		delay(dmicro);
	}
	return start;
}
