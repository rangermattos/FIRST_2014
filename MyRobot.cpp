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
public:
        RobotDemo():
                m1(1),
                m2(2),
                inpMan()
        {
                //myRobot.SetExpiration(0.1);
        }

        /**
         * Drive left & right motors for 2 seconds then stop
         */
        void Autonomous()
        {
                //myRobot.SetSafetyEnabled(false);
                //myRobot.Drive(-0.5, 0.0);         // drive forwards half speed
                Wait(2.0);                                 //    for 2 seconds
                //myRobot.Drive(0.0, 0.0);         // stop robot
        }

        /**
         * Runs the motors with arcade steering. 
         */
        void OperatorControl()
        {
                //myRobot.SetSafetyEnabled(false);
                while (IsOperatorControl())
                {
                        inpMan.update();
                        std::cout << "motor 1 : " << inpMan.getMotor(1) << ", Motor 2 : " << inpMan.getMotor(2) << "\n";
                        m1.Set(inpMan.getMotor(1) / 2.0f);
                        m2.Set(-inpMan.getMotor(2) / 2.0f);
                        Wait(0.005);                                // wait for a motor update time
                }
        }
        
        /**
         * Runs during test mode
         */
        void Test() {

        }
};

START_ROBOT_CLASS(RobotDemo);
