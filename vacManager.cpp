#include "vacManager.hpp"

FRC::vacManager::vacManager(FRC::inputManager * inpMan, FRC::deviceManager * devMan, FRC::guiManager * guiMan) :
	currentThreshold(25.0)
{
	inputMan = inpMan;
	devices = devMan;
	gMan = guiMan;
}

void FRC::vacManager::vacuum()
{	
	CANJagCurrent1 = devices->getCANJagCurrent(1);
	CANJagCurrent2 = devices->getCANJagCurrent(2);
	
	// debugging : print out both CANJaguar currents
	gMan->print(4, "%d", CANJagCurrent1);
	gMan->print(5, "%d", CANJagCurrent2);
	
	if(CANJagCurrent1 < currentThreshold)
	{
		
		// arm firing solenoid
		devices->setSolenoid(1, 1);
		Wait(0.050);
		devices->setSolenoid(1, 0);	
			
		// if fire button pressed, fire
		if(inputMan->getButton(3))
		{
			// turn off vacuum temporarily
			
		}
		
	}
}
