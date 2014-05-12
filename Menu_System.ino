
//  MENU SYSTEM
//  ***********************************************
void MenuTitle()
{
	lcd.clear();
	lcd.setCursor(0,0);
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
	}
//	MenuLines();
//}
//void MenuLines()
//{
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
				lcd.print(MMItems[MPoint]);
				break;
			case 1:
				switch (M1Sel)
				{
					case 0:
					lcd.print(M1Items[MPoint]);
					break;
					case 1:
					lcd.print(M2Items[MPoint]);
					break;
					case 2:
					lcd.print(M3Items[MPoint]);
					break;
					case 3:
					lcd.print(M4Items[MPoint]);
					break;
				}
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
	}
	delay(250);
	MenuLoop();
	return;
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
	}
	MenuTitle();
}

void MenuBack()
{
	MLevel--;
	switch (MLevel)
	{
		case 0:
		M0Start = 0;
		break;
		case 1:
		M1Sel = MStart;
		M1Start = 0;
		break;
	}
	MenuTitle();
}