#include "elevManager.hpp"
#define MAX_HEIGHT 5.0
#define MIN_HEIGHT 2.5

FRC::elevManager::elevManager( FRC::deviceManager * devMan)
{
	devices = devMan;
	threshold = 0.1f;

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

void FRC::elevManager::moveArmTo( float value )
{
	if(value > MAX_HEIGHT)
		value = MAX_HEIGHT;
	if(value < MIN_HEIGHT)
		value = MIN_HEIGHT;
	height = devices->getAnalogVoltage(3);
	while(height > (value + threshold))
	{
		devices->setCANJag(1, -0.1);
	}
	while(height < (value - threshold))
	{
		devices->setCANJag(1, 0.1);		
	}
}

float FRC::elevManager::getHeight()
{
	return height = devices->getAnalogVoltage(3);
}
