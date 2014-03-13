#include "elevManager.hpp"
#define MAX_HEIGHT 3.62
#define MIN_HEIGHT 0.0

FRC::elevManager::elevManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan )
{
	devices = devMan;
	gMan = guiMan;
	devices->setControlMode(1, 5);
	//devices->setControlMode(2, 5);
	//devices->setControlMode(3, 5);
}

void FRC::elevManager::moveArm( float speed )
//Used to move the arm. Also invloves height constraints of the arm. 
{	
	devices->setCANJag(2,0);
	devices->setCANJag(3,0);
	height = devices->getAnalogVoltage(3);
	float direction = (speed >= 0) ? 1 : -1;
	//if(height >= MIN_HEIGHT && height <= MAX_HEIGHT)
	//{
	if(direction > 0)
	{
		if(height <= MAX_HEIGHT)
		{
			devices->setCANJag(1, speed);
		}
	}
	else if(direction < 0)
	{
		if(height >= MIN_HEIGHT && devices->getHomeSwitch() == 1)
		{
			devices->setCANJag(1, speed);
		}
	}
}

float FRC::elevManager::getHeight()
{
	return height;
}
