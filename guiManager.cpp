#include "guiManager.hpp"
#include <iostream>
FRC::guiManager::guiManager() :
display(DriverStationLCD::GetInstance())
{
        
}

void FRC::guiManager::print(int line, const char *message, ...)
{
	switch(line)
	{
	case 0:
		display->PrintfLine(DriverStationLCD::kUser_Line1, message);
		break;
	case 1:
		display->PrintfLine(DriverStationLCD::kUser_Line2, message);
		break;
	case 2:
		display->PrintfLine(DriverStationLCD::kUser_Line3, message);
		break;
	case 3:
		display->PrintfLine(DriverStationLCD::kUser_Line4, message);
		break;
	case 4:
		display->PrintfLine(DriverStationLCD::kUser_Line5, message);
		break;
	case 5:
		display->PrintfLine(DriverStationLCD::kUser_Line6, message);
		break;
	}
}

void FRC::guiManager::update()
{
	display->UpdateLCD();
}
