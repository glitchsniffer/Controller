
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
	if (MLevel == 3)		//	menu printing is not necessary if its the final level so it just calls the do function
		{MenuDo();
		return;}
		
	lcd.clear();					//	clear the menu in preparation for setting the new title
	lcd.setCursor(0,0);				//	set the lcd cursor back to 0,0
	if (MLevel <= 0){MLevel = 0;}	//	keeps the menu level from going negative

//	switches necessary to print the title on the menu page and to set the max number of lines for the given menu
	switch (MLevel)
	{
		case 0:
			lcd.print("     Main Menu");
			MIMax = 3;
			break;
		case 1:
			switch (M1Sel)
			{
				case 0:
					lcd.print("   System Config");
					MIMax = 5;
					break;
				case 1:
					lcd.print("    Timer Setup");
					MIMax = 3;
					break;
				case 2:
					lcd.print(" Sensor Addr Config");
					MIMax = 3;
					break;
				case 3:
					lcd.print("    Calibration");
					MIMax = 4;
					break;
			}
			break;
		case 2:
			switch (M1Sel)
			{
				case 0:
					switch (M2Sel)
					{
						case 0:
						lcd.print("     Temp Type");
						MIMax = 1;
						break;
						case 1:
						lcd.print("   Temp Precision");
						MIMax = 1;
						break;
						case 2:
						lcd.print("    Time Format");
						MIMax = 1;
						break;
						case 3:
						lcd.print("  B Light Brightness");
						MIMax = 1;
						break;
						case 4:
						lcd.print("   Set Date/Time");
						MIMax = 1;
						break;
						case 5:
						lcd.print("  Serial Debugging");
						MIMax = 1;
						break;
					}
					break;
				case 1:
					switch (M2Sel)
					{
						case 0:
						lcd.print("    Set Timer 1");
						MIMax = 1;
						break;
						case 1:
						lcd.print("    Set Timer 2");
						MIMax = 1;
						break;
						case 2:
						lcd.print("    Set Timer 3");
						MIMax = 1;
						break;
						case 3:
						lcd.print("    Set Timer 4");
						MIMax = 1;
						break;
					}
					break;
				case 2:
					switch (M2Sel)
					{
						case 0:
						lcd.print("  Temp Sens 1 Addr");
						MIMax = 1;
						break;
						case 1:
						lcd.print("  Temp Sens 2 Addr");
						MIMax = 1;
						break;
						case 2:
						lcd.print("  Temp Sens 3 Addr");
						MIMax = 1;
						break;
						case 3:
						lcd.print("  Temp Sens 4 Addr");
						MIMax = 1;
						break;
					}
					break;
				case 3:
					switch (M2Sel)
					{
						case 0:
						lcd.print(" Temp Sens 1 Calib");
						MIMax = 1;
						break;
						case 1:
						lcd.print(" Temp Sens 2 Calib");
						MIMax = 1;
						break;
						case 2:
						lcd.print(" Temp Sens 3 Calib");
						MIMax = 1;
						break;
						case 3:
						lcd.print(" Temp Sens 4 Calib");
						MIMax = 1;
						break;
						case 4:
						lcd.print(" Flow Sensor Calib");
						MIMax = 1;
						break;
					}
					break;
			}
			break;
	}

//	sets the menu navigation pointers up for the line printing for statement
	switch (MLevel)
	{	
		case 0:
			if (M0Start >= MIMax){M0Start = MIMax;}
			if (M0Start < 0){M0Start = 0;}
			MStart = M0Start;
			break;
		case 1:
			if (M1Start >= MIMax){M1Start = MIMax;}
			if (M1Start < 0){M1Start = 0;}
			MStart = M1Start;
			break;
		case 2:
			if (M2Start >= MIMax){M2Start = MIMax;}
			if (M2Start < 0){M2Start = 0;}
			MStart = M2Start;
			break;
		case 3:
			if (M3Start >= MIMax){M3Start = MIMax;}
			if (M3Start < 0){M3Start = 0;}
			MStart = M3Start;
			break;
	}

	int mmax = 1;			//	sets mmax = 1 for use to only print 3 lines.
	MPoint = MStart;		//	set the starting position of the pointer
	mmax = MPoint+3;		//  sets the ending position of the pointer to only print 3 lines
	lcd.setCursor(0,2);		//  moves the lcd cursor to line 3
	lcd.write(byte(2));		//	prints the right arrow to indicate the current menu selection
	MCur = 1;				//	sets the lcd cursor to start on line 1

//	this for loop actually prints the lines.  it runs this loop 3 times to print each line
	for (; MPoint < mmax; MPoint++)
	{
		lcd.setCursor(1,MCur);		//	move the lcd cursor to the lines location
		
//	determines what menu array to use when printing each line.
		switch (MLevel)
		{
			case 0:
				lcd.print(M0Items[MPoint]);
				break;
			case 1:
				switch (M1Sel)
				{
					case 0:
						lcd.print(M1Items0[MPoint]);
						break;
					case 1:
						lcd.print(M1Items1[MPoint]);
						break;
					case 2:
						lcd.print(M1Items2[MPoint]);
						break;
					case 3:
						lcd.print(M1Items3[MPoint]);
						break;
				}
				break;
			case 2:
				switch (M1Sel)
				{
					case 0:
						switch (M2Sel)
						{
							case 0:
								lcd.print(M2Items00[MPoint]);
								break;
							case 1:
								lcd.print(M2Items01[MPoint]);
								break;
							case 2:
								lcd.print(M2Items02[MPoint]);
								break;
							case 3:
								lcd.print(M2Items03[MPoint]);
								break;
							case 4:
								lcd.print(M2Items04[MPoint]);
								break;
							case 5:
								lcd.print(M2Items05[MPoint]);
								break;
						}
						break;
					case 1:
						switch (M2Sel)
						{
							case 0:
								lcd.print(M2Items10[MPoint]);
								break;
							case 1:
								lcd.print(M2Items11[MPoint]);
								break;
							case 2:
								lcd.print(M2Items12[MPoint]);
								break;
							case 3:
								lcd.print(M2Items13[MPoint]);
								break;
						}
						break;
					case 2:
						switch (M2Sel)
						{
							case 0:
								lcd.print(M2Items20[MPoint]);
								break;
							case 1:
								lcd.print(M2Items21[MPoint]);
								break;
							case 2:
								lcd.print(M2Items22[MPoint]);
								break;
							case 3:
								lcd.print(M2Items23[MPoint]);
								break;
						}
						break;
					case 3:
						switch (M2Sel)
						{
							case 0:
								lcd.print(M2Items30[MPoint]);
								break;
							case 1:
								lcd.print(M2Items31[MPoint]);
								break;
							case 2:
								lcd.print(M2Items32[MPoint]);
								break;
							case 3:
								lcd.print(M2Items33[MPoint]);
								break;
							case 4:
								lcd.print(M2Items34[MPoint]);
								break;
						}
						break;
				break;
				}
			default:
				break;
		}
		MCur++;		//	increase the cursor and return to the begining of the for loop
	}
	
//	after the for loop prints the 3 lines it sets the new starting position for the pointer
	switch (MLevel)
	{
		case 0:
			MPoint = M0Start;
			break;
		case 1:
			MPoint = M1Start;
			break;
		case 2:
			MPoint = M2Start;
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
	while (MenuMode == 1)		//  scans for a button press to do the appropriate action
	{
		int Down = digitalRead(DownButton);
		int Up = digitalRead(UpButton);
		int Right = digitalRead(RightButton);
		int Left = digitalRead(LeftButton);
		
		if (Up == 0){MenuUp();}
		if (Down == 0){MenuDown();}
		if (Right == 0){MenuSelect();}
		if (Left == 0){MenuBack();}
		
		delay(250);		//	small delay for debounce.  will get rid of this when I have a hardware debounce in place
	}
	Serial.println("Exiting Menu loop");	//	prints this message when it exits the menu loop
//	reset all pointers to 0 in preparation for the next time the menu is run
	MLevel = 0;
	M0Start = 0;
	M1Start = 0;
	M2Start = 0;
	mRet = 0;
	lcd.clear();			//  clear the screen
	today = 0;				//  set today to 0 so that the date function gets called
	lcd.setCursor(0,3);		//	starting position of the relays.
	lcd.print("--------");	//  set the initial display for the relays
}

void MenuUp()
{
	switch (MLevel)		//  switches to adjust the appropriate pointer
	{
		case 0:
			M0Start--;
			break;
		case 1:
			M1Start--;
			break;
		case 2:
			M2Start--;
			break;
	}
	MenuTitle();
}

void MenuDown()
{
	switch (MLevel)		//  switches to adjust the appropriate pointer
	{
		case 0:
			M0Start++;
			break;
		case 1:
			M1Start++;
			break;
		case 2:
			M2Start++;
			break;
	}
	MenuTitle();
}

void MenuSelect()
{
	MLevel++;			//  increments the menu level
	switch (MLevel)		//	switches for selecting the appropriate title and menu lines
	{
		case 0:
			M0Start = 0;
			break;
		case 1:
			M1Sel = MStart;		//	sets the current position to the selected position
			M1Start = 0;
			break;
		case 2:
			M2Sel = MStart;		//	sets the current position to the selected position
			M2Start = 0;
			break;
		case 3:
			if (MLevel > 3){MLevel = 3;}	//  keeps the MLevel from overflowing
			M3Sel = MStart;		//	sets the current position to the selected position
			M3Start = 0;
			break;
	}
	MenuTitle();
}

void MenuBack()
{
	MLevel--;
	MenuTitle();
}

void MenuDo()
{
	Serial.print("Doing selection, ");
	Serial.print(M1Sel);
	Serial.print(", ");
	Serial.print(M2Sel);
	Serial.print(", ");
	Serial.println(M3Sel);
	
	switch (M1Sel)
	{	case 0:
			switch (M2Sel)
			{
				case 0:
					switch (M2Start)
					{
						case 0:
							writeEEPROM(20,0);
							TempType = readEEPROM(20);
							break;
						case 1:
							writeEEPROM(20,1);
							TempType = readEEPROM(20);
							break;
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
					switch (M2Start)
					{
						case 0:
							writeEEPROM(5,0);
							Serial_Debug = readEEPROM(5);
							break;
						case 1:
							writeEEPROM(5,1);
							Serial_Debug = readEEPROM(5);
							break;
					}
					break;				
			}
			break;
		case 1:
			switch (M2Sel)
			{
				case 0:
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
		case 2:
			break;
		case 3:
			break;
	}
	MLevel = MLevel-2;
	MenuTitle();
	return;
}
void MenuBinary()
{
	if (M2Start = 0)
	{return;
	}
}
