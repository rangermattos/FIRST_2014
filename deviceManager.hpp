#ifndef FRC2014_DEVICEMANAGER
#define FRC2014_DEVICEMANAGER
#include "WPILib.h"
namespace FRC
{
        ///@breif a class that will lock the controls, and counter out deadband
        ///
        ///has a noexcept garentee 
        ///@note gareneteed that values pulled will be unchanging until the next update is called
        ///@warning must call the update function every frame
        class deviceManager 
        {
        private:
        	///
        	
        public:
        	///@brief the diffrent driving modes
        	///
        	///@brief constructs the object, default values are most optimal
        	deviceManager();
        	
        };
}
#endif // FRC2014_GUIMANAGER
