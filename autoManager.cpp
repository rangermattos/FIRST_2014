#include "autoManager.hpp"

FRC::autoManager::autoManager( FRC::deviceManager * devMan, FRC::elevManager * elevMan )
{
	devices = devMan;
	arm = elevMan;
	isGoodPosition = isGoodAngle = false;
}

void FRC::autoManager::correctPosition( float desiredPos, float posThresh)
{
	goodPosition = desiredPos;
	positionTopThreshold = desiredPos + posThresh;
	positionBottomThreshold = desiredPos - posThresh;
	position = devices->getAnalogVoltage(2)*512/5; // Distance sensor converted to inches

	if (position < positionBottomThreshold) // below threshold
	{
		// Movement backwards required
		// Set speed equal to the ratio of how close we are from target
		positionSpeed = -0.021 * position + 1; // 0.021 = 1/48
		positionSpeed = (positionSpeed < 1) ? ((positionSpeed > -1) ? positionSpeed : -1) : 1;
		positionSpeed /= 2;
		devices->setSpeed(1, -0.8*positionSpeed); // speed balance manually adjusted
		devices->setSpeed(2, positionSpeed);
		
		isGoodPosition = false;
	}
	else if(position > positionTopThreshold) // over threshold
	{
		// Movement forward required
		// Set speed equal to the ratio of how close we are from target
		positionSpeed = 0.021 * (position-72);
	    positionSpeed = (positionSpeed < 1) ? ((positionSpeed > -1) ? positionSpeed : -1) : 1;
	    positionSpeed /= 2;
	    devices->setSpeed(1, 0.8*positionSpeed); // speed balance manually adjusted
	    devices->setSpeed(2, -positionSpeed);
	    
	    isGoodPosition = false;
	}
	else 
	{
		// Robot is within the correct position threshold
		positionSpeed = 0.0f;
		devices->setSpeed(1, positionSpeed);
		devices->setSpeed(2, positionSpeed);
		
		isGoodPosition = true;
	}

}

void FRC::autoManager::correctAngle( float desiredAngle, float angleThresh)
{
	goodAngle = desiredAngle;
	angleTopThreshold = desiredAngle + angleThresh;
	angleBottomThreshold = desiredAngle - angleThresh;
		
	angle = devices->getAnalogVoltage(3);
	//printf("Auton angle %f Top = %f Bot = %f\n", angle, angleTopThreshold, angleBottomThreshold);
	
	if (angle > angleTopThreshold)
	{
	    // Above threshold, move arm down
		arm->moveArm(-0.4);
		isGoodAngle = false;
	}
	else if(angle < angleBottomThreshold)
	{
		// Below threshold, move arm up
		arm->moveArm(0.4);
		isGoodAngle = false;
	}
	else
	{
		arm->moveArm(0.0);
		isGoodAngle = true;
	}
	
}

bool FRC::autoManager::isAtCorrectPosition()
{
	return isGoodPosition;
}

bool FRC::autoManager::isAtCorrectAngle()
{
	return isGoodAngle;
}
