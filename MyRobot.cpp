#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "DriverStationEnhancedIO.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	//RobotDrive myRobot; // robot drive system
	FRC::inputManager inpMan;
	Talon m1, m2;
	AnalogChannel A1;
	float distance;
	float speed;
	bool IsArcade, prevArcade;
	DriverStationLCD *display;
	DigitalInput switch1;
	Compressor compressor;
	Solenoid sol1, sol2; // sol1 = pulls out, sol 2 =pulls in
	bool pistonExtended;
	CANJaguar *can;
	Encoder encoder;
	DriverStationEnhancedIO *displayenhanced;
	
public:
	RobotDemo():
		inpMan(0.1f, 								// threshold of 0.1f
				FRC::inputManager::MODE_XBOX_TANK),	// xbox tank mode to start
		m1(1),
		m2(2),
		A1(1),
		IsArcade(0), 								// not arcade to start
		prevArcade(0), 								//previously not arcade
		display(DriverStationLCD::GetInstance()),
		switch1(1),
		compressor(2,1),
		sol1(1),
		sol2(2),
		pistonExtended(0),
		can (new CANJaguar(1)),
		encoder(1, 2, true)
		{
			//myRobot.SetExpiration(0.1);
			encoder.Start();
		}

        /**
         * Drive left & right motors for 2 seconds then stop
         */
        void Autonomous()
        {
        	while (IsAutonomous())
        	{
                //myRobot.SetSafetyEnabled(false);
                //myRobot.Drive(-0.5, 0.0);         // drive forwards half speed
                //Wait(2.0);                      	// for 2 seconds
                //myRobot.Drive(0.0, 0.0);         	// stop robot
        		//printf("value = %d  voltage = %f  distance = %f \n", A1.GetValue(), A1.GetVoltage(), A1.GetVoltage()*512/5);
        		distance = A1.GetVoltage()*512/5;
        		if (distance < 48) // less than 48 inches / 4 feet
        		{
        			// backwards
        			speed = -0.021 * distance + 1; // 0.021 = 1/48
        			speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        			speed /= 2;
        			m1.Set(speed);
        			m2.Set(-speed);
        		}
        		else if(distance > 72 ) // more than 72 inches / 6 feet
        		{
        			// forwards
        			speed = 0.021 * (distance-72);
        			speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        			speed /= 2;
        			m1.Set(-speed);
        			m2.Set(speed);
        		}
        		else 
        		{
        			// stopped
        			speed = 0.0f;
        			m1.Set(speed);
        			m2.Set(speed);
        		}
        		// print distance and speed
        		display->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", distance);
        		display->PrintfLine(DriverStationLCD::kUser_Line2, "speed = %f", speed);
        		display->UpdateLCD();
        		Wait(0.005); // wait for a motor update time
        	}
        }

        /**
         * Runs the motors with arcade or tank steering. 
         */
        void OperatorControl()
        {
        	displayenhanced = &DriverStation::GetInstance()->GetEnhancedIO();
        	compressor.Start();
            //myRobot.SetSafetyEnabled(false);
            while (IsOperatorControl())
            {
            	//std::cout << "z : " << inpMan.getZ() << "\n";
            	// xbox back button / joystick button 7 : switching drive modes
        		if (inpMan.getButton(7))
        		{
        			while (inpMan.getButton(7))
        			{
        				if (IsArcade != prevArcade)
        				{
        					if(IsArcade && inpMan.getMode() == FRC::inputManager::MODE_XBOX_ARCADE)
        						inpMan.setMode(FRC::inputManager::MODE_XBOX_TANK);
        					else if(!IsArcade && inpMan.getMode() == FRC::inputManager::MODE_XBOX_TANK)
        						inpMan.setMode(FRC::inputManager::MODE_XBOX_ARCADE);
        					else if(IsArcade && inpMan.getMode() == FRC::inputManager::MODE_JOY_ARCADE)
        						inpMan.setMode(FRC::inputManager::MODE_JOY_TANK);
        					else
        						inpMan.setMode(FRC::inputManager::MODE_JOY_ARCADE);
        				}
        			}
        			prevArcade = IsArcade;
        			IsArcade = !IsArcade;
        		}
        			
        		/*  Joystick z-axis switching drive modes
        		if (inpMan.getZ() > 0.0f)
        		{
        			inpMan.setMode(FRC::inputManager::MODE_JOY_ARCADE);
        		}
        		else
        		{
        			inpMan.setMode(FRC::inputManager::MODE_JOY_TANK);
        		}
                */
        		
                //read and activate solenoids
        		if (inpMan.getButton(3))	// toggle solenoids with x button / 3 button
        		{
        			while (inpMan.getButton(3))
        			{
        				if (!pistonExtended)
        				{
        					sol1.Set(true);
        					sol2.Set(false);
        				}
        				else
        				{
        					sol1.Set(false);
        					sol2.Set(true);
        				}
        			}
    				pistonExtended = !pistonExtended;
        		}
        		else
        		{
        			sol1.Set(false);
        			sol2.Set(false);
        		}
        		
        		/*if (inpMan.getButton(6))//solenoid 2
        		{
        			sol2.Set(true);
        		}
        		else 
        		{
        			sol2.Set(false);
        		}*/
        		
                // Update InputManager
                inpMan.update();
                        
                //Set Motor Commands
                m1.Set(inpMan.getMotor(1));
                m2.Set(-inpMan.getMotor(2));
                //std::cout << "motor 1 : " << inpMan.getMotor(1) << ", Motor 2 : " << inpMan.getMotor(2) << "\n";
                        
                // LCD Print Commands
                /*display->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", A1.GetVoltage()*512/5);
                display->PrintfLine(DriverStationLCD::kUser_Line2, "motor1 = %f", inpMan.getMotor(1));
                display->PrintfLine(DriverStationLCD::kUser_Line3, "motor2 = %f", -inpMan.getMotor(2));
                display->PrintfLine(DriverStationLCD::kUser_Line4, "button = %d", switch1.Get());
                display->PrintfLine(DriverStationLCD::kUser_Line1, "xbox button = %i", inpMan.getButton(8));
                display->PrintfLine(DriverStationLCD::kUser_Line2, "xbox button = %i", inpMan.getButton(9));
                display->PrintfLine(DriverStationLCD::kUser_Line3, "xbox button = %i", inpMan.getButton(10));
                display->PrintfLine(DriverStationLCD::kUser_Line4, "xbox button = %i", inpMan.getButton(6));
                display->PrintfLine(DriverStationLCD::kUser_Line5, "xbox button = %i", inpMan.getButton(7));
                display->PrintfLine(DriverStationLCD::kUser_Line6, "xbox button = %i", inpMan.getButton(0));
                display->PrintfLine(DriverStationLCD::kUser_Line2, "EncoderValue: %ld", encoder.GetRaw());*/
                
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
                
                // Update Driver Station LCD Display
                display->UpdateLCD();
                Wait(0.005); // wait for a motor update time
                        
                }
            delete can;
        }
        
        /**
         * Runs during test mode
         */
        void Test() {

        }
};

START_ROBOT_CLASS(RobotDemo);
