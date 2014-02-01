#include "deviceManager.hpp"
#include <iostream>
FRC::deviceManager::deviceManager() :
	//talon1(1),
	//talon2(2),
	victor1(1),
	victor2(2),
	ultrasonic(1),
	switch1(1),
	compressor(2,1),
	sol1(1),
	sol2(2),
	sol3(3),
	sol4(4),
	pistonExtended(0),
	encoder1(1, 2, true),
	encoder2(1, 2, true)
{
       can1 = new CANJaguar(1);
       can2 = new CANJaguar(2);
}

void FRC::deviceManager::setSpeed(int motor, float speed)
{
	switch(motor)
	{
	case 1:
		victor1.Set(speed);
		break;
	case 2:
		victor2.Set(speed);
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

int FRC::deviceManager::getEncoderValue(int encoder)// may need changes later
{
	if (encoder == 1)
		return encoder1.GetRaw();
	else 
		return encoder2.GetRaw();
}

void FRC::deviceManager::startEncoder()
{
	encoder1.Start();
	encoder2.Start();
}

void FRC::deviceManager::startCompressor()
{
	compressor.Start();
}

void FRC::deviceManager::stopCompressor()
{
	compressor.Stop();
}

