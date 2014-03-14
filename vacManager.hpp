#ifndef FRC2014_VACMANAGER
#define FRC2014_VACMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"

namespace FRC
{
	class vacManager
	{
	private:
		FRC::deviceManager * devices;
		float currentThreshold;
		bool armReady;
		
	public:
		vacManager( FRC::deviceManager * devMan);
		void vacuum();
		void shoot(float delay);
	};
}
#endif
