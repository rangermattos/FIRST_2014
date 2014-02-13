#ifndef FRC2014_DEVICEMANAGER
#define FRC2014_DEVICEMANAGER
#include "WPILib.h"
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
        	Victor drivemotor1, drivemotor2, elevMotor1, elevMotor2;
        	AnalogChannel elevPotHeight, gyro, ultrasonic, armPotHeight;
        	DigitalInput elevhomeswitch, armhomeswitch;
        	Compressor compressor;
        	Solenoid armfire, lowsp, highsp; // sol1 = pulls out shooter piston, sol 2 = pulls in shooter piston
        	bool pistonExtended;
        	CANJaguar *vacMotor1, *vacMotor2, *armMotor; 
        	//CANJaguar *can2;
        	//CANJaguar *can3;
        	//Encoder encoder1, encoder2;
        	//float speed;
        	//float distance;
        	//float motorcommand1, motorcommand2;
        	
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
        	void setControlMode(int CanJag, int mode);
        	void setPositionReference(int CANJag, int reference);
        	double PotPosition();
        	float getAnalogVoltage( int analog );
        	float drivemotor1Control(float speed);
        	float drivemotor2Control(float speed);
        	float elevMotor1Control(float speed);
        	float elevMotor2Control(float speed);
        	float vacMotor1Control(float speed);
        	float vacMotor2Control(float speed);
        	float armMotorControl(float speed);
        	int getHomeSwitch(int location);
        };
}
#endif // FRC2014_GUIMANAGER
