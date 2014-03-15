#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "deviceManager.hpp"
#include "guiManager.hpp"
#include "vacManager.hpp"
#include "elevManager.hpp"
#include "autoManager.hpp"
#include <string>
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
	FRC::autoManager autoMan;
	DriverStation *drive;
	I2C *wire_i2c;
	unsigned char datareceived[2];
	unsigned char datatosend[1];
	float distance;
	float angle;
	float speed;
	bool IsArcade, prevArcade, vacStatus, vacSpeed;
	bool inPosition, goodAngle; // autonomous variables
	int CmpP, SpdP;
	int Count;
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
		inPosition(0),								// not yet in position
		goodAngle(0),								// not yet at correct angle
		drive(DriverStation::GetInstance()),
		//display(DriverStationLCD::GetInstance())
		//netMan(&guiMan)
		vacMan(&devices),
		elevMan(&devices),
		autoMan(&devices, &elevMan)
		{
			//myRobot.SetExpiration(0.1);
			devices.startCompressor();
			wire_i2c = DigitalModule::GetInstance(1)->GetI2C(4);
			//netMan.connect("10.51.48.50", 1180);
		}

        /**
         * Drive left & right motors for 2 seconds then stop
         */
        void Autonomous()
        {
        	devices.startCompressor();
        	devices.vacMotor1Control(0.9);
        	devices.vacMotor2Control(0.9);
        	guiMan.print(1, "Vacuum ON");
        	vacStatus = true;
        	vacSpeed = false;
        	Wait(1);
        	while (IsAutonomous())
        	{
                //myRobot.SetSafetyEnabled(false);
        		
        		//vacMan.vacuum();
        		        		
        		if(!autoMan.isAtCorrectPosition() && !autoMan.isAtCorrectAngle())
        		{
        			autoMan.correctPosition(60.0f, 6.0f);
        			autoMan.correctAngle(4.08f, 0.05f);
        		}
        		else
        		{
        			vacMan.shoot(0.2);
        		}
        		
        		Wait(0.005);
        	}
        }

        /**
         * Runs the motors with arcade or tank steering. 
         */
        void OperatorControl()
        {
        	guiMan.clear();
        	
            //myRobot.SetSafetyEnabled(false);
        	//int i = 0;
        	// Initial arm solenoid setting
			devices.setSolenoid(1, false); // arm solenoid
			devices.setSolenoid(2, true); // arm relief solenoid
			
			// Set shifter to low
			devices.setSolenoid(3, true);
			devices.setSolenoid(4, false);
			SpdP = 7;
			CmpP = 2;
			guiMan.print(0, "Spd LOW : Cmp ON");
			guiMan.print(1, "Vacuum OFF");
			Count = 0;
			
        	//devices.setPositionReference(1, 2);
        	Timer(t1);
        	t1.Reset();
        	t1.Start();
        	double	t0;
        	
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	t0 = t1.Get();
            	
            	// I2C read/write commands          	
            	// Light up LED based on arm position
            	/* green = wire_i2c->Write(3,0x67)
            	 * red = wire_i2c->Write(0,0x72)
            	 * temp = wire_i2c->Write(0,0x74); 
            	 *      = stat = wire_i2c->Transaction(datatosend,0,datarecieved,2);
            	 * RPM = wire_i2c->Write(0,0x73);
            	 *     = stat = wire_i2c->Transaction(datatosend,0,datarecieved,1);*/
            	Count = Count++;
            	if (Count >=20);
            	{
            		if (devices.getAnalogVoltage(3) >= 4.0 && devices.getAnalogVoltage(3) <= 4.2)
            		{
            			printf("stat = %d\n", wire_i2c->Write(3,0x67)); // sets LED green
            		}
            		else
            		{
            			printf("stat = %d\n", wire_i2c->Write(0,0x72)); // sets LED red
            		}
            		wire_i2c->Write(0,0x74); 
            		wire_i2c->Transaction(datatosend,0,datareceived,2);
            		Count = 0;
            	}
            	
            	// Update InputManager
            	inpMan.update();
            	                
            	
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
                
        		//----------HIGH LOW SPEED SWITCHING------------------------------
                // read and activate solenoids for the speed shifter.
        		if (inpMan.getButton(1, 6)) // toggle solenoids with x button / 3 button
        		{
        			devices.setSolenoid(3, false);
        			devices.setSolenoid(4, true);
        			//guiMan.print(0, "Speed high");
        		}
        		
        		if (inpMan.getButton(1, 7))
        		{
        			devices.setSolenoid(3, true);
        			devices.setSolenoid(4, false);
        			//guiMan.print(0, "Speed low");
        		}
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, -inpMan.getMotor(1) * 0.8f);
                devices.setSpeed(2, inpMan.getMotor(2) * 0.8f);
                
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------ARM CONTROL-------------------------------------------
                elevMan.moveArm(inpMan.getAxis(2, 2)); // Forward back axis of joystick
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                if (inpMan.getButton(2, 5))
                {
                	devices.vacMotor1Control(0.9);
                	devices.vacMotor2Control(0.9); //0.9 is good
                	guiMan.print(1, "Vacuum ON");
                	vacStatus = true;
                	vacSpeed = false;
                }
                
                if (inpMan.getButton(2, 6))
                {
                	devices.vacMotor1Control(0);
                	devices.vacMotor2Control(0);
                	guiMan.print(1, "Vacuum OFF");
                	vacStatus = false;
                	vacSpeed = false;
                }
                
                // button 1 on joystick 2 shoots
                if (inpMan.getButton(2, 1))
                {                	
                	guiMan.print(2, "Solenoid FIRED");
                	vacMan.shoot(0.5*inpMan.getAxis(2, 4) + 0.5);
                	guiMan.print(2, "Solenoid RESET");
                	vacStatus = false;
                	vacSpeed = false;
                }
                
                // Indicate when the vacuum is up to speed after being activated
                if (vacStatus && devices.getCANJagCurrent(2) >= 19.0)
                {
                	vacSpeed = true;
                }
                
                // Once vacuum at speed and active then check that it's pulled suction
                if (vacStatus && vacSpeed && devices.getCANJagCurrent(2) <= 15.0)
                {
                	devices.vacMotor1Control(1.0);
                	devices.vacMotor2Control(1.0);
                }
                
      
                
                //------------------COMPRESSOR ON/OFF--------------------------
                // Compressor On/Off
                if (drive->GetDigitalIn(1) == 1) //change to remove == 1
                {
                	//guiMan.print(1, "Compressor On");
                    devices.startCompressor();
                }
                else if (drive->GetDigitalIn(1) == 0) //change to remove == 0, replace with !
                {
                   	//guiMan.print(1, "Compressor Off");
                   	devices.stopCompressor();
                }
                                
             
                //------------------LCD PRINTS--------------------------------
                //------------------------------------------------------------
                //------------------------------------------------------------
                
                //------------------GUI PRINTS FOR DRIVER---------------------
                // Compressor state description
                if (drive->GetDigitalIn(1))
                	CmpP = 2;
                else
                    CmpP = 5;
                
                // Shifter state description
                if (inpMan.getButton(1, 6))
                	SpdP = 3;
                if (inpMan.getButton(1, 7))
                	SpdP = 7;
                
                // Combined compressor shifter state
                if (SpdP + CmpP == 12)
                	guiMan.print(0, "Spd LOW : Cmp OFF");
                if (SpdP + CmpP == 8) 
                	guiMan.print(0, "Spd HIGH : Cmp OFF");
                if (SpdP + CmpP == 5)
                	guiMan.print(0, "Spd HIGH : Cmp ON");
                if (SpdP + CmpP == 9)
                	guiMan.print(0, "Spd LOW : Cmp ON");
                
                //guiMan.print(2, "Left Motor = %f", -inpMan.getMotor(1));
                //guiMan.print(3, "Right Motor = %f", -inpMan.getMotor(2));
                //guiMan.print(2, "Elev Axis = %f", inpMan.getElevAxis());
                //guiMan.print(4, "Arm Axis = %f", inpMan.getAxis(2, 2));
                guiMan.print(3, "Arm Pot = %f", devices.getAnalogVoltage(3));
                //guiMan.print(4, "Elev Home = %d", devices.getHomeSwitch(1)); //if d doesn't work try i
                //guiMan.print(5, "Arm Home = %d", devices.getHomeSwitch());
                guiMan.print(4, "Distance(ft) = %f", devices.getAnalogVoltage(2)*512/60);
                //guiMan.print(4, "CANJag current = %f", devices.getCANJagCurrent(2));
                //guiMan.print(5, "CANJag current = %f", devices.getCANJagCurrent(3));
                guiMan.print(5, "Firing delay(s) = %f", 0.5*inpMan.getAxis(2, 4) + 0.5);
                //guiMan.print(5, "VacT = %ldF", datareceived[0]);
                printf("CANJag current = %f  %f\n", devices.getCANJagCurrent(2), devices.getCANJagCurrent(3));
                printf("T1 = %ldF T2 = %ldF\n", datareceived[0], datareceived[1]);
                
                //-----------------UPDATES THE LCD----------------------------
                // Update Driver Station LCD Display
                guiMan.update();
                //printf("counter = %i\n", i++);
                Wait(0.005); // wait for a motor update time
                printf("time = %f\n", t1.Get()-t0);
                        
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
        		
        	}
        }
};

START_ROBOT_CLASS(RobotDemo);
