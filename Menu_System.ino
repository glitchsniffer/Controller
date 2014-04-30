
//  MENU SYSTEM
//  ***********************************************
void MenuSystem()
{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("     Main Menu");
	int MenuPointer = 0;
	char* MenuItems[]={"System Config", "Timers Setup", "Sensor Addr Config","Calibration", "System Config"};
	
	while (MenuMode == 1)
	{	
		int cur = 0;
		lcd.setCursor(0,1);
		lcd.write(byte(2));
		for (int p = 0; p < 3; p++)
			{
				cur = p+1;
				lcd.setCursor(1,cur);
				lcd.print(MenuItems[p]);
			}
		
	//	lcd.setCursor(0,1);
	//	lcd.print(MenuItems[0]);
	//	lcd.print("Timer Setup  ");
	//	lcd.println("Sensor Address Config");
		
	}
	
	lcd.clear();
}