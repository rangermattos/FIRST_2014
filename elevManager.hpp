#ifndef FRC2014_ELEVMANAGER
#define FRC2014_ELEVMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"
#include "guiManager.hpp"

namespace FRC
{
	class elevManager
	{
	private:
		FRC::deviceManager * devices;
		FRC::guiManager * gMan;
		float height;
		
	public:
		elevManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan );
		void moveElevator( float speed );
		void moveArm( float speed );
		float getHeight();
		
	};
}

#endif
