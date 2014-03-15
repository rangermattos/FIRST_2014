#ifndef FRC2014_ELEVMANAGER
#define FRC2014_ELEVMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"

namespace FRC
{
	class elevManager
	{
	private:
		FRC::deviceManager * devices;
		float height;
		float threshold;
		
	public:
		elevManager( FRC::deviceManager * devMan );
		void moveArm( float speed );
		void moveArmTo( float value );
		float getHeight();
	};
}

#endif
