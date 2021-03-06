#ifndef FRC2014_INPUTMANAGER
#define FRC2014_INPUTMANAGER
#include "WPILib.h"
#include <iostream>
#include <string>
#include "commonFunctions.hpp"

namespace FRC
{
        ///@brief a class that will lock the controls, and counter out deadband
        ///
        ///has a noexcept guarantee 
        ///Written by Clash(scott), FRC2014
        ///@note guaranteed that values pulled will be unchanging until the next update is called
        ///@warning must call the update function every frame
	class inputManager
	{
		private:
			float commandRight, commandLeft;
			unsigned char driveMode;
			Joystick s1, s2, s3;
			float thresh;

        public:
			///@brief the different driving modes
			///
			///choosing different driving modes allows redefinition of how motors return, and how they are updated
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
			
			float rateLimit( float current, float prev, float rate );
			///@brief returns current drive mode
			unsigned char getMode();
			///@brief allows swapping of modes while running, or during configuration
			void setDriveMode(unsigned char mode);
			///@brief get the motor, either side one or side two
			float getDriveCommand(string side);
			///@brief get the z-axis data off of the primary joystick
			bool getJoystickButton(int joystick, int button);
			float getJoystickAxis(int joystick, int axis);
	};
}
#endif // FRC2014_INPUTMANAGER
