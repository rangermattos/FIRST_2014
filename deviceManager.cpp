#include "deviceManager.hpp"
#include <iostream>
FRC::deviceManager::deviceManager() :
	talon1(1),
	talon2(2),
	ultrasonic(1),
	switch1(1),
	compressor(2,1),
	sol1(1),
	sol2(2),
	pistonExtended(0),
	encoder(1, 2, true)
{
       can = new CANJaguar(1);
}

void FRC::deviceManager::setSpeed(int motor, float speed)
{
	switch(motor)
	{
	case 1:
		talon1.Set(speed);
		break;
	case 2:
		talon2.Set(speed);
		break;
	}
}

void FRC::deviceManager::setSolenoid(int sol, bool set)
{
	switch(sol)
	{
	case 1:
		sol1.Set(set);
		break;
	case 2:
		sol2.Set(set);
		break;
	}
}

bool FRC::deviceManager::isPistonExtended()
{
	return pistonExtended;
}

void FRC::deviceManager::togglePistonExtended()
{
	pistonExtended = !pistonExtended;
}

int FRC::deviceManager::getEncoderValue()
{
	return encoder.GetRaw();
}

void FRC::deviceManager::startEncoder()
{
	encoder.Start();
}

void FRC::deviceManager::startCompressor()
{
	compressor.Start();
}
