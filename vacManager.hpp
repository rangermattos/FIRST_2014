#ifndef FRC2014_VACMANAGER
#define FRC2014_VACMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"
#include "guiManager.hpp"

namespace FRC
{
	class vacManager
	{
	private:
		FRC::deviceManager * devices;
		FRC::guiManager * gMan;
		float currentThreshold;
		float CANJagCurrent1;
		float CANJagCurrent2;
		
	public:
		vacManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan );
		void vacuum();
		void shoot();
	};
}
#endif
