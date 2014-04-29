#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
#include "deviceManager.hpp"
#include "guiManager.hpp"
#include "vacManager.hpp"
#include "armManager.hpp"
#include "autoManager.hpp"
#include "netManager.hpp"
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
	Timer *t1; // timer for loop
	double prevTime, currTime, deltaT; // stores previous, current, change in time for loop
	bool firstCallIndicatorAngle, firstCallIndicatorDrive; // used to clear the error integration function for the automative moves
	//bool firstAuton;
	//DriverStationLCD *display;
	//DriverStationEnhancedIO *displayenhanced;
	//netManager netMan;
	CANJaguar *vacMotor1;
	CANJaguar *vacMotor2;
	
public:
	RobotDemo():
		inpMan(0.1f, 								// threshold of 0.1f
				FRC::inputManager::MODE_JOY_TANK),	// joystick tank mode to start
		devices(),
		guiMan(),
		vacMan(&devices),
		armMan(&devices),
		autoMan(&devices, &armMan),
		drive(DriverStation::GetInstance()),
		
		//IsArcade(0), 								// not arcade to start
		//prevArcade(0), 								// previously not arcade
		//inPosition(0),								// not yet in position
		//goodAngle(0),								// not yet at correct angle
		//t1(),
		
		//display(DriverStationLCD::GetInstance())
		//netMan(),
		
		firstCallIndicatorAngle(0),
		firstCallIndicatorDrive(0)
		{
			//myRobot.SetExpiration(0.1);
			devices.startCompressor();
			wire_i2c = DigitalModule::GetInstance(1)->GetI2C(4);
			//netMan.spawnServer();
			//netMan.connect("10.51.48.50", 1180);
				
			t1 = new Timer();
			vacMotor1 = new CANJaguar(3);
			vacMotor2 = new CANJaguar(4);
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

        	printf("Start of Autonomous\r\n");
 
        	//devices.vacMotor1Control(1.0);
        	//devices.vacMotor2Control(1.0);
        	vacMotor1->Set(1.0);
        	vacMotor2->Set(1.0);
        	
        	Wait(0.005);
        	vacMotor1->Set(1.0);
        	vacMotor2->Set(1.0);
        	
			// sync ultrasonics
			//devices.setUltrasonicDiode(1);
			//Wait(0.00005);
			//devices.setUltrasonicDiode(0);
        	
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
			
			// temp wait statement
			printf("before wait\r\n");
			Wait(2.0);
			printf("after wait\r\n");
			
			// reset gyro before match
			devices.resetGyro();
			
			// initialize timer
			t1->Reset();
			t1->Start();
			prevTime = -1.0;
			
        	while (IsAutonomous())
        	{

        		// set initial prevTime unless currTime is set
        		prevTime = (prevTime < 0) ? t1->Get() : currTime;
    			currTime = t1->Get();
    			deltaT = currTime - prevTime;
        		
        		// SMART DASHBOARD COMMANDS
            	SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            	SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            	SmartDashboard::PutNumber("JAG1 Current", devices.getCANJagCurrent(2));
            	SmartDashboard::PutNumber("JAG2 Current", devices.getCANJagCurrent(3));
            	SmartDashboard::PutNumber("i2c Counter", Count);
            	SmartDashboard::PutNumber("Arm Angle", devices.getSensorSignal("armPotHieght"));
            	SmartDashboard::PutNumber("Gyro", devices.getSensorSignal("gyro"));
            	SmartDashboard::PutNumber("Ultrasonic Right", devices.getSensorSignal("ultrasonic right"));
            	SmartDashboard::PutNumber("Ultrasonic Left", devices.getSensorSignal("ultrasonic left"));
            	SmartDashboard::PutNumber("Angle to Wall", devices.angleToWall());
            	//SmartDashboard::PutBoolean("Camera Target Status", netMan.getGoalIsHot() == 1);
            	
            	inPosition = false;
            	goodAngle = false;
        		
        		if (fired == false)
        		{
                	vacMotor1->Set(1.0);
                	vacMotor2->Set(1.0);
        			
        			//devices.vacMotor1Control(1.0);
        			//devices.vacMotor2Control(1.0);
        			//printf("loop wait\n");
        		}
        		
        		// If we are not in the correct position and angle run the following
        		if (fired == false)
        		{
        			autoMan.correctPosition(6.35f, 0.8f, deltaT); // Distance value in inches @ 6.35, 0.8
        			
        			// Angle value in voltage @ 3/21 was 4.2, 0.25
        			autoMan.correctArmAngle(3.70f, 0.15f, deltaT, !firstCallIndicatorAngle); // was 3.75, 0.05
                    if (!firstCallIndicatorAngle)
                    	firstCallIndicatorAngle = true;
                    
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
        			vacMotor1->Set(0.0);
        			vacMotor2->Set(0.0);
        			
        			Wait(1.5); //1.0
        						
        			devices.setSolenoid(1, false);
        			devices.setSolenoid(2, true);
        				
        			fired = true;
        		}

                // GUI Manager print statements
                guiMan.print(3, "Arm Pot = %f", devices.getSensorSignal("armPotHieght"));
                guiMan.print(4, "Distance(ft) = %f", devices.getSensorSignal("ultrasonic right"));
                guiMan.print(5, "Firing delay(s) = %f", 0.5*inpMan.getJoystickAxis(2, 4) + 0.5);
        		
        		guiMan.update();
        		Wait(0.008);
        		SmartDashboard::PutNumber("Update rate (s)", t1->Get()-prevTime);
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
        	vacMotor1->Set(0.0);
        	vacMotor2->Set(0.0);
        	//devices.vacMotor1Control(0.0);
        	//devices.vacMotor2Control(0.0);
        	
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
			
			// sync ultrasonics
			//devices.setUltrasonicDiode(1);
			//Wait(0.00005);
			//devices.setUltrasonicDiode(0);
			
			// Timer setup
        	//Timer(t1);
        	t1->Reset();
        	t1->Start();
        	prevTime = -1.0;
        	
        	// clear vac status
        	vacStatus = false;
        	vacSpeed = false;
        	
        	firstCallIndicatorAngle = false;
        	
            while (IsOperatorControl())
            {
            	//----------START UP----------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	//set initial prevTime unless currTime is set
            	prevTime = (prevTime < 0) ? t1->Get() : currTime;
            	currTime = t1->Get();
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
            	if (Count == 10)
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
            	}
            	if (Count == 20)
            	{
            		wire_i2c->Write(0,0x74); 
            		//wire_i2c->Transaction(datatosend,0,datareceived,2);
            		printf("i2c Temp Connection %i\n", wire_i2c->Transaction(datatosend,0,datareceived,2));
            		SmartDashboard::PutNumber("Temp1 F", datareceived[0]);
            		SmartDashboard::PutNumber("Temp2 F", datareceived[1]);
            	}
            	if (Count == 30)
            	{          
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
            	SmartDashboard::PutNumber("Ultrasonic Right", devices.getSensorSignal("ultrasonic right"));
            	SmartDashboard::PutNumber("Gyro", devices.getSensorSignal("gyro"));
            	SmartDashboard::PutNumber("Ultrasonic Left", devices.getSensorSignal("ultrasonic left"));
            	SmartDashboard::PutNumber("Angle to Wall", devices.angleToWall());
            	//SmartDashboard::PutBoolean("Camera Target Status", netMan.getGoalIsHot() == 1);
            	
            	// Correct Arm Position Indicator
            	if (4.0 <= devices.getSensorSignal("armPotHieght") && devices.getSensorSignal("armPotHieght") <= 4.2)	
            		SmartDashboard::PutBoolean("Ready to Fire", true);
            	else
            		SmartDashboard::PutBoolean("Ready to Fire", false);
            	
            	// Correct Distance Indicator
            	if (4.7 >= devices.getSensorSignal("ultrasonic right") && devices.getSensorSignal("ultrasonic right") <= 4.3)
            		SmartDashboard::PutBoolean("Correct Distance", true);
            	else
            		SmartDashboard::PutBoolean("Correct Distance", false);
            	
            	// Check if distance is too far
            	if (4.7 < devices.getSensorSignal("ultrasonic right"))
            		SmartDashboard::PutBoolean("Move Closer", true);
            	else
            		SmartDashboard::PutBoolean("Move Closer", false);
            	
            	// Check if distance is too close
            	if (4.3 > devices.getSensorSignal("ultrasonic right"))
            		SmartDashboard::PutBoolean("Move Back", true);
            	else
            		SmartDashboard::PutBoolean("Move Back", false);
            	
            	                
            	//----------DRIVER CODE-------------------------------------------
            	//----------------------------------------------------------------
            	//----------------------------------------------------------------
            	
            	// Update InputManager
            	inpMan.update();
            	
            	if (drive->GetDigitalIn(1) == 0)
            	{
            		devices.stopCompressor();
            	}
                
            	else
            	{
            		devices.startCompressor();
            	}
            	
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
        		devices.setSpeed(1, inpMan.getDriveCommand("right") * 1.0f);
                devices.setSpeed(2, -inpMan.getDriveCommand("left") * 1.0f);
                
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
                		autoMan.correctArmAngle(3.75f, 0.025f, deltaT, !firstCallIndicatorAngle);
                	}
                	else
                	{
                		autoMan.correctArmAngle(2.3f, 0.025f, deltaT, !firstCallIndicatorAngle);
                	}
                	if (!firstCallIndicatorAngle)
                	   	firstCallIndicatorAngle = true;
                }
                else
                {
                	firstCallIndicatorAngle = false;
                }
                
                //----------MANAGE VACUUM SHOOTING--------------------------------
                if (inpMan.getJoystickButton(2, 5))
                {
                	vacMotor1->Set(0.9);
                	vacMotor2->Set(0.9);
                	//devices.vacMotor1Control(0.9);
                	//devices.vacMotor2Control(0.9); //0.9 is good
                	guiMan.print(1, "Vacuum ON");
                	vacStatus = true;
                	vacSpeed = false;
                }
                
                if (inpMan.getJoystickButton(2, 6))
                {
                	vacMotor1->Set(0.0);
                	vacMotor2->Set(0.0);
                	//devices.vacMotor1Control(0.0);
                	//devices.vacMotor2Control(0.0);
                	guiMan.print(1, "Vacuum OFF");
                	vacStatus = false;
                	vacSpeed = false;
                }
                
                // button 1 on joystick 2 shoots
                if (inpMan.getJoystickButton(2, 1))
                {                	
                	guiMan.print(2, "Solenoid FIRED");
                	vacMan.shoot(0.2); //0.5*inpMan.getJoystickAxis(2, 4) + 0.5);
                	vacMotor1->Set(0.0);
                	vacMotor2->Set(0.0);
                	
                	Wait(1.5); //1.0
                	        						
                	devices.setSolenoid(1, false);
                	devices.setSolenoid(2, true);
                	
                	guiMan.print(2, "Solenoid RESET");
                	vacStatus = false;
                	vacSpeed = false;
                }
                
                // Indicate when the vacuum is up to speed after being activated
                if (vacStatus && vacMotor1->GetOutputCurrent() >= 19.0)
                {
                	vacSpeed = true;
                }
                
                // Once vacuum at speed and active then check that it's pulled suction
                if (vacStatus && vacSpeed && vacMotor1->GetOutputCurrent() <= 15.0)
                {
                	vacMotor1->Set(1.0);
                	vacMotor2->Set(1.0);
                	//devices.vacMotor1Control(1.0);
                	//devices.vacMotor2Control(1.0);
                }
                
                if (inpMan.getJoystickButton(1, 2))
                {
                	autoMan.correctDriveAngle(0.0, 1.0, deltaT, !firstCallIndicatorDrive);
            	
                	if (!firstCallIndicatorDrive)
                		firstCallIndicatorDrive = true;
                }
                else
                {
                	firstCallIndicatorDrive = false;
                }
                             
                if (inpMan.getJoystickButton(3, 2))
                {
                	autoMan.correctPosition(4.5f, 0.8f, deltaT);
                	
                	
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
                guiMan.print(4, "Distance(ft) = %f", devices.getSensorSignal("ultrasonic right"));
                guiMan.print(5, "Joystick 2 = %f", inpMan.getJoystickAxis(2,2));
                
                //-----------------UPDATES THE LCD----------------------------
                // Update Driver Station LCD Display
                guiMan.update();                
                Wait(0.01); // wait for a motor update time
                SmartDashboard::PutNumber("Update rate (s)", t1->Get()-prevTime);
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
