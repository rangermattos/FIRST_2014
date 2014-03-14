#include "elevManager.hpp"
#define MAX_HEIGHT 5.0
#define MIN_HEIGHT 2.5

FRC::elevManager::elevManager( FRC::deviceManager * devMan)
{
	devices = devMan;

}

void FRC::elevManager::moveArm( float speed )
//Used to move the arm. Also invloves height constraints of the arm. 
{	

	height = devices->getAnalogVoltage(3);
	float direction = (speed >= 0) ? 1 : -1; // Direction up = 1

	if(direction > 0)
	{
		if(height <= MAX_HEIGHT && devices->getHomeSwitch() == 1) 
		{
			devices->setCANJag(1, -speed);
		}
		else
		{
			devices->setCANJag(1, 0);
		}
	}
	else if(direction < 0)
	{
		if(height >= MIN_HEIGHT)
		{
			devices->setCANJag(1, -speed);
		}
		else
		{
			devices->setCANJag(1, 0);
		}
	}
}

float FRC::elevManager::getHeight()
{
	return height;
}
