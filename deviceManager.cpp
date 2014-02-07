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
	armMotor = new CANJaguar(1);
	vacMotor1 = new CANJaguar(2);
	vacMotor2 = new CANJaguar(3);
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
		return armMotor->GetOutputCurrent();
	case 2:
		return vacMotor1->GetOutputCurrent();
	case 3:
		return vacMotor2->GetOutputCurrent();
	default:
		return armMotor->GetOutputCurrent();
	}
}

void FRC::deviceManager::setCANJag(int CANJag, float value)
{
	switch(CANJag)
	{
	case 1:
		armMotor->Set(value);
		break;
	case 2:
		vacMotor1->Set(value);
		break;
	case 3:
		vacMotor2->Set(value);
		break;
	}
}

void FRC::deviceManager::setControlMode(int CANJag, int mode)
{
	switch(CANJag)
	{
	case 1:
		if(mode == 1)
			armMotor->ChangeControlMode(CANJaguar::kPercentVbus);
		else if(mode == 2)
			armMotor->ChangeControlMode(CANJaguar::kCurrent);
		else if(mode == 3)
			armMotor->ChangeControlMode(CANJaguar::kSpeed);
		else if(mode == 4)
			armMotor->ChangeControlMode(CANJaguar::kPosition);
		else if(mode == 5)
			armMotor->ChangeControlMode(CANJaguar::kVoltage);
		break;
	case 2:
		if(mode == 2)
			vacMotor1->ChangeControlMode(CANJaguar::kPercentVbus);
		else if(mode == 2)
			vacMotor1->ChangeControlMode(CANJaguar::kCurrent);
		else if(mode == 3)
			vacMotor1->ChangeControlMode(CANJaguar::kSpeed);
		else if(mode == 4)
			vacMotor1->ChangeControlMode(CANJaguar::kPosition);
		else if(mode == 5)
			vacMotor1->ChangeControlMode(CANJaguar::kVoltage);
		break;
	case 3:
		if(mode == 3)
			vacMotor2->ChangeControlMode(CANJaguar::kPercentVbus);
		else if(mode == 2)
			vacMotor2->ChangeControlMode(CANJaguar::kCurrent);
		else if(mode == 3)
			vacMotor2->ChangeControlMode(CANJaguar::kSpeed);
		else if(mode == 4)
			vacMotor2->ChangeControlMode(CANJaguar::kPosition);
		else if(mode == 5)
			vacMotor2->ChangeControlMode(CANJaguar::kVoltage);
		break;
	}	
}

void FRC::deviceManager::setPositionReference(int CANJag, int reference)
{
	//armmotor->ChangeControlMode(CANJaguar::kPosition);
	//armmotor->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
	switch(CANJag)
	{
	case 1:
		if(reference == 1)
			armMotor->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		else if(reference == 2)
			armMotor->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
		break;
	case 2:
		if(reference == 1)
			vacMotor1->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		else if(reference == 2)
			vacMotor1->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
		break;
	case 3:
		if(reference == 1)
			vacMotor2->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		else if(reference == 2)
			vacMotor2->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
		break;
	}
}

double FRC::deviceManager::PotPosition()
{
	return armMotor->GetPosition();
}

