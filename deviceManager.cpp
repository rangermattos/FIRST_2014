#include "deviceManager.hpp"
#include <iostream>
FRC::deviceManager::deviceManager() :
	elevmotor1(1),
	elevmotor2(2),
	drivemotor1(1),
	drivemotor2(2),
	elevpotheight(1),
	gyro(2),
	ultrasonic(3),
	elevhomeswitch(2),
	compressor(1,1),
	armfire(1),
	lowsp(2),
	highsp(3),
	pistonExtended(0)
	//encoder1(1, 2, true),
	//encoder2(1, 2, true),
{
       vacMotor1 = new CANJaguar(1);
       vacMotor2 = new CANJaguar(2);
       armmotor = new CANJaguar(3);
}

void FRC::deviceManager::setSpeed(int motor, float speed)
{
	switch(motor)
	{
	case 1:
		drivemotor1.Set(speed);
		break;
	case 2:
		drivemotor2.Set(speed);
		break;
	}
}

void FRC::deviceManager::setSolenoid(int sol, bool set)
{
	switch(sol)
	{
	case 1:
		armfire.Set(set);
		break;
	case 2:
		lowsp.Set(set);
		break;
	case 3:
		highsp.Set(set);
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

/*int FRC::deviceManager::getEncoderValue(int encoder)// may need changes later
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
}*/

void FRC::deviceManager::startCompressor()
{
	compressor.Start();
}

void FRC::deviceManager::stopCompressor()
{
	compressor.Stop();
}

float FRC::deviceManager::getCANJagCurrent(int CANJag)
{
	switch(CANJag)
	{
	case 1:
		return vacMotor1->GetOutputCurrent();
	case 2:
		return vacMotor2->GetOutputCurrent();
	case 3:
		return armmotor->GetOutputCurrent();
	}
}

