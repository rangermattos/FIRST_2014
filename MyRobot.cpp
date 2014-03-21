#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "deviceManager.hpp"
#include "guiManager.hpp"
#include "vacManager.hpp"
#include "elevManager.hpp"
#include "autoManager.hpp"
#include "NetworkTables/NetworkTable.h"
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
	bool inPosition, goodAngle, fired; // autonomous variables
	int CmpP, SpdP, Count, startup;
	//DriverStationLCD *display;
	//DriverStationEnhancedIO *displayenhanced;
	//netManager netMan;
	
public:
	RobotDemo():
		inpMan(0.1f, 								// threshold of 0.1f
				FRC::inputManager::MODE_JOY_TANK),	// xbox tank mode to start
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
        	guiMan.clear();
        	
        	devices.startCompressor();
        	startup = 0;
        	inPosition = false;
        	goodAngle = false;
        	fired = false;

        	devices.vacMotor1Control(1.0);
        	devices.vacMotor2Control(1.0);
        	
            //myRobot.SetSafetyEnabled(false);
        	
        	// Initial arm solenoid setting
			devices.setSolenoid(1, false); // arm solenoid
			devices.setSolenoid(2, true); // arm relief solenoid
			
			// Set shifter to high, compressor should be auto-set on
			devices.setSolenoid(3, false);
			devices.setSolenoid(4, true);
			SmartDashboard::PutBoolean("Shifter High", true);
			SpdP = 7; //Is there a reason for this?
			CmpP = 2;
			guiMan.print(0, "Spd LOW : Cmp ON");
			guiMan.print(1, "Vacuum OFF");
			
			// reset gyro before match
			devices.resetGyro();
			
        	while (IsAutonomous())
        	{
        		//SMART DASHBOARD COMMANDS
            	SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            	SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            	SmartDashboard::PutNumber("i2c Counter", Count);
            	SmartDashboard::PutNumber("Arm Angle", devices.getAnalogVoltage(3));
            	SmartDashboard::PutNumber("Gyro", devices.getAnalogVoltage(1));
            	
            	inPosition = false;
            	goodAngle = false;
        		
        		if (fired == false)
        		{
        			devices.vacMotor1Control(1.0);
        			devices.vacMotor2Control(1.0);
        			printf("loop wait\n");
        		}
        		
        		/*printf("Startup = %i\n", startup);
        		if (startup == 0)
        		{
        			devices.vacMotor1Control(1.0);
        			devices.vacMotor2Control(1.0);
        			guiMan.print(1, "Vacuum ON");
        			//vacStatus = true;
        			//vacSpeed = false;
        			guiMan.update();
        			startup = 1;
        			//printf("WAIT\n");
        			//Wait(1.0);
        		}*/
        		
        		// If we are not in the correct position and angle run the following
        		if (fired == false)
        		{
        			autoMan.correctPosition(64.0f, 6.0f); // Distance value in inches
        			autoMan.correctAngle(3.90f, 0.25f); // Angle value in voltage
        		}
        		inPosition = autoMan.isAtCorrectPosition();
        		goodAngle = autoMan.isAtCorrectAngle();

                //myRobot.SetSafetyEnabled(false);
        		//printf("GoodPos = %i  GoodAng = %i\n", inPosition, goodAngle);
        		        		
        		if(inPosition == true && goodAngle == true && fired == false)
        		{
        			// Fire
        			printf("FIRE\n");
        			vacMan.shoot(0.2);
        			fired = true;
        		}

        		
                // GUI Manager print statements
                guiMan.print(3, "Arm Pot = %f", devices.getAnalogVoltage(3));
                guiMan.print(4, "Distance(ft) = %f", devices.getAnalogVoltage(2)*512/60);
                guiMan.print(5, "Firing delay(s) = %f", 0.5*inpMan.getAxis(2, 4) + 0.5);
        		
        		guiMan.update();
        		Wait(0.005);
        	}
        }

        /**
         * Runs the motors with arcade or tank steering. 
         */
        void OperatorControl()
        {
        	guiMan.clear();
        	devices.startCompressor();
        	devices.vacMotor1Control(0.0);
        	devices.vacMotor2Control(0.0);
        	
            //myRobot.SetSafetyEnabled(false);
        	
        	// Initial arm solenoid setting
			devices.setSolenoid(1, false); // arm solenoid
			devices.setSolenoid(2, true); // arm relief solenoid
			
			// Set shifter to high, compressor should be auto-set on
			devices.setSolenoid(3, false);
			devices.setSolenoid(4, true);
			SmartDashboard::PutBoolean("Shifter High", true);
			SpdP = 3; //Are lines 164 - 166 needed?
			CmpP = 2;
			guiMan.print(0, "Spd HIGH : Cmp ON");
			guiMan.print(1, "Vacuum OFF");
			Count = 0;
			
			// Timer setup
        	Timer(t1);
        	t1.Reset();
        	t1.Start();
        	double	t0;
        	
        	// clear vac status
        	vacStatus = false;
        	vacSpeed = false;
        	
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// Initialize timer
            	t0 = t1.Get();
            	
            	// I2C read/write commands          	
            	// Light up LED based on arm position
            	/* green = wire_i2c->Write(3,0x67)
            	 * red = wire_i2c->Write(0,0x72)
            	 * temp = wire_i2c->Write(0,0x74); 
            	 *      = stat = wire_i2c->Transaction(datatosend,0,datarecieved,2);
            	 * RPM = wire_i2c->Write(0,0x73);
            	 *     = stat = wire_i2c->Transaction(datatosend,0,datarecieved,1);*/
            	
            	Count++;
            	if (Count >=200)
            	{
            		if (devices.getAnalogVoltage(3) >= 3.8 && devices.getAnalogVoltage(3) <= 4.0)
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
            	
            	// SmartDashboard calls
            	SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            	SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            	SmartDashboard::PutNumber("i2c Counter", Count);
            	SmartDashboard::PutNumber("Arm Angle", devices.getAnalogVoltage(3));
            	SmartDashboard::PutNumber("Ultrasonic Distance", devices.getAnalogVoltage(2));
            	if (3.8 <= devices.getAnalogVoltage(3) <= 4.0 /*&& 4.2 <= devices.getAnalogVoltage(2) <= 4.4*/)
            	{
            		SmartDashboard::PutBoolean("Ready to Fire", true);
        		}
            	else
            	{
            		SmartDashboard::PutBoolean("Ready to Fire", false);
            	}
            	SmartDashboard::PutNumber("Gyro", devices.getAnalogVoltage(1));
            	
            	// Update InputManager
            	inpMan.update();
            	                
            	//printf("1 - %f\n",t1.Get()-t0);
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
                
        		//----------HIGH LOW SPEED SWITCHING------------------------------
                // read and activate solenoids for the speed shifter.
        		if (inpMan.getButton(3, 1)) // toggle solenoids with x button / 3 button
        		{
        			devices.setSolenoid(3, false);
        			devices.setSolenoid(4, true);
        			SmartDashboard::PutBoolean("Shifter High", true);
        			//guiMan.print(0, "Speed high");
        		}
        		
        		if (inpMan.getButton(1, 1))
        		{
        			devices.setSolenoid(3, true);
        			devices.setSolenoid(4, false);
        			SmartDashboard::PutBoolean("Shifter High", false);
        			//guiMan.print(0, "Speed low");
        		}
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, inpMan.getMotor(1) * 0.9f);
                devices.setSpeed(2, -inpMan.getMotor(2) * 0.9f);
               //printf("2 - %f\n",t1.Get()-t0);
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------ARM CONTROL-------------------------------------------
                elevMan.moveArm(inpMan.getAxis(2, 2)); // Forward back axis of joystick
                //printf("2a - %f\n",t1.Get()-t0);
                // Push button to automate arm movement
                if (inpMan.getButton(2, 2))
                {
                	autoMan.correctAngle(3.95f, 0.05f);
                }
                //printf("3 - %f\n",t1.Get()-t0);	
                
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
                	devices.vacMotor1Control(0.0);
                	devices.vacMotor2Control(0.0);
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
                //printf("4 - %f\n",t1.Get()-t0);
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
                
                //printf("5 - %f\n",t1.Get()-t0);
                
                //------------------COMPRESSOR ON/OFF--------------------------
                // Compressor On/Off
                /*if (drive->GetDigitalIn(1) == 1) //change to remove == 1
                {
                	//guiMan.print(1, "Compressor On");
                    devices.startCompressor();
                }
                else if (drive->GetDigitalIn(1) == 0) //change to remove == 0, replace with !
                {
                   	//guiMan.print(1, "Compressor Off");
                   	devices.stopCompressor();
                }*/
                                
             
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
                
                // GUI Manager print statements
                guiMan.print(3, "Arm Pot = %f", devices.getAnalogVoltage(3));
                guiMan.print(4, "Distance(ft) = %f", devices.getAnalogVoltage(2)*512/60);
                //guiMan.print(5, "Firing delay(s) = %f", 0.5*inpMan.getAxis(2, 4) + 0.5);
               //printf("%f %f\n", inpMan.getMotor(1), inpMan.getMotor(2));
                
                //guiMan.print(2, "Left Motor = %f", -inpMan.getMotor(1));
                //guiMan.print(3, "Right Motor = %f", -inpMan.getMotor(2));
                //guiMan.print(2, "Elev Axis = %f", inpMan.getElevAxis());
                //guiMan.print(4, "Arm Axis = %f", inpMan.getAxis(2, 2));
                //guiMan.print(4, "Elev Home = %d", devices.getHomeSwitch(1)); //if d doesn't work try i
                //guiMan.print(5, "Arm Home = %d", devices.getHomeSwitch());
                //guiMan.print(4, "CANJag current = %f", devices.getCANJagCurrent(2));
                //guiMan.print(5, "CANJag current = %f", devices.getCANJagCurrent(3));
                //guiMan.print(5, "VacT = %ldF", datareceived[0]);
                
                // NetConsole print statements
                //printf("CANJag current = %f  %f\n", devices.getCANJagCurrent(2), devices.getCANJagCurrent(3));
                //printf("T1 = %ldF T2 = %ldF\n", datareceived[0], datareceived[1]);
                //printf("VacStatus = %i  VacSpeed = %i\n", vacStatus, vacSpeed);
                
                //-----------------UPDATES THE LCD----------------------------
                // Update Driver Station LCD Display
                guiMan.update();
                //printf("counter = %i\n", i++); // Everything is okay counter
                
                Wait(0.008); // wait for a motor update time
                //printf("time = %f\n", t1.Get()-t0);
                SmartDashboard::PutNumber("Update rate (s)", t1.Get()-t0);
                        
               }
            
        }
        
        /**
         * Runs during test mode
         */
        void Test() 
        {
        	// Initialization
        	
        	while (IsTest())
        	{        		
        		// Main loop
        		
        	}
        }
};

START_ROBOT_CLASS(RobotDemo);
