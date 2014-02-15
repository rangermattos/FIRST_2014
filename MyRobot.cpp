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
        	// Set shifter to low
			devices.setSolenoid(2, true);
			devices.setSolenoid(3, false);
			guiMan.print(0, "Speed Low");
			
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
        		devices.setControlMode(1 ,1);//set the CANJaguar to speed mode.
                
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
                // read and activate solenoids for the speed shifter.
        		if (inpMan.getButton(1, 6)) // toggle solenoids with x button / 3 button
        		{
        			while (inpMan.getButton(1, 6))
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
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, inpMan.getMotor(1));
                devices.setSpeed(2, -inpMan.getMotor(2));
                
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------ARM CONTROL-------------------------------------------
                elevMan.moveArm(-inpMan.getAxis(2, 6));
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                // button 3 shoots
                //vacMan.vacuum();
                
                //if(inpMan.getButton(2, 1))
                //{
                	//vacMan.shoot();
                //}
                
                                
                
                //------------------COMPRESSOR ON/OFF--------------------------
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
                
                //------------------GUI PRINTS FOR DRIVER---------------------
                //guiMan.print(2, "Left Motor = %f", -inpMan.getMotor(1));
                //guiMan.print(3, "Right Motor = %f", -inpMan.getMotor(2));
                //guiMan.print(2, "Elev Axis = %f", inpMan.getElevAxis());
                //guiMan.print(3, "Arm Axis = %f", inpMan.getArmAxis());
                guiMan.print(3, "Arm Pot = %f", devices.getAnalogVoltage(3));
                //guiMan.print(4, "Elev Home = %d", devices.getHomeSwitch(1)); //if d doesn't work try i
                //guiMan.print(5, "Arm Home = %d", devices.getHomeSwitch(2));
                
                //-----------------UPDATES THE LCD----------------------------
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
        		if (inpMan.getButton(1, 6)) // toggle solenoids for shifters with RB button / 6 button
        		{
        			while (inpMan.getButton(1, 6))
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
                if (drive->GetDigitalIn(1) == 1)// starts the compressor using digital input one on the drivers station.
                {
                	guiMan.print(1, "Compressor On");
                    devices.startCompressor();
                }
                else if (drive->GetDigitalIn(1) == 0)// turns the compressor off using digital input one on the drivers station.
                {
                   	guiMan.print(1, "Compressor Off");
                   	devices.stopCompressor();
                }
        		devices.setControlMode(1, 1);// changes the control mode of the CANJaguar to speed control mode.
        		if (inpMan.getButton(2, 5) == 1)// Starts the vaccuum when button five (LB) is pressed.
        		{
        			devices.setCANJag(2, 1);
        			devices.setCANJag(3, 1);
        		}
        		else
        		{
        			devices.setCANJag(2, 0);
        			devices.setCANJag(3, 0);
        		}
        		if (inpMan.getButton(2, 6) == 1)//extends the firing piston when button six is pressed on the shooting joystick.
        		{
        			devices.setSolenoid(1, 1);
        		}
        		else
        		{
        			devices.setSolenoid(1, 0);
        		}
        		guiMan.print(2, "Vac Current = %f", devices.getCANJagCurrent(2));
        		guiMan.print(3, "Firing Solenoid = %d", inpMan.getButton(2, 6));
                guiMan.update();
        	}
        }
};

START_ROBOT_CLASS(RobotDemo);
