#include "elevManager.hpp"
#define MAX_HEIGHT 1.0
#define MIN_HEIGHT 0.0

FRC::elevManager::elevManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan )
{
	devices = devMan;
	gMan = guiMan;
	devices->setControlMode(1, 5);
}

void FRC::elevManager::moveElevator( float speed )
{
	height = devices->getAnalogVoltage(1);
	float direction = (speed >= 0) ? 1 : -1;
	
	// rewrite code, we want to move freely until an end stop prevents up or down motion
	//if(height >= MIN_HEIGHT && height <= MAX_HEIGHT)
	//{
	if(direction > 0)
	{
		if(height <= MAX_HEIGHT)
		{	
			devices->setSpeed(3, speed);
			devices->setSpeed(4, speed);
		}
	}
	else if(direction < 0)
	{
		if(height >= MIN_HEIGHT || devices->getHomeSwitch(1) == 1)
		{
			devices->setSpeed(3, speed);
			devices->setSpeed(4, speed);
		}
	}
}


void FRC::elevManager::moveArm( float speed )
{	
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
		if(height >= MIN_HEIGHT || devices->getHomeSwitch(2) == 1)
		{
			devices->setCANJag(1, -speed);
		}
	}
}

float FRC::elevManager::getHeight()
{
	return height;
}
