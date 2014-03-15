#ifndef FRC2014_AUTOMANAGER
#define FRC2014_AUTOMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"
#include "elevManager.hpp"

namespace FRC
{
	class autoManager
	{
	private:
		FRC::deviceManager * devices;
		FRC::elevManager * arm;
		float angle;					// angle of arm
		float position;					// distance to wall, in inches
		float positionSpeed;
		float goodAngle, angleTopThreshold, angleBottomThreshold; 
		float goodPosition, positionTopThreshold, positionBottomThreshold;
		bool isGoodPosition, isGoodAngle;
		
	public:
		autoManager( FRC::deviceManager * devMan, FRC::elevManager * elevMan );
		void correctPosition( float desiredPos, float posThresh );
		void correctAngle( float desiredAngle, float angleThresh );
		bool isAtCorrectPosition();
		bool isAtCorrectAngle();
	};
}

#endif
