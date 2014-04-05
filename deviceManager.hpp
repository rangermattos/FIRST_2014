#ifndef FRC2014_DEVICEMANAGER
#define FRC2014_DEVICEMANAGER
#include "WPILib.h"
#include <cmath>

namespace FRC
{
        ///@brief a class that will lock the controls, and counter out deadband
        ///
        ///has a noexcept guarantee
        ///@note guaranteed that values pulled will be unchanging until the next update is called
        ///@warning must call the update function every frame
        class deviceManager 
        {
        private:
        	Victor drivemotor1, drivemotor2;
        	//Talon vacMotor1, vacMotor2;
        	Gyro gyro;
        	AnalogChannel ultrasonicRight, ultrasonicLeft, armPotHeight;
        	DigitalInput armhomeswitch;
        	Compressor compressor;
        	Solenoid armfire, armrelief, lowsp, highsp; // sol1 = pulls out shooter piston, sol 2 = pulls in shooter piston
        	bool pistonExtended;
        	CANJaguar *armMotor; 
        	CANJaguar *vacMotor1;
        	CANJaguar *vacMotor2;

        	
        public:        	
        	///@brief the diffrent driving modes
        	///
        	///@brief constructs the object, default values are most optimal
        	deviceManager();
        	void setSpeed(int motor, float speed);
        	void setSolenoid(int sol, bool set);
        	bool isPistonExtended();
        	void togglePistonExtended();
        	//int getEncoderValue(int encoder);
        	//void startEncoder();
        	void startCompressor();
        	void stopCompressor();
        	float getCANJagCurrent(int CANJag);
        	void setCANJag(int CANJag, float value);
        	double PotPosition();
        	float getSensorSignal(string analog );
        	void drivemotor1Control(float speed);
        	void drivemotor2Control(float speed);
        	void vacMotor1Control(float speed);
        	void vacMotor2Control(float speed);
        	void armMotorControl(float speed);
        	int getHomeSwitch(void);
        	void resetGyro(void);
        	float averageDistance();
        	float angleToWall();
        };
}
#endif // FRC2014_GUIMANAGER
