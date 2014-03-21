#include "elevManager.hpp"
#define MAX_HEIGHT 4.4
#define MIN_HEIGHT 2.35

FRC::elevManager::elevManager( FRC::deviceManager * devMan)
{
	devices = devMan;
	threshold = 0.1f; // Move to arm threshold

}

void FRC::elevManager::moveArm( float speed )
// Used to move the arm. Also invloves height constraints of the arm. 
{	
	/*Timer(t3);
	        	t3.Reset();
	        	t3.Start();
	        	double	t2 = t3.Get();*/
	height = devices->getAnalogVoltage(3);
	float direction = (speed >= 0) ? 1 : -1; // Direction up = 1
	//printf("2a1 %f\n", t3.Get() - t2);
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
		//printf("2a2 %f\n", t3.Get() - t2);
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
		//printf("2a3 %f\n", t3.Get() - t2);
	}
}

void FRC::elevManager::moveArmTo( float value )
{
	// Check value is within max/min
	if(value > MAX_HEIGHT)
		value = MAX_HEIGHT;
	if(value < MIN_HEIGHT)
		value = MIN_HEIGHT;
	
	// Current arm position
	height = devices->getAnalogVoltage(3);
	
	while(height > (value + threshold))
	{
		devices->setCANJag(1, -0.1); // Current command target is set to %10
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
