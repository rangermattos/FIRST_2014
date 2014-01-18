#include "WPILib.h"
#include <iostream>
#include "inputManager.hpp"
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
        DriverStationLCD *LCD;
        DigitalInput Myswitch;
        bool IsArcade, prevArcade;
        Compressor compressor;
        Solenoid sol1, sol2;
public:
        RobotDemo():
        		inpMan(0.1f, FRC::inputManager::MODE_XBOX_TANK),
                m1(1),
                m2(2),
                A1(1),
                LCD(DriverStationLCD::GetInstance()),
                Myswitch(1),
                IsArcade(0),
                prevArcade(0),
                compressor(2,1),
                sol1(1),
                sol2(2)
        {
                //myRobot.SetExpiration(0.1);
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
                //Wait(2.0);                                 //    for 2 seconds
                //myRobot.Drive(0.0, 0.0);         // stop robot
            //printf("value = %d  voltage = %f  distance = %f \n", A1.GetValue(), A1.GetVoltage(), A1.GetVoltage()*512/5);
            distance = A1.GetVoltage()*512/5;
            if (distance < 48)
            {
            	// backwards
            	speed = -0.021 * distance + 1; // 0.021 = 1/48
            	speed = (speed < 1) ? ((speed > -1) ? speed : -1) : 1;
            	speed /= 2;
            	m1.Set(speed);
            	m2.Set(-speed);
            }
            else if(distance > 72 )
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
            	m1.Set(0);
            	m2.Set(0);
            }
            LCD->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", A1.GetVoltage()*512/5);
            LCD->PrintfLine(DriverStationLCD::kUser_Line2, "speed = %f", speed);
            LCD->UpdateLCD();
            Wait(0.005); // wait for a motor update time
        	}
        }

        /**
         * Runs the motors with arcade steering. 
         */
        void OperatorControl()
        {
        	compressor.Start();
                //myRobot.SetSafetyEnabled(false);
                while (IsOperatorControl())
                {
                	
                	//std::cout << "z : " << inpMan.getZ() << "\n";
                	// xbox back button switching drive modes
        			if (inpMan.getButton(7))
        			{
        				while (inpMan.getButton(7))
        				{
        					if (IsArcade == prevArcade)
        					{
        					if(!IsArcade)
        					inpMan.setMode(FRC::inputManager::MODE_XBOX_TANK);
        					else
        					inpMan.setMode(FRC::inputManager::MODE_XBOX_ARCADE);
        					}
        					else
        					{
        					if(IsArcade)
        					inpMan.setMode(FRC::inputManager::MODE_XBOX_TANK);
        					else
        					inpMan.setMode(FRC::inputManager::MODE_XBOX_ARCADE);
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
        			if (inpMan.getButton(5))// solenoid 1
        			{
        				sol1.Set(true);
        			}
        			else
        			{
        				sol1.Set(false);
        			}
        			if (inpMan.getButton(6))//solenoid 2
        			{
        				sol2.Set(true);
        			}
        			else 
        			{
        				sol2.Set(false);
        			}
        			
                		//Update InputManager
                        inpMan.update();
                        
                        //std::cout << "motor 1 : " << inpMan.getMotor(1) << ", Motor 2 : " << inpMan.getMotor(2) << "\n";
                        //Set Motor Commands
                        m1.Set(inpMan.getMotor(1));//Only goes half speed
                        m2.Set(-inpMan.getMotor(2));//Only goes half speed
                        
                        //printf("value = %d  voltage = %f  distance = %f \n", A1.GetValue(), A1.GetVoltage(), A1.GetVoltage()*512/5);
                        //LCD Print Commands
                        /*LCD->PrintfLine(DriverStationLCD::kUser_Line1, "distance = %f", A1.GetVoltage()*512/5);
                        LCD->PrintfLine(DriverStationLCD::kUser_Line2, "motor1 = %f", inpMan.getMotor(1));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line3, "motor2 = %f", -inpMan.getMotor(2));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line4, "button = %d", Myswitch.Get());
                        LCD->PrintfLine(DriverStationLCD::kUser_Line1, "xbox button = %i", inpMan.getButton(8));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line2, "xbox button = %i", inpMan.getButton(9));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line3, "xbox button = %i", inpMan.getButton(10));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line4, "xbox button = %i", inpMan.getButton(6));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line5, "xbox button = %i", inpMan.getButton(7));
                        LCD->PrintfLine(DriverStationLCD::kUser_Line6, "xbox button = %i", inpMan.getButton(0));*/
                        if(IsArcade)
                        	LCD->PrintfLine(DriverStationLCD::kUser_Line1, "ArcadeMode");
                        else
                        	LCD->PrintfLine(DriverStationLCD::kUser_Line1, "TankMode");
                        LCD->PrintfLine(DriverStationLCD::kUser_Line2, "Pressure Switch = %d", compressor.GetPressureSwitchValue());
                        LCD->UpdateLCD();
                        Wait(0.005); // wait for a motor update time
                        
                }
        }
        
        /**
         * Runs during test mode
         */
        void Test() {

        }
};

START_ROBOT_CLASS(RobotDemo);
