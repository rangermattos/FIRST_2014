#include "elevManager.hpp"
#define MAX_HEIGHT 1.0
#define MIN_HEIGHT 0.0

FRC::elevManager::elevManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan )
{
	devices = devMan;
	gMan = guiMan;
	devices->setControlMode(1, 5);
}

void FRC::elevManager::moveElevator( float direction, float speed = 0.5 )
{
	height = devices->getAnalogVoltage(1);
	
	if(height >= MIN_HEIGHT && height <= MAX_HEIGHT)
	{
		if(direction > 0)
		{
			devices->setSpeed(3, speed);
			devices->setSpeed(4, -speed);
		}
		else if(direction < 0)
		{
			devices->setSpeed(3, -speed);
			devices->setSpeed(4, speed);
		}
	}
}

void FRC::elevManager::moveArm( float direction, float speed = 0.5 )
{	
	if(height >= MIN_HEIGHT && height <= MAX_HEIGHT)
	{
		if(direction > 0)
		{
			devices->setCANJag(1, speed);
		}
		else if(direction < 0)
		{
			devices->setCANJag(1, -speed);
		}
	}
}

float FRC::elevManager::getHeight()
{
	return height;
}
