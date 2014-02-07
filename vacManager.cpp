#include "vacManager.hpp"

FRC::vacManager::vacManager(FRC::inputManager * inpMan, FRC::deviceManager * devMan, FRC::guiManager * guiMan) :
	currentThreshold(25.0)
{
	inputMan = inpMan;
	devices = devMan;
	gMan = guiMan;
	devices->setControlMode(2, 5);
	devices->setControlMode(3, 5);
}

void FRC::vacManager::vacuum()
{	
	CANJagCurrent1 = devices->getCANJagCurrent(2);
	CANJagCurrent2 = devices->getCANJagCurrent(3);
	
	// debugging : print out both CANJaguar currents
	gMan->print(4, "%d", CANJagCurrent1);
	gMan->print(5, "%d", CANJagCurrent2);
	
	if(CANJagCurrent1 < currentThreshold)
	{
		
		// arm firing solenoid
		devices->setSolenoid(1, 1);
		Wait(0.050);
		devices->setSolenoid(1, 0);	
		
	}
}

void FRC::vacManager::shoot()
{
	if(CANJagCurrent1 < currentThreshold)
	{
		// turn off vacuum temporarily
		devices->setCANJag(2, 0);
		devices->setCANJag(3, 0);
		
		Wait(0.050);
		
		devices->setCANJag(2, 1);
		devices->setCANJag(3, 1);
	}
}


// "Beware of bugs in the above code; I have only proved it correct, not tried it."
// --Donald Knuth
