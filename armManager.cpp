#include "armManager.hpp"
#define MAX_HEIGHT 4.25
#define MIN_HEIGHT 2.2

FRC::armManager::armManager( FRC::deviceManager * devMan)
{
	devices = devMan;
	threshold = 0.1f; // Move to arm threshold

}

void FRC::armManager::moveArm( float speed )
// Used to move the arm. Also invloves height constraints of the arm. 
{	
	height = devices->getSensorSignal("armPotHieght");
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

void FRC::armManager::moveArmTo( float value )
{
	// Check value is within max/min
	if(value > MAX_HEIGHT)
		value = MAX_HEIGHT;
	if(value < MIN_HEIGHT)
		value = MIN_HEIGHT;
	
	// Current arm position
	height = devices->getSensorSignal("armPotHieght");
	
	while(height > (value + threshold))
	{
		devices->setCANJag(1, -0.1); // Current command target is set to %10
	}
	
	while(height < (value - threshold))
	{
		devices->setCANJag(1, 0.1);		
	}
}

float FRC::armManager::getHeight()
{
	return height = devices->getSensorSignal("armPotHieght");
}
