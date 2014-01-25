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
        	Talon talon1, talon2;
        	AnalogChannel ultrasonic;
        	DigitalInput switch1;
        	Compressor compressor;
        	Solenoid sol1, sol2; // sol1 = pulls out, sol 2 = pulls in
        	CANJaguar *can;     	
        	bool pistonExtended;
        	Encoder encoder;
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
        	int getEncoderValue();
        	void startEncoder();
        	void startCompressor();
        };
}
#endif // FRC2014_GUIMANAGER
