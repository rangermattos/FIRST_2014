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
public:
        RobotDemo():
        		inpMan(0.1f, FRC::inputManager::MODE_XBOX_TANK),
                m1(1),
                m2(2),
                A1(1),
                LCD(DriverStationLCD::GetInstance())
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
                //myRobot.SetSafetyEnabled(false);
                while (IsOperatorControl())
                {
                	//std::cout << "z : " << inpMan.getZ() << "\n";
        			/*if (inpMan.getZ() > 0.0f)
        			{
        				inpMan.setMode(FRC::inputManager::MODE_JOY_ARCADE);
        			}
        			else
        			{
        				inpMan.setMode(FRC::inputManager::MODE_JOY_TANK);
        			}*/
                        inpMan.update();
                        //std::cout << "motor 1 : " << inpMan.getMotor(1) << ", Motor 2 : " << inpMan.getMotor(2) << "\n";
                        m1.Set(inpMan.getMotor(1));
                        m2.Set(-inpMan.getMotor(2));
                        printf("value = %d  voltage = %f  distance = %f \n", A1.GetValue(), A1.GetVoltage(), A1.GetVoltage()*512/5);
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
