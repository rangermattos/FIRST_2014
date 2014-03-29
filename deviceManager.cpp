#include "deviceManager.hpp"
#include <iostream>
FRC::deviceManager::deviceManager() :
	drivemotor1(3),
	drivemotor2(2),
	//vacMotor1(3),
	//vacMotor2(4),
	gyro(2),
	ultrasonic(3),
	armPotHeight(4),
	armhomeswitch(3),
	compressor(1,1),
	armfire(1),
	armrelief(2),
	lowsp(3),
	highsp(4),
	pistonExtended(0),
	//encoder1(1, 2, true),
	//encoder2(1, 2, true),
	armMotor (new CANJaguar(2)),
	vacMotor1  (new CANJaguar(3)),
	vacMotor2  (new CANJaguar(4))
{

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
	case 2:
		return vacMotor1->GetOutputCurrent();
	case 3:
		return vacMotor2->GetOutputCurrent();
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
		vacMotor1->Set(value);
		break;
	case 3:
		vacMotor2->Set(value);
		break;*/
	default:
		armMotor->Set(value);
	}
}

double FRC::deviceManager::PotPosition()
{
	return armMotor->GetPosition();
}


float FRC::deviceManager::getSensorSignal(string device)
//Chooses which analog voltage to display
{
	if (device == "gyro")
	{
		return gyro.GetAngle();
		//return gyro.GetVoltage();
	}
	else if (device == "ultrasonic")
	{
		//return ultrasonic.GetVoltage();
		return ultrasonic.GetAverageVoltage()*512/60;
	}
	else if (device == "armPotHieght")
	{
		return armPotHeight.GetVoltage();
	}
	else
	{
		return 0.0f;
	}
}

void FRC::deviceManager::drivemotor1Control(float speed)
{
	drivemotor1.SetSpeed(speed);
}

void FRC::deviceManager::drivemotor2Control(float speed)
{
	drivemotor2.SetSpeed(speed);
}

void FRC::deviceManager::vacMotor1Control(float speed)
{
	vacMotor1->Set(speed);
}

void FRC::deviceManager::vacMotor2Control(float speed) 
{
	vacMotor2->Set(speed);
}

void FRC::deviceManager::armMotorControl(float speed)
{
	armMotor->Set(speed);
}

int FRC::deviceManager::getHomeSwitch(void)
{
	return armhomeswitch.Get();
}
void FRC::deviceManager::resetGyro(void)
{
	gyro.Reset();
}
