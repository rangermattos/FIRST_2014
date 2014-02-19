#include "vacManager.hpp"

FRC::vacManager::vacManager( FRC::deviceManager * devMan, FRC::guiManager * guiMan ) :
	currentThreshold(25.0)
{
	devices = devMan;
	gMan = guiMan;
	//devices->setControlMode(2, 5);
	//devices->setControlMode(3, 5);
	bool armReady = false;
}

void FRC::vacManager::vacuum()
//Use the CANJaguar current to tell if the ball can be fired.
{	
	/*CANJagCurrent1 = devices->getCANJagCurrent(2);
	CANJagCurrent2 = devices->getCANJagCurrent(3);
	
	// debugging : print out both CANJaguar currents
	gMan->print(4, "%d", CANJagCurrent1);
	gMan->print(5, "%d", CANJagCurrent2);
	*/
	
	//if(!armReady)
	//{
		
		// arm firing solenoid
		devices->setSolenoid(1, true);
		//Wait(0.050);
		devices->setSolenoid(2, false);	
		
		//armReady = true;
		
	//}
}

void FRC::vacManager::shoot()
//turns off the vacuum to fire.
{
	//if(armReady)
	//{
		// turn off vacuum temporarily
		devices->vacMotor1Control(0);
		devices->vacMotor2Control(0);
		
		Wait(2.0);
		
		devices->setSolenoid(1, false);
		devices->setSolenoid(2, true);
				
		//armReady = false;
	//}
}


// "Beware of bugs in the above code; I have only proved it correct, not tried it."
// --Donald Knuth
