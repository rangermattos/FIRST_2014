#ifndef FRC2014_GUIMANAGER
#define FRC2014_GUIMANAGER
#include "WPILib.h"
namespace FRC
{
        ///@brief a class that will lock the controls, and counter out deadband
        ///
        ///has a noexcept guarentee 
        ///@note guaranteed that values pulled will be unchanging until the next update is called
        ///@warning must call the update function every frame
        class guiManager 
        {
        private:
                ///
        	DriverStationLCD *display;
        	
        public:
            ///@brief the diffrent driving modes
            ///
        	///@brief constructs the object, default values are most optimal
        	guiManager();
        	void print(int line, const char *message, ...);
        	void update();
        	void clear();
        	
        };
}
#endif // FRC2014_GUIMANAGER
