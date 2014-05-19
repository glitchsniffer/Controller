
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
	if (MLevel == 3)
	{
		MenuDo();
		return;
	}
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
	MenuLines();
}
void MenuLines()
{
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
			break;
	}
	if (MLevel == 3){MenuDo();}
	delay(250);
	if (mRet == 0){
		mRet = 1;
		MenuLoop();}
		else{return;}
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
		case 3:
			M3Sel = MStart;
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
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
	}
	return;
}
void MenuBinary()
{
	if (M2Start = 0)
	{
	}
}
