
//  MENU SYSTEM
//  ***********************************************
void MenuSystem()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("     Main Menu");
	MenuStart();
	int Down;
	int Up;
	int Right;
	int Left;

	while (MenuMode == 1)
	{	
		Down = digitalRead(DownButton);
		Up = digitalRead(UpButton);
		Right = digitalRead(RightButton);
		Left = digitalRead(LeftButton);

		Serial.print(Up);
		Serial.print(":");
		Serial.println(Down);
		if (Up == 0){MenuUp();}
		if (Down == 0){MenuDown();}

		delay(250);
	}
	
	lcd.clear();
}
void MenuStart()
{
	MenuPointer = 0;
	lcd.setCursor(0,2);
	lcd.write(byte(2));
	for (; MenuPointer < 3; MenuPointer++)
	{
		MenuCursor = MenuPointer+1;
		lcd.setCursor(1,MenuCursor);
		lcd.print(MenuItems[MenuPointer]);
	}
	MenuPointer = 0;
}

void MenuUp()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("     Main Menu");
	MenuPointer = MenuPointer - 1;
	Serial.print(MenuPointer);
	Serial.print(":");
	MenuStartCur = MenuPointer;
	Serial.print(MenuStartCur);
	Serial.print(":");
	MenuMaxCur = MenuPointer + 3;
	Serial.println(MenuMaxCur);
	lcd.setCursor(0,2);
	MenuCursor = 1;
	lcd.write(byte(2));
	for (; MenuPointer < MenuMaxCur; MenuPointer++)
	{
		lcd.setCursor(1,MenuCursor);
		lcd.print(MenuItems[MenuPointer]);
		MenuCursor = MenuCursor+1;
	}
	Serial.print(MenuPointer);
	MenuPointer = MenuStartCur;
	Serial.print(":");
	Serial.print(MenuPointer);
	Serial.println("<<");
	delay(500);
}

void MenuDown()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("     Main Menu");
	MenuPointer = MenuPointer + 1;
	Serial.print(MenuPointer);
	Serial.print(":");
	MenuStartCur = MenuPointer;
	Serial.print(MenuStartCur);
	Serial.print(":");
	MenuMaxCur = MenuPointer + 3;
	Serial.println(MenuMaxCur);
	lcd.setCursor(0,2);
	MenuCursor = 1;
	lcd.write(byte(2));
	for (; MenuPointer < MenuMaxCur; MenuPointer++)
	{
		lcd.setCursor(1,MenuCursor);
		lcd.print(MenuItems[MenuPointer]);
		MenuCursor = MenuCursor+1;
	}
	Serial.print(MenuPointer);
	MenuPointer = MenuStartCur;
	Serial.print(":");
	Serial.print(MenuPointer);
	Serial.println("<<");
	delay(500);
}