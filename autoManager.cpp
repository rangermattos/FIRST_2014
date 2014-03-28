#include "autoManager.hpp"

FRC::autoManager::autoManager( FRC::deviceManager * devMan, FRC::armManager * armMan )
{
	devices = devMan;
	arm = armMan;
	isGoodPosition = isGoodAngle = false;
	pGainPos = 1/45;
	iGainPos = 0.1 * pGainPos;
	pGainAngle = 0.1; 
	iGainAngle = 0.001; // change value at some point
}

void FRC::autoManager::correctPosition( float desiredPos, float posThresh, double deltaT)
{
	goodPosition = desiredPos;
	positionTopThreshold = desiredPos + posThresh;
	positionBottomThreshold = desiredPos - posThresh;
	position = devices->getSensorSignal("ultrasonic")*512/5; // Distance sensor converted to inches

	if(position > positionTopThreshold || position < positionBottomThreshold) // over or under threshold
	{
		// Movement with Gyro
		prevGyro = currGyro || devices->getSensorSignal("gyro");
		currGyro = devices->getSensorSignal("gyro");
		//gyroCorrection = 2*devices->getSensorSignal("gyro")/90; //gyro correction
		proportionalErrorPos = currGyro - prevGyro;
		
		//use trapezoid to calculate integral
		integralErrorPos = 0.5 * deltaT * (currGyro + prevGyro);
		
		correctionCommandPos = pGainPos * proportionalErrorPos + iGainPos * integralErrorPos;
		
		if (proportionalErrorPos > 0)
		{
			devices->setSpeed(1, -0.5 + 0.5 * correctionCommandPos);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.4);//*positionSpeed);
		}
		else if (proportionalErrorPos < 0)
		{
			devices->setSpeed(1, -0.5);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.4 + 0.4 * correctionCommandPos);//*positionSpeed);
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
	
	if(angle > angleTopThreshold || angle < angleBottomThreshold) // over or under threshold
	{
		// angle movement
		prevAngle = angle;
		proportionalErrorAngle = angle - goodAngle;
		
		// use trapezoid to calculate integral
		integralErrorAngle = 0.5 * deltaT * (angle + prevAngle);
		
		correctionCommandAngle = pGainAngle * proportionalErrorAngle + iGainAngle * integralErrorAngle;
		
		arm->moveArm(correctionCommandAngle);	
	    
	    isGoodAngle = false;
	}
	else 
	{
		// Robot is within the correct angle threshold
		angleSpeed = 0.0f;
		arm->moveArm(angleSpeed);
		
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
