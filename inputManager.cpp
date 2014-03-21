#include "inputManager.hpp"
#include <iostream>
#include <cmath>

FRC::inputManager::inputManager(float threshold, unsigned char mode) :
	m1(0),
	m2(0),
	s1(1), // xbox
	s2(2), // xbox
	s3(3) //joystick
{
	thresh = threshold;
	setMode(mode);
	enhancedIO = &DriverStation::GetInstance()->GetEnhancedIO();

}

unsigned char FRC::inputManager::getMode()
//tells if in tank or arcade.
{
	return driveMode;
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

float FRC::inputManager::rateLimit( float newVal, float prevVal, float rate )
{
	float out;
	float delta = fabs(newVal - prevVal);
	float sign = (newVal > prevVal) ? 1 : -1;
	
	if(delta > rate)
		out = prevVal + sign * rate;
	else
		out = newVal;
	
	return out;
}

void FRC::inputManager::update()
//changes the drive mode.
{
	switch(driveMode)
	{
		case MODE_JOY_TANK:
			m1 = threshold(s1.GetY(), thresh);
			m2 = threshold(s3.GetY(), thresh);
			break;
        case MODE_JOY_ARCADE:
        	//the max value is 2.0f, and -2.0f, so by dividing by 2.0, limits the input
        	m1 = threshold(clamp(-1.0f, 1.0f, s1.GetY() + s1.GetX()), thresh);
        	m2 = threshold(clamp(-1.0f, 1.0f, s1.GetY() - s1.GetX()), thresh);
        	break;
        case MODE_XBOX_TANK:
        	m2 = threshold(s1.GetY() / 1.0f, thresh);
        	m1 = threshold(s1.GetRawAxis(5) / 1.0f, thresh);
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
//returns the values of the motors.
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

bool FRC::inputManager::getButton(int joystick, int button)
{
	/*
	* logitech joystick buttons match indicators
	* xbox button1 = A
	* xbox button2 = B
	* xbox button3 = X
	* xbox button4 = Y
	* xbox button5 = leftbump
	* xbox button6 = rightbump
	* xbox button7 = back
	* xbox button8 = start
	*/
	switch(joystick)
	{
	case 1:
		return s1.GetRawButton(button);
		break;
	case 2:
		return s2.GetRawButton(button);
		break;
	case 3:
		return s3.GetRawButton(button);
		break;
	default:
		return false;
	}
		
}

float FRC::inputManager::getAxis(int joystick, int axis)
{
	/*
	 * axis 1 = left, right
	 * axis 2 = foward, back
	 * axis 3 = rotate
	 * axis 4 = throttle 1 up, -1 down
	 * axis 5 = top left right
	 * axis 6 = top -1 foward, 1 back
	 * xbox axis 3 = triggers
	 */
	//return s1.GetRawAxis(axis);
	switch(joystick)
	{
	case 1:
		return s1.GetRawAxis(axis);
		break;
	case 2:
		return s2.GetRawAxis(axis);
		break;
	default:
		return 0.0f;
	}
}
