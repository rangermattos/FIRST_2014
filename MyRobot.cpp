#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "deviceManager.hpp"
#include "guiManager.hpp"
#include "vacManager.hpp"
#include "armManager.hpp"
#include "autoManager.hpp"
#include "NetworkTables/NetworkTable.h"
#include "commonFunctions.hpp"
#include <iostream>
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
	FRC::armManager armMan;
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
	int CmpP, SpdP, Count; // robot state variable indicators
	Timer(t1); // timer for loop
	double prevTime, currTime, deltaT; // stores previous, current, change in time for loop
	bool firstCallIndicator; // used to clear the error integration function for the automative moves
	//DriverStationLCD *display;
	//DriverStationEnhancedIO *displayenhanced;
	//netManager netMan;
	
public:
	RobotDemo():
		inpMan(0.1f, 								// threshold of 0.1f
				FRC::inputManager::MODE_JOY_TANK),	// joystick tank mode to start
		devices(),
		guiMan(),
		IsArcade(0), 								// not arcade to start
		prevArcade(0), 								// previously not arcade
		inPosition(0),								// not yet in position
		goodAngle(0),								// not yet at correct angle
		t1(),
		drive(DriverStation::GetInstance()),
		//display(DriverStationLCD::GetInstance())
		//netMan(&guiMan)
		vacMan(&devices),
		armMan(&devices),
		autoMan(&devices, &armMan),
		firstCallIndicator(0)
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
        	inPosition = false;
        	goodAngle = false;
        	fired = false;

        	devices.vacMotor1Control(1.0);
        	devices.vacMotor2Control(1.0);
        	
            //myRobot.SetSafetyEnabled(false);
        	
        	// Initial arm solenoid setting, not armed
			devices.setSolenoid(1, false); // arm solenoid
			devices.setSolenoid(2, true); // arm relief solenoid
			
			// Set shifter to high, compressor should be auto-set on
			devices.setSolenoid(3, false);
			devices.setSolenoid(4, true);
			SmartDashboard::PutBoolean("Shifter High", true);
			
			guiMan.print(0, "Spd LOW : Cmp ON");
			guiMan.print(1, "Vacuum ON");
			
			// reset gyro before match
			devices.resetGyro();
			
			// initialize timer
			t1.Reset();
			t1.Start();
			prevTime = -1.0;
			
        	while (IsAutonomous())
        	{
        		// set initial prevTime unless currTime is set
        		prevTime = (prevTime < 0) ? t1.Get() : currTime;
    			currTime = t1.Get();
    			deltaT = currTime - prevTime;
        		
        		// SMART DASHBOARD COMMANDS
            	SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            	SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            	SmartDashboard::PutNumber("JAG1 Current", devices.getCANJagCurrent(2));
            	SmartDashboard::PutNumber("JAG2 Current", devices.getCANJagCurrent(3));
            	SmartDashboard::PutNumber("i2c Counter", Count);
            	SmartDashboard::PutNumber("Arm Angle", devices.getSensorSignal("armPotHieght"));
            	SmartDashboard::PutNumber("Gyro", devices.getSensorSignal("gyro"));
            	SmartDashboard::PutNumber("Ultrasonic Distance", devices.getSensorSignal("ultrasonic"));
            	
            	inPosition = false;
            	goodAngle = false;
        		
        		if (fired == false)
        		{

        			devices.vacMotor1Control(1.0);
        			devices.vacMotor2Control(1.0);
        			//printf("loop wait\n");
        		}
        		
        		// If we are not in the correct position and angle run the following
        		if (fired == false)
        		{
        			autoMan.correctPosition(4.5f, 0.5f, deltaT); // Distance value in inches @ 3/21 was 34, 6
        			
        			// Angle value in voltage @ 3/21 was 4.2, 0.25
        			autoMan.correctAngle(4.1f, 0.1f, deltaT, !firstCallIndicator);
                    if (!firstCallIndicator)
                    	firstCallIndicator = true;
                    
        		}
        		inPosition = autoMan.isAtCorrectPosition();
        		goodAngle = autoMan.isAtCorrectAngle();
        		
        		//printf("Pos = %i  Ang = %i\n", inPosition, goodAngle);

                //myRobot.SetSafetyEnabled(false);
        		
        		// Check for valid position, valid angle, and that the robot hasn't yet fired
        		if(inPosition == true && goodAngle == true && fired == false)
        		{
        			// Fire
        			Wait(1.0); // Just to allow the robot arm to settle
        			vacMan.shoot(0.2);
        			fired = true;
        		}

                // GUI Manager print statements
                guiMan.print(3, "Arm Pot = %f", devices.getSensorSignal("armPotHieght"));
                guiMan.print(4, "Distance(ft) = %f", devices.getSensorSignal("ultrasonic"));
                guiMan.print(5, "Firing delay(s) = %f", 0.5*inpMan.getJoystickAxis(2, 4) + 0.5);
        		
        		guiMan.update();
        		Wait(0.008);
        		SmartDashboard::PutNumber("Update rate (s)", t1.Get()-prevTime);
        		SmartDashboard::PutNumber("deltaT (s)", deltaT);
        		
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
			SpdP = 3; 
			CmpP = 2;
			guiMan.print(1, "Vacuum OFF");
			Count = 0;
			
			// REMOTE TEST CODE WPT 03/29/14 reset gyro before match
			devices.resetGyro();
			
			// Timer setup
        	//Timer(t1);
        	t1.Reset();
        	t1.Start();
        	prevTime = -1.0;
        	
        	// clear vac status
        	vacStatus = false;
        	vacSpeed = false;
        	
        	firstCallIndicator = false;
        	
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	//set initial prevTime unless currTime is set
            	prevTime = (prevTime < 0) ? t1.Get() : currTime;
            	currTime = t1.Get();
            	deltaT = currTime - prevTime;
            	
            	
            	//----------I2C COMMS---------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// I2C read/write commands          	
            	// Light up LED based on arm position
            	/* green = wire_i2c->Write(3,0x67)
            	 * red = wire_i2c->Write(0,0x72)
            	 * temp = wire_i2c->Write(0,0x74); 
            	 *      = stat = wire_i2c->Transaction(datatosend,0,datarecieved,2);
            	 * RPM = wire_i2c->Write(0,0x73);
            	 *     = stat = wire_i2c->Transaction(datatosend,0,datarecieved,1);*/
            	
            	// I2C Wait to Send function
            	Count++;
            	if (Count >=10)
            	{
            		if (devices.getSensorSignal("armPotHieght") >= 4.0 && devices.getSensorSignal("armPotHieght") <= 4.2)
            		{
            			//printf("stat = %d\n", wire_i2c->Write(3,0x67)); // sets LED green
            			wire_i2c->Write(3,0x67);
            		}
            		else
            		{
            			//printf("stat = %d\n", wire_i2c->Write(0,0x72)); // sets LED red
            			wire_i2c->Write(0,0x72);
            		}
            		wire_i2c->Write(0,0x74); 
            		//wire_i2c->Transaction(datatosend,0,datareceived,2);
            		printf("i2c Temp Connection %i\n", wire_i2c->Transaction(datatosend,0,datareceived,2));
            		SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            		SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            		
            		wire_i2c->Write(0,0x73);
            		//wire_i2c->Transaction(datatosend,0,datareceived,1);
            		printf("i2c Speed Connection %i\n", wire_i2c->Transaction(datatosend,0,datareceived,1));
            		SmartDashboard::PutNumber("Vac Speed", datareceived[0]);
            		Count = 0;
            	}
            	
            	// SmartDashboard calls
            	SmartDashboard::PutNumber("JAG1 Current", devices.getCANJagCurrent(2));
            	SmartDashboard::PutNumber("JAG2 Current", devices.getCANJagCurrent(3));
            	SmartDashboard::PutNumber("i2c Counter", Count);
            	SmartDashboard::PutNumber("Arm Angle", devices.getSensorSignal("armPotHieght"));
            	SmartDashboard::PutNumber("Ultrasonic Distance", devices.getSensorSignal("ultrasonic"));
            	SmartDashboard::PutNumber("Gyro", devices.getSensorSignal("gyro"));
            	
            	// Correct Arm Position Indicator
            	if (4.0 <= devices.getSensorSignal("armPotHieght") && devices.getSensorSignal("armPotHieght") <= 4.2)	
            		SmartDashboard::PutBoolean("Ready to Fire", true);
            	else
            		SmartDashboard::PutBoolean("Ready to Fire", false);
            	
            	// Correct Distance Indicator
            	if (4.7 >= devices.getSensorSignal("ultrasonic") && devices.getSensorSignal("ultrasonic") <= 4.3)
            		SmartDashboard::PutBoolean("Correct Distance", true);
            	else
            		SmartDashboard::PutBoolean("Correct Distance", false);
            	
            	// Check if distance is too far
            	if (4.7 < devices.getSensorSignal("ultrasonic"))
            		SmartDashboard::PutBoolean("Move Closer", true);
            	else
            		SmartDashboard::PutBoolean("Move Closer", false);
            	
            	// Check if distance is too close
            	if (4.3 > devices.getSensorSignal("ultrasonic"))
            		SmartDashboard::PutBoolean("Move Back", true);
            	else
            		SmartDashboard::PutBoolean("Move Back", false);
            	
            	                
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// Update InputManager
            	inpMan.update();
                
        		//----------HIGH LOW SPEED SWITCHING------------------------------
                // read and activate solenoids for the speed shifter.
        		if (inpMan.getJoystickButton(3, 1)) // toggle solenoids with x button / 3 button
        		{
        			devices.setSolenoid(3, false);
        			devices.setSolenoid(4, true);
        			SmartDashboard::PutBoolean("Shifter High", true);
        		}
        		
        		if (inpMan.getJoystickButton(1, 1))
        		{
        			devices.setSolenoid(3, true);
        			devices.setSolenoid(4, false);
        			SmartDashboard::PutBoolean("Shifter High", false);
        		}
        		
        		//----------SET DRIVE MOTOR COMMANDS------------------------------        
                devices.setSpeed(1, inpMan.getDriveCommand("right") * 0.9f);
                devices.setSpeed(2, -inpMan.getDriveCommand("left") * 0.9f);
                
                //----------SHOOTER CODE------------------------------------------
                //----------------------------------------------------------------
                //----------------------------------------------------------------
                
                //----------ARM CONTROL-------------------------------------------
                armMan.moveArm(inpMan.getJoystickAxis(2, 2)); // Forward back axis of joystick
                
                // Push button to automate arm movement
                if (inpMan.getJoystickButton(2, 2) || inpMan.getJoystickButton(2, 4)) 
                {
                	if (inpMan.getJoystickButton(2, 2) == true)
                	{
                		autoMan.correctAngle(4.1f, 0.1f, deltaT, !firstCallIndicator);
                	}
                	else
                	{
                		autoMan.correctAngle(2.25f, 0.1f, deltaT, !firstCallIndicator);
                	}
                	if (!firstCallIndicator)
                	   	firstCallIndicator = true;
                }
                else
                {
                	firstCallIndicator = false;
                }
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                if (inpMan.getJoystickButton(2, 5))
                {
                	devices.vacMotor1Control(0.9);
                	devices.vacMotor2Control(0.9); //0.9 is good
                	guiMan.print(1, "Vacuum ON");
                	vacStatus = true;
                	vacSpeed = false;
                }
                
                if (inpMan.getJoystickButton(2, 6))
                {
                	devices.vacMotor1Control(0.0);
                	devices.vacMotor2Control(0.0);
                	guiMan.print(1, "Vacuum OFF");
                	vacStatus = false;
                	vacSpeed = false;
                }
                
                // button 1 on joystick 2 shoots
                if (inpMan.getJoystickButton(2, 1))
                {                	
                	guiMan.print(2, "Solenoid FIRED");
                	vacMan.shoot(0.2); //0.5*inpMan.getJoystickAxis(2, 4) + 0.5);
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
                if (inpMan.getJoystickButton(1, 6))
                	SpdP = 3;
                if (inpMan.getJoystickButton(1, 7))
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
                guiMan.print(3, "Arm Pot = %f", devices.getSensorSignal("armPotHieght"));
                guiMan.print(4, "Distance(ft) = %f", devices.getSensorSignal("ultrasonic"));
                
                //-----------------UPDATES THE LCD----------------------------
                // Update Driver Station LCD Display
                guiMan.update();                
                Wait(0.01); // wait for a motor update time
                SmartDashboard::PutNumber("Update rate (s)", t1.Get()-prevTime);
                SmartDashboard::PutNumber("deltaT (s)", deltaT);
                        
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
