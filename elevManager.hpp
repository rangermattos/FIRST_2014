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
		
	public:
		elevManager( FRC::deviceManager * devMan);
		void moveArm( float speed );
		float getHeight();
		
	};
}

#endif
