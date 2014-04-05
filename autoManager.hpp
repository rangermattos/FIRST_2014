#ifndef FRC2014_AUTOMANAGER
#define FRC2014_AUTOMANAGER
#include "WPILib.h"
#include "deviceManager.hpp"
#include "armManager.hpp"
#include "commonFunctions.hpp"

namespace FRC
{
	class autoManager
	{
	private:
		FRC::deviceManager * devices;
		FRC::armManager * arm;
		float angle;					// angle of arm
		float angleSpeed;
		float position;					// distance to wall, in inches
		float positionSpeed;
		float goodAngle, angleTopThreshold, angleBottomThreshold; 
		float goodPosition, positionTopThreshold, positionBottomThreshold;
		float baseAngleTopThreshold, baseAngleBottomThreshold;
		float gyroCorrection, prevGyro, currGyro;
		float prevAngle;
		float proportionalErrorPos, integralErrorPos, proportionalErrorAngle, integralErrorAngle;
		float pGainPos, iGainPos, pGainAngle, iGainAngle;
		float correctionCommandPos, correctionCommandAngle;
		bool isGoodPosition, isGoodAngle;
		
	public:
		autoManager( FRC::deviceManager * devMan, FRC::armManager * armMan );
		void correctPosition( float desiredPos, float posThresh, double deltaT);
		void correctArmAngle( float desiredAngle, float angleThresh, double deltaT, bool firstCall);
		void correctDriveAngle( float desiredAngle, float angleThresh, double deltaT );
		bool isAtCorrectPosition();
		bool isAtCorrectAngle();
	};
}

#endif
