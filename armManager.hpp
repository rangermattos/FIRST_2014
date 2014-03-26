#ifndef FRC2014_ARMVMANAGER
#define FRC2014_ARMVMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"

namespace FRC
{
	class armManager
	{
	private:
		FRC::deviceManager * devices;
		float height;
		float threshold;
		
	public:
		armManager( FRC::deviceManager * devMan );
		void moveArm( float speed );
		void moveArmTo( float value );
		float getHeight();
	};
}

#endif
