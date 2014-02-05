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
	//LCD Print Commands
	                /*display->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", A1.GetVoltage()*512/5);
	                display->PrintfLine(DriverStationLCD::kUser_Line2, "motor1 = %f", inpMan.getMotor(1));
	                display->PrintfLine(DriverStationLCD::kUser_Line3, "motor2 = %f", -inpMan.getMotor(2));
	                display->PrintfLine(DriverStationLCD::kUser_Line4, "button = %d", switch1.Get());
	                display->PrintfLine(DriverStationLCD::kUser_Line1, "xbox button = %i", inpMan.getButton(8));
	                display->PrintfLine(DriverStationLCD::kUser_Line2, "xbox button = %i", inpMan.getButton(9));
	                display->PrintfLine(DriverStationLCD::kUser_Line3, "xbox button = %i", inpMan.getButton(10));
	                display->PrintfLine(DriverStationLCD::kUser_Line4, "xbox button = %i", inpMan.getButton(6));
	                display->PrintfLine(DriverStationLCD::kUser_Line5, "xbox button = %i", inpMan.getButton(7));
	                display->PrintfLine(DriverStationLCD::kUser_Line6, "xbox button = %i", inpMan.getButton(0));*/
	                
	                //display->PrintfLine(DriverStationLCD::kUser_Line6, "xbox button = %i", inpMan.getButton(0));
	                //display->PrintfLine(DriverStationLCD::kUser_Line2, "EncoderValue: %ld", devices.getEncoderValue());
	                
	                /*if(IsArcade)
	                   	display->PrintfLine(DriverStationLCD::kUser_Line1, "ArcadeMode");
	                else
	                   	display->PrintfLine(DriverStationLCD::kUser_Line1, "TankMode");
	                
	                display->PrintfLine(DriverStationLCD::kUser_Line2, "Pressure Switch = %d", compressor.GetPressureSwitchValue());*/
	                
	                // LCD Jaguar Can Print Commands
	                //display->PrintfLine(DriverStationLCD::kUser_Line1, "Voltage: %f", can->GetOutputVoltage());
	                //display->PrintfLine(DriverStationLCD::kUser_Line2, "Current: %f", can->GetOutputCurrent());
	                //display->PrintfLine(DriverStationLCD::kUser_Line3, "Temperature: %f", can->GetTemperature());
	                
	                // Enhanced IO Display and Interface Commands
	                //displayenhanced->SetLED(1, true);
	                //display->PrintfLine(DriverStationLCD::kUser_Line3, "x accel = %d", displayenhanced->GetAcceleration(DriverStationEnhancedIO::kAccelX));
	                //display->PrintfLine(DriverStationLCD::kUser_Line3, "Switch = %i", displayenhanced->GetDitigal(2));
	                
	                //guiMan.print(1, "Button 1 pressed: %d", inpMan.getButton(1));
	                
	                // Compressor Off
	                //guiMan.print(1, "DigitalIn 1 = %i", drive->GetDigitalIn(1));
}
