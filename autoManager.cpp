#include "autoManager.hpp"

FRC::autoManager::autoManager( FRC::deviceManager * devMan, FRC::elevManager * elevMan )
{
	devices = devMan;
	arm = elevMan;
	isGoodPosition = isGoodAngle = false;
}

void FRC::autoManager::correctPosition( float desiredPos, float posThresh )
{
	goodPosition = desiredPos;
	positionTopThreshold = desiredPos + posThresh;
	positionBottomThreshold = desiredPos - posThresh;
	position = devices->getAnalogVoltage(2)*512/5;

	if (position < positionBottomThreshold) // below threshold
	{
		// backwards
		positionSpeed = -0.021 * position + 1; // 0.021 = 1/48
		positionSpeed = (positionSpeed < 1) ? ((positionSpeed > -1) ? positionSpeed : -1) : 1;
		positionSpeed /= 2;
		devices->setSpeed(1, -0.8*positionSpeed);
		devices->setSpeed(2, positionSpeed);
	}
	else if(position > positionTopThreshold) // over threshold
	{
		// forwards
		positionSpeed = 0.021 * (position-72);
	    positionSpeed = (positionSpeed < 1) ? ((positionSpeed > -1) ? positionSpeed : -1) : 1;
	    positionSpeed /= 2;
	    devices->setSpeed(1, 0.8*positionSpeed);
	    devices->setSpeed(2, -positionSpeed);
	}
	else 
	{
		// stopped
		positionSpeed = 0.0f;
		devices->setSpeed(1, positionSpeed);
		devices->setSpeed(2, positionSpeed);
		
		isGoodPosition = true;
	}
	
	
}

void FRC::autoManager::correctAngle( float desiredAngle, float angleThresh )
{
	goodAngle = desiredAngle;
	angleTopThreshold = desiredAngle + angleThresh;
	angleBottomThreshold = desiredAngle - angleThresh;
		
	angle = devices->getAnalogVoltage(3);
	
	if (angle > angleTopThreshold)			// above threshold
	{
		// JASPY COMMENTS: May need to set greater than -1 (-.9???) so it doesn't run past the desired angle
	    arm->moveArm(-1);
	}
	else if(angle < angleBottomThreshold)	// below threshold
	{
		arm->moveArm(1);
	}
	else
	{
		isGoodAngle = true;
	}
	
    //Wait(0.005);
}

bool FRC::autoManager::isAtCorrectPosition()
{
	return isGoodPosition;
}

bool FRC::autoManager::isAtCorrectAngle()
{
	return isGoodAngle;
}
