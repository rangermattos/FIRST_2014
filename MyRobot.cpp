#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "deviceManager.hpp"
#include "guiManager.hpp"
#include "vacManager.hpp"
#include "elevManager.hpp"
//#include "netManager.hpp"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	FRC::inputManager inpMan;
	FRC::deviceManager devices;
	FRC::guiManager guiMan;
	FRC::vacManager vacMan;
	FRC::elevManager elevMan;
	DriverStation *drive;
	float distance;
	float speed;
	bool IsArcade, prevArcade;
	
	//DriverStationLCD *display;
	//DriverStationEnhancedIO *displayenhanced;
	//netManager netMan;
public:
	RobotDemo():
		inpMan(0.1f, 								// threshold of 0.1f
				FRC::inputManager::MODE_XBOX_TANK),	// xbox tank mode to start
		devices(),
		guiMan(),
		IsArcade(0), 								// not arcade to start
		prevArcade(0), 								//previously not arcade
		drive(DriverStation::GetInstance()),
		//display(DriverStationLCD::GetInstance())
		//netMan(&guiMan)
		vacMan(&devices, &guiMan),
		elevMan(&devices, &guiMan)
		{
			//myRobot.SetExpiration(0.1);
			//devices.startEncoder();
			devices.startCompressor();
			//netMan.connect("10.51.48.50", 1180);
		}

        /**
         * Drive left & right motors for 2 seconds then stop
         */
        void Autonomous()
        {
        	while (IsAutonomous())
        	{
                //myRobot.SetSafetyEnabled(false);
        		//distance = devices->getVoltage(3)*512/5;
        		if (distance < 48) // less than 48 inches / 4 feet
        		{
        			// backwards
        			speed = -0.021 * distance + 1; // 0.021 = 1/48
        			speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        			speed /= 2;
        			devices.setSpeed(1, speed);
        			devices.setSpeed(2, -speed);
        		}
        		else if(distance > 72 ) // more than 72 inches / 6 feet
        		{
        			// forwards
        			speed = 0.021 * (distance-72);
        			speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        			speed /= 2;
        			devices.setSpeed(1, -speed);
        			devices.setSpeed(2, speed);
        		}
        		else 
        		{
        			// stopped
        			speed = 0.0f;
        			devices.setSpeed(1, speed);
        			devices.setSpeed(2, speed);
        		}
        		// print distance and speed
        		//display->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", distance);
        		//display->PrintfLine(DriverStationLCD::kUser_Line2, "speed = %f", speed);
        		//display->UpdateLCD();
        		Wait(0.005); // wait for a motor update time
        	}
        }

        /**
         * Runs the motors with arcade or tank steering. 
         */
        void OperatorControl()
        {
            //myRobot.SetSafetyEnabled(false);
        	//int i = 0;
        	devices.setPositionReference(1, 2);
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// Update InputManager
            	inpMan.update();
            	                
            	
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	//---------------Jaguar Mode--------------------------------------
        		devices.setControlMode(1 ,1);
        		devices.setControlMode(2 ,1);
        		devices.setControlMode(3 ,1);
                
            	//----------ARCADE TANK SWITCH------------------------------------
            	// xbox back button / joystick button 7 : switching drive modes
        		/*if (inpMan.getButton(7))
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
        		}*/
        		
                
        		//----------HIGH LOW SPEED SWITCHING------------------------------
                // read and activate solenoids
        		if (inpMan.getButton(6)) // toggle solenoids with x button / 3 button
        		{
        			while (inpMan.getButton(6))
        			{
        				if (!devices.isPistonExtended())
        				{
        					devices.setSolenoid(2, true);
        					devices.setSolenoid(3, false);
        					guiMan.print(0, "Speed Low");
        				}
        				else
        				{
        					devices.setSolenoid(2, false);
        					devices.setSolenoid(3, true);
        					guiMan.print(0, "Speed High");
        				}
        			}
    				devices.togglePistonExtended();
        		}
        		else
        		{
        			devices.setSolenoid(2, true);
        			devices.setSolenoid(3, false);
        			guiMan.print(0, "Speed Low");
        		}
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, inpMan.getMotor(1));
                devices.setSpeed(2, -inpMan.getMotor(2));
                
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                // button 3 shoots
                /*vacMan.vacuum();
                
                if(inpMan.getButton(3))
                {
                	vacMan.shoot();
                } */
                
                elevMan.moveArm(inpMan.getArmAxis());
                elevMan.moveElevator(inpMan.getElevAxis());
                
                
                //---------------------COMPRESSOR ON/OFF--------------------------
                // Compressor On/Off
                if (drive->GetDigitalIn(1) == 1)
                {
                	guiMan.print(1, "Compressor On");
                    devices.startCompressor();
                }
                else if (drive->GetDigitalIn(1) == 0)
                {
                   	guiMan.print(1, "Compressor Off");
                   	devices.stopCompressor();
                }
                                
             
                //------------------LCD PRINTS--------------------------------
                //------------------------------------------------------------
                //------------------------------------------------------------
                
                //------------------GUI PRINTS FOR DRIVER----------------------
                guiMan.print(2, "Left Motor = %f", -inpMan.getMotor(1));
                guiMan.print(3, "Right Motor = %f", -inpMan.getMotor(2));
                guiMan.print(4, "Elev Axis = %f", inpMan.getElevAxis());
                guiMan.print(5, "Arm Axis = %f", inpMan.getArmAxis());
                //guiMan.print(4, "Arm Pot = %f", devices.PotPosition());
                
                //-----------------UPDATES THE LCD--------------------
                // Update Driver Station LCD Display
                guiMan.update();
                //printf("counter = %i\n", i++);
                Wait(0.005); // wait for a motor update time
                        
               }
            
        }
        
        /**
         * Runs during test mode
         */
        void Test() 
        {
        	
        	while (IsTest())
        	{
        		devices.setControlMode(1 ,1);
        		devices.setControlMode(2 ,1);
        		devices.setControlMode(3 ,1);
        		devices.drivemotor1Control(inpMan.Joystick2());
        		devices.drivemotor2Control(inpMan.Joystick1());
        		devices.elevMotor1Control(inpMan.Joystick3());
        		devices.elevMotor2Control(inpMan.Joystick4());
        		devices.vacMotor1Control(inpMan.Joystick5());
        		devices.vacMotor2Control(inpMan.Joystick6());
        		devices.armMotorControl(inpMan.Joystick7());
        		guiMan.print(0, "X Axis VM 1 = %f", inpMan.Joystick5());
        		guiMan.print(1, "Y Axis VM 2 = %f", inpMan.Joystick6());
        		guiMan.print(2, "X Axis AM 1 = %f", inpMan.Joystick7());
        		//guiMan.print(0, "X Axis DM 1 = %f", inpMan.Joystick2());
        		//guiMan.print(1, "Y Axis DM 2 = %f", inpMan.Joystick1()); 
        		//guiMan.print(2, "X Axis EM 1 = %f", inpMan.Joystick3());
        		//guiMan.print(3, "Y Axis EM 2 = %f", inpMan.Joystick4()); 
                guiMan.update();
        	}
        }
};

START_ROBOT_CLASS(RobotDemo);
