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
	I2C *wire;
	unsigned char datareceived[2];
	unsigned char datatosend[1];
	float distance;
	float angle;
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
        	devices.startCompressor();
        	devices.vacMotor1Control(1);
        	devices.vacMotor2Control(1);
        	while (IsAutonomous())
        	{
                //myRobot.SetSafetyEnabled(false);
        		distance = devices.getAnalogVoltage(2)*512/5;
        		while (distance < 48 || distance > 72)
        		{
        			distance = devices.getAnalogVoltage(2)*512/5;
        			if (distance < 48) // less than 48 inches / 4 feet
        			{
        				// backwards
        				speed = -0.021 * distance + 1; // 0.021 = 1/48
        				speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        				speed /= 2;
        				devices.setSpeed(1, -0.8*speed);
        				devices.setSpeed(2, speed);
        			}
        			else if(distance > 72 ) // more than 72 inches / 6 feet
        			{
        				// forwards
        				speed = 0.021 * (distance-72);
        				speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
        				speed /= 2;
        				devices.setSpeed(1, 0.8*speed);
        				devices.setSpeed(2, -speed);
        			}
        			else 
        			{
        				// stopped
        				speed = 0.0f;
        				devices.setSpeed(1, speed);
        				devices.setSpeed(2, speed);
        			}
        			Wait(0.005);
        		}
        		vacMan.vacuum();
        		angle = devices.getAnalogVoltage(3);
        		while (angle > 2.6)
        		{
        			angle = devices.getAnalogVoltage(3);
        			// JASPY COMMENTS: May need to set greater than -1 (-.9???) so it doesn't run past the desired angle
        			elevMan.moveArm(-1);
        			Wait(0.005);
        		}
        		
        		
        		if (angle < 2.7) 
        			vacMan.shoot();
        		
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
        	// Initial arm solenoid setting
			devices.setSolenoid(1, false); // arm solenoid
			devices.setSolenoid(2, true); // arm relief solenoid
			
			// Set shifter to low
			devices.setSolenoid(3, true);
			devices.setSolenoid(4, false);
			guiMan.print(0, "Speed Low");
			
        	devices.setPositionReference(1, 2);
        	
        	// I2C wire connection
        	//wire = DigitalModule::GetInstance(1)->GetI2C(4);
        	//wire->SetCompatibilityMode(true);
        	//conn = wire->AddressOnly();
        	
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// Initial I2C start up
            	//wire->Write(4, 0);
            	//wire->Transaction(datatosend, 0, datareceived, 2);
            	//guiMan.print(5, "CmpT = 0x %ld, VacT = 0x %X", datareceived[0], datareceived[1];
            	//itoa(A, butter, 16;
            	
            	// Update InputManager
            	inpMan.update();
            	                
            	
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	//---------------Jaguar Mode--------------------------------------
        		devices.setControlMode(1 ,1);//set the CANJaguar to speed mode.
        		//devices.setControlMode(2 ,1);
        		//devices.setControlMode(3 ,1);
                
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
        		if (inpMan.getButton(1, 7)) // toggle solenoids with x button / 3 button
        		{
        			devices.setSolenoid(3, false);
        			devices.setSolenoid(4, true);
        			guiMan.print(0, "Speed high");
        		}
        		
        		if (inpMan.getButton(1, 6))
        		{
        			devices.setSolenoid(3, true);
        			devices.setSolenoid(4, false);
        			guiMan.print(0, "Speed low");
        		}
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, -inpMan.getMotor(1) * 0.8f);
                devices.setSpeed(2, inpMan.getMotor(2) * 0.8f);
                
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------ARM CONTROL-------------------------------------------
                elevMan.moveArm(-inpMan.getAxis(2, 6));
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                if (inpMan.getButton(2, 5))
                {
                	devices.vacMotor1Control(1);
                	devices.vacMotor2Control(1);
                	guiMan.print(2, "Vacuum on");
                }
                
                if (inpMan.getButton(2, 6))
                {
                	devices.vacMotor1Control(0);
                	devices.vacMotor2Control(0);
                	guiMan.print(0, "Vacuum off");
                }
                
                if (inpMan.getButton(2, 2))
                {
                	vacMan.vacuum();
                	guiMan.print(3, "Solenoid armed");
                }

                // button 1 on joystick 2 shoots
                if (inpMan.getButton(2, 1))
                {
                	vacMan.shoot();
                	guiMan.print(3, "Solenoid fired");
                }
                
                //------------------COMPRESSOR ON/OFF--------------------------
                // Compressor On/Off
                if (drive->GetDigitalIn(1) == 1) //change to remove == 1
                {
                	guiMan.print(1, "Compressor On");
                    devices.startCompressor();
                }
                else if (drive->GetDigitalIn(1) == 0) //change to remove == 0, replace with !
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
                guiMan.print(5, "Distance = %f", devices.getAnalogVoltage(2)*512/5);
                
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
        	// set shifter to low
        	devices.setSolenoid(1, false); // arm solenoid
        	devices.setSolenoid(2, true); // arm relief solenoid
        	devices.setSolenoid(3, true); // 1/2 shifter solenoid
        	devices.setSolenoid(4, false); // 1/2 shifter solenoid
        	guiMan.print(0, "Speed low");
        	
        	while (IsTest())
        	{        		
        		// Compressor On/Off
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
                // Joysticks
        		devices.setControlMode(1, 1);
        		//devices.setControlMode(2, 1);
        		//devices.setControlMode(3, 1);
        		devices.drivemotor1Control(inpMan.Joystick2());
        		devices.drivemotor2Control(inpMan.Joystick1());
        		//devices.elevMotor1Control(inpMan.Joystick3());
        		//devices.elevMotor2Control(inpMan.Joystick4());
        		devices.vacMotor1Control(inpMan.Joystick5());
        		devices.vacMotor2Control(inpMan.Joystick6());
        		devices.armMotorControl(inpMan.Joystick7());
        		
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
        			devices.setSolenoid(1, true);
        			devices.setSolenoid(2, false);
        		}
        		else
        		{
        			devices.setSolenoid(1, false);
        			devices.setSolenoid(2, true);
        		}
        		guiMan.print(2, "Vac Current = %f", devices.getCANJagCurrent(2));
        		guiMan.print(3, "Firing Solenoid = %d", inpMan.getButton(2, 6));
        		guiMan.print(4, "Shifter switch = %d", inpMan.getButton(1, 6));
                guiMan.update();
        	}
        }
};

START_ROBOT_CLASS(RobotDemo);
