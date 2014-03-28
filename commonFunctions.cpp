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

float FRC::threshold(float val, float thresh)
{
	float output;
	
	if((-thresh < val) && (val < thresh))
		output = 0;
	else
		output = val;
}
