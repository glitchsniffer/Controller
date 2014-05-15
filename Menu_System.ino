
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
	lcd.clear();
	lcd.setCursor(0,0);
	if (MLevel <= 0){MLevel = 0;}
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
					lcd.print("   Timer Setup");
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
						break;
						case 1:
						lcd.print("    Set Timer 2");
						break;
						case 2:
						lcd.print("    Set Timer 3");
						break;
						case 3:
						lcd.print("    Set Timer 4");
						break;
					}
					break;
			}
			break;
	}
	MenuLines();
}
void MenuLines()
{
	if (MLevel == 2 && M1Sel > 0 && M2Sel > 0)
	{
		Serial.print("Doing Menu Items functions and returning");
		return;
	}
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
	}

	int mmax = 1;
	MPoint = MStart;
	mmax = MPoint+3;
	lcd.setCursor(0,2);
	lcd.write(byte(2));
	MCur = 1;
	for (; MPoint < mmax; MPoint++)
	{
		lcd.setCursor(1,MCur);
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
			default:
				break;
		}
		MCur++;
	}
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
	if (MLevel == 3){MenuDo();}
	delay(250);
	if (mRet == 0){
		mRet = 1;
		MenuLoop();}
		else{return;}
//	MenuLoop();
}
void MenuLoop()
{
	while (MenuMode == 1)
	{
		int Down = digitalRead(DownButton);
		int Up = digitalRead(UpButton);
		int Right = digitalRead(RightButton);
		int Left = digitalRead(LeftButton);
		
		if (Up == 0){MenuUp();}
		if (Down == 0){MenuDown();}
		if (Right == 0){MenuSelect();}
		if (Left == 0){MenuBack();}
		
		delay(250);
	}
	Serial.println("Exiting Menu loop");
	MLevel = 0;
	M0Start = 0;
	M1Start = 0;
	M2Start = 0;
	mRet = 0;
	lcd.clear();
	today = 0;
	lcd.setCursor(0,3);
	lcd.print("--------");	//  set the initial display for the relays
}

void MenuUp()
{
	switch (MLevel)
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
	switch (MLevel)
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
	MLevel++;
	switch (MLevel)
	{
		case 0:
			M0Start = 0;
			break;
		case 1:
			M1Sel = MStart;
			M1Start = 0;
			break;
		case 2:
			
			M2Sel = MStart;
			M2Start = 0;
			break;
	}
	MenuTitle();
}

void MenuBack()
{
	MLevel--;
	switch (MLevel)
	{
		case 0:
//			M0Start = 0;
			break;
		case 1:
			switch()
			M1Sel = MStart;
//			M1Start = 0;
			break;
		case 2:
			M2Sel = MStart;
			M2Start = 0;
			break;
		default:
			break;
	}
	MenuTitle();
}

void MenuDo()
{
	if (M1Start < 0){M1Start = 0;}
		
	switch (M1Start)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:
			break;
		default:
			break;
	}
}
void MenuYesNo()
{
	
}
