#include "inputManager.hpp"
#include <iostream>
FRC::inputManager::inputManager(float threshold, unsigned char mode) :
	m1(0),
	m2(0),
	s1(1),
	s2(2)
{
	thresh = threshold;
	setMode(mode);
}
void FRC::inputManager::setMode(unsigned char mode)
{
	driveMode = mode;
}
inline float clamp(float low, float high, float value)
{
	return (value < high) ? ((value > low) ? value : low) : high;
}
inline float threshold(const float & val, const float & thresh)
{
	if((-thresh < val) && (val < thresh))
		return 0;
	else
		return val;
}
void FRC::inputManager::update()
{
	switch(driveMode)
	{
		case MODE_JOY_TANK:
			m1 = threshold(s1.GetY(), thresh);
			m2 = threshold(s2.GetY(), thresh);
			break;
        case MODE_JOY_ARCADE:
        	//the max value is 2.0f, and -2.0f, so by dividing by 2.0, limits the input
        	m1 = threshold(clamp(-1.0f, 1.0f, s1.GetY() + s1.GetX()), thresh);
        	m2 = threshold(clamp(-1.0f, 1.0f, s1.GetY() - s1.GetX()), thresh);
        	break;
        case MODE_XBOX_TANK:
        	m2 = threshold(s1.GetY() / 2.0f, thresh);
        	m1 = threshold(s1.GetRawAxis(5) / 2.0f, thresh);
        	break;
        case MODE_XBOX_ARCADE:
        	m1 = threshold(clamp(-1.0f, 1.0f, s1.GetY() + s1.GetX()), thresh);
        	m2 = threshold(clamp(-1.0f, 1.0f, s1.GetY() - s1.GetX()), thresh);
        	break;
	}
	float diff = m1 - m2;
	if((-thresh < diff) && (diff < thresh))
	{
		float val = (m1 + m2) / 2.0f;
		m1 = val;
		m2 = val;
	}
}
	
float FRC::inputManager::getMotor(int motor)
{
	switch(motor)
	{
		case 1:
			return m1;
			break;
        case 2:
        	return m2;
        	break;
        default:
        	return 0.0f;
	}
}
float FRC::inputManager::getZ(void)
{
	 return s1.GetZ();
}
bool FRC::inputManager::getButton(int button)
{
	// Xbox button1=A
	// xbox button2=B
	// xbox button3=X
	// xbox button4=Y
	// xbox button5=leftbump
	// xbox button6=rightbump
	// xbox button7=back
	// xbox button8=start
	// xbox axis3=triggers
	 return s1.GetRawButton(button);
}

