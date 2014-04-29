#include "vacManager.hpp"

FRC::vacManager::vacManager( FRC::deviceManager * devMan ) :
	currentThreshold(25.0)
{
	devices = devMan;
}

void FRC::vacManager::shoot(float delay)
//turns off the vacuum to fire.
{	
	// Fire the solenoid, wait then drop vacuum then drop solenoid
	devices->setSolenoid(1, true);
	devices->setSolenoid(2, false);
	
	//Wait(0.2); // Works best so far with this delay
	//Wait(0.2);
	Wait(delay);
	
	// Turn off vacs to allow vac behind ball to drop
	//devices->vacMotor1Control(0);
	//devices->vacMotor2Control(0);
			
	// Allow time for the ball to fire before dropping the solenoids back down
	//Wait(1.5); //1.0
			
	//devices->setSolenoid(1, false);
	//devices->setSolenoid(2, true);
	
}
