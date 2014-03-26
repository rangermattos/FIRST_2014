#include "autoManager.hpp"

FRC::autoManager::autoManager( FRC::deviceManager * devMan, FRC::armManager * armMan )
{
	devices = devMan;
	arm = armMan;
	isGoodPosition = isGoodAngle = false;
	pGain = 1/45;
	iGain = 0.1 * pGain;
}

void FRC::autoManager::correctPosition( float desiredPos, float posThresh, double deltaT)
{
	goodPosition = desiredPos;
	positionTopThreshold = desiredPos + posThresh;
	positionBottomThreshold = desiredPos - posThresh;
	position = devices->getSensorSignal("ultrasonic")*512/5; // Distance sensor converted to inches

	if(position > positionTopThreshold) // over threshold
	{
		// Movement with Gyro
		prevGyro = currGyro || devices->getSensorSignal("gyro");
		currGyro = devices->getSensorSignal("gyro");
		//gyroCorrection = 2*devices->getSensorSignal("gyro")/90; //gyro correction
		proportionalError = currGyro - prevGyro;
		integralError = 0.5 * deltaT * (currGyro + prevGyro);
		correctionCommand = pGain * proportionalError + iGain * integralError;
		if (proportionalError > 0)
		{
			devices->setSpeed(1, -0.5 + 0.5 * correctionCommand);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.4);//*positionSpeed);
		}
		else if (proportionalError < 0)
		{
			devices->setSpeed(1, -0.5);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.4 + 0.4 * correctionCommand);//*positionSpeed);
		}
		else
		{
			devices->setSpeed(1, -0.5);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.4);//*positionSpeed);
		}
	
	    
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

void FRC::autoManager::correctAngle( float desiredAngle, float angleThresh, double deltaT)
{
	goodAngle = desiredAngle;
	angleTopThreshold = desiredAngle + angleThresh;
	angleBottomThreshold = desiredAngle - angleThresh;
		
	angle = devices->getSensorSignal("armPotHieght");
	//printf("Auton angle %f Top = %f Bot = %f\n", angle, angleTopThreshold, angleBottomThreshold);
	
	if (angle > angleTopThreshold)
	{
	    // Above threshold, move arm down
		arm->moveArm(-0.6);
		isGoodAngle = false;
	}
	else if(angle < angleBottomThreshold)
	{
		// Below threshold, move arm up
		arm->moveArm(0.6);
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
