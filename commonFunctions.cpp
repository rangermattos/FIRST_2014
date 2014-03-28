#include "commonFunctions.hpp"

float FRC::clamp(float low, float high, float value)
{
	float output;
	
	if (value > high)
		output = high;
	else if (value < low)
		output = low;
	else
		output = value;
	
	return output;
}
