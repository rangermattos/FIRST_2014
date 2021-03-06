#include "autoManager.hpp"

FRC::autoManager::autoManager( FRC::deviceManager * devMan, FRC::armManager * armMan )
{
	devices = devMan;
	arm = armMan;
	isGoodPosition = isGoodAngle = false;
	pGainPos = 0.5; // was 1.0
	iGainPos = 0.1 * pGainPos;
	pGainAngle = 1.5; 
	iGainAngle = 0.1; // change value at some point
}

void FRC::autoManager::correctPosition( float desiredPos, float posThresh, double deltaT)
{
	goodPosition = desiredPos;
	positionTopThreshold = desiredPos + posThresh;
	positionBottomThreshold = desiredPos - posThresh;
	position = devices->getSensorSignal("ultrasonic"); // Distance sensor converted to inches

	if(position > positionTopThreshold || position < positionBottomThreshold) // over or under threshold
	{
		// Movement with Gyro
		prevGyro = currGyro || devices->getSensorSignal("gyro");
		currGyro = devices->getSensorSignal("gyro");
		//gyroCorrection = 2*devices->getSensorSignal("gyro")/90; //gyro correction
		proportionalErrorPos = currGyro - prevGyro;
		
		//use trapezoid to calculate integral
		integralErrorPos = 0.5 * deltaT * (currGyro + prevGyro);
		
		correctionCommandPos = (pGainPos * proportionalErrorPos + iGainPos * integralErrorPos);
		correctionCommandPos= clamp(-1, 1, correctionCommandPos);
		
		// print for debugging
		//printf("correctionCommandPos: %f\n", correctionCommandPos);
		
		if (proportionalErrorPos > 0)
		{
			devices->setSpeed(1, -0.75 + 0.75 * correctionCommandPos);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.6);//*positionSpeed);
		}
		else if (proportionalErrorPos < 0)
		{
			devices->setSpeed(1, -0.75);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.6 + 0.6 * correctionCommandPos);//*positionSpeed);
		}
		else
		{
			devices->setSpeed(1, -0.75);//*positionSpeed); // speed balance manually adjusted
			devices->setSpeed(2, 0.6);//*positionSpeed);
		}
	
	    
	    isGoodPosition = false;
	}
	else 
	{
		// Robot is within the correct position threshold
		positionSpeed = 0.0f;
		//printf("correctionCommandPos: %f\n", positionSpeed);
		devices->setSpeed(1, positionSpeed);
		devices->setSpeed(2, positionSpeed);
		
		isGoodPosition = true;
	}

}

void FRC::autoManager::correctAngle( float desiredAngle, float angleThresh, double deltaT, bool firstCall)
{
	if (firstCall)
		prevAngle = devices->getSensorSignal("armPotHieght");
	else 
		prevAngle = angle;
	
	goodAngle = desiredAngle;
	angleTopThreshold = desiredAngle + angleThresh;
	angleBottomThreshold = desiredAngle - angleThresh;	
	
	angle = devices->getSensorSignal("armPotHieght");
	//printf("Auton angle %f Top = %f Bot = %f\n", angle, angleTopThreshold, angleBottomThreshold);
	
	if(angle > angleTopThreshold || angle < angleBottomThreshold) // over or under threshold
	{
		// angle movement
		proportionalErrorAngle = goodAngle - angle;
		
		// use trapezoid to calculate integral
		integralErrorAngle = 0.5 * deltaT * (angle + prevAngle);
		
		correctionCommandAngle = (pGainAngle * proportionalErrorAngle + iGainAngle * integralErrorAngle);
		correctionCommandAngle = clamp(-1, 1, correctionCommandAngle);
		
		// print for debugging
		//printf("correctionCommandAngle: %f\n", correctionCommandAngle);
		
		arm->moveArm(correctionCommandAngle);	
	    
	    isGoodAngle = false;
	}
	else 
	{
		// Robot is within the correct angle threshold
		angleSpeed = 0.0f;
		//printf("correctionCommandAngle: %f\n", angleSpeed);
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
