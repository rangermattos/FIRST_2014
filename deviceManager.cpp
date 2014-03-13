#include "deviceManager.hpp"
#include <iostream>
FRC::deviceManager::deviceManager() :
	drivemotor1(1),
	drivemotor2(2),
	vacMotor1(3),
	vacMotor2(4),
	gyro(2),
	ultrasonic(3),
	armPotHeight(4),
	armhomeswitch(3),
	compressor(1,1),
	armfire(1),
	armrelief(2),
	lowsp(3),
	highsp(4),
	pistonExtended(0)
	//encoder1(1, 2, true),
	//encoder2(1, 2, true),
{
	armMotor = new CANJaguar(2);
	//testMotor1 = new CANJaguar(3);
	//testMotor2 = new CANJaguar(4);
	//vacMotor1 = new CANJaguar(2);
	//vacMotor2 = new CANJaguar(3);
}

void FRC::deviceManager::setSpeed(int motor, float speed)
//choose drive motor and sets the speed.
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
//choose the solenoid and decide if it is true of false.
{
	switch(sol)
	{
	case 1:
		armfire.Set(set);
		break;
	case 2:
		armrelief.Set(set);
		break;
	case 3:
		lowsp.Set(set);
		break;
	case 4:
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
//gets the current of the CANJaguar motors.
{
	switch(CANJag)
	{
	case 1:
		return armMotor->GetOutputCurrent();
	/*case 2:
		return testMotor1->GetOutputCurrent();
	case 3:
		return testMotor2->GetOutputCurrent();*/
	default:
		return armMotor->GetOutputCurrent();
	}
}

void FRC::deviceManager::setCANJag(int CANJag, float value)
//Used to set the speed of the CANJaguars.
{
	switch(CANJag)
	{
	case 1:
		armMotor->Set(value);
		break;
	/*case 2:
		testMotor1->Set(value);
		break;
	case 3:
		testMotor2->Set(value);
		break;*/
	default:
		armMotor->Set(value);
	}
}

void FRC::deviceManager::setControlMode(int CANJag, int mode)
//Changes the control mode of each CANJaguar.
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
	/*case 2:
		if(mode == 1)
			testMotor1->ChangeControlMode(CANJaguar::kPercentVbus);
		else if(mode == 2)
			testMotor1->ChangeControlMode(CANJaguar::kCurrent);
		else if(mode == 3)
			testMotor1->ChangeControlMode(CANJaguar::kSpeed);
		else if(mode == 4)
			testMotor1->ChangeControlMode(CANJaguar::kPosition);
		else if(mode == 5)
			testMotor1->ChangeControlMode(CANJaguar::kVoltage);
		break;
	case 3:
		if(mode == 1)
			testMotor2->ChangeControlMode(CANJaguar::kPercentVbus);
		else if(mode == 2)
			testMotor2->ChangeControlMode(CANJaguar::kCurrent);
		else if(mode == 3)
			testMotor2->ChangeControlMode(CANJaguar::kSpeed);
		else if(mode == 4)
			testMotor2->ChangeControlMode(CANJaguar::kPosition);
		else if(mode == 5)
			testMotor2->ChangeControlMode(CANJaguar::kVoltage);
		break; */
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
	/*case 2:
		if(reference == 1)
			testMotor1->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		else if(reference == 2)
			testMotor1->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
		break;
	case 3:
		if(reference == 1)
			testMotor2->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
		else if(reference == 2)
			testMotor2->SetPositionReference(CANJaguar::kPosRef_Potentiometer);
		break;*/
	}
}

double FRC::deviceManager::PotPosition()
{
	return armMotor->GetPosition();
}


float FRC::deviceManager::getAnalogVoltage( int analog )
//Chooses which analog voltage to display
{
	switch(analog)
	{
	case 1:
		return gyro.GetVoltage();
	case 2:
		return ultrasonic.GetVoltage();
	case 3:
		return armPotHeight.GetVoltage();
	default:
		break;
	}
}

float FRC::deviceManager::drivemotor1Control(float speed)
{
	drivemotor1.SetSpeed(speed);
}

float FRC::deviceManager::drivemotor2Control(float speed)
{
	drivemotor2.SetSpeed(speed);
}

float FRC::deviceManager::vacMotor1Control(float speed)
{
	vacMotor1.Set(speed, 0);
}

float FRC::deviceManager::vacMotor2Control(float speed) 
{
	vacMotor2.Set(speed, 0);
}

float FRC::deviceManager::armMotorControl(float speed)
{
	armMotor->Set(speed, 0);
}

int FRC::deviceManager::getHomeSwitch(void)
{
		return armhomeswitch.Get();
}
