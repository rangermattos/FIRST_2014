#include "inputManager.hpp"
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
        if(mode != MODE_JOY_TANK || mode != MODE_JOY_ARCADE || mode != MODE_XBOX_TANK || mode != MODE_XBOX_ARCADE)
                driveMode = MODE_JOY_TANK;
        else
                driveMode = mode;
}
inline float threshold(const float & val, const float & thresh)
{
        if(-thresh < val < thresh)
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
                //WARNING! REQUIRES INPUT LIMITING, DO NOT COMPILE WITHOUT IMPLIMENTING PROPER LIMITING
                m1 = threshold(s1.GetY() + s1.GetX(), thresh);
                m2 = threshold(s1.GetY() - s1.GetX(), thresh);
                break;
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
