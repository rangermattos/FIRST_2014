#ifndef FRC2014_INPUTMANAGER
#define FRC2014_INPUTMANAGER
#include "WPILib.h"
namespace FRC
{
        ///@breif a class that will lock the controls, and counter out deadband
        ///
        ///has a noexcept garentee 
        ///Written by Clash(scott), FRC2014
        ///@note gareneteed that values pulled will be unchanging until the next update is called
        ///@warning must call the update function every frame
        class inputManager 
        {
        private:
                float m1, m2;
                unsigned char driveMode;
                Joystick s1, s2;
                float thresh;
        public:
                ///@brief the diffrent driving modes
                ///
                ///choosing diffrent driving modes allows redefinition of how motors return, and how they are updated
                enum
                {
                        MODE_JOY_TANK,
                        MODE_JOY_ARCADE,
                        MODE_XBOX_TANK,
                        MODE_XBOX_ARCADE
                };
                ///@brief constructs the object, default values are most optimal
                inputManager(float threshold = 0.1f, unsigned char mode = MODE_JOY_TANK);
                ///@brief locks in new values, and thresholds the value
                void update();
                ///@brieft get the current drive mode
                unsigned char getMode();
                ///@brief allows swapping of modes while running, or during configuration
                void setMode(unsigned char mode);
                ///@brief get the motor, either side one or side two
                float getMotor(int motor);
        		///@brief get the z-axis data off of the primary joystick
        		float getZ(void);
        		///@brief get joystick button status
        		bool getButton(int button);
        };
}
#endif // FRC2014_INPUTMANAGER
