/*
 * PivotArm.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */
#include "PivotArm.h"

PivotState State = kArmIdle;

PivotArm::PivotArm(int TalonPort, int AbsPort, int Up, int Low)
:
	ArmC(TalonPort),
	EncoderC(AbsPort)
{
	upperLimit = Up;
	lowerLimit = Low;
}

PivotArm::~PivotArm()
{

}

void PivotArm::Set(float value, uint8_t syncGroup)
{
	ArmC.Set(value);
}

void PivotArm::Update(Joystick &stick, int port1, int port2, int value)
{
	if(EncoderC.Get() > upperLimit || EncoderC.Get() < lowerLimit)
	{
		Disable();
	}
	if(stick.GetRawButton(port1))
	{
		ArmC.Set(value);
	}
	else if(stick.GetRawButton(port2))
	{
		ArmC.Set(-1.0 * value);
	}

}

void PivotArm::SetToAngle(float value2, float angle, uint8_t syncGroup)
{

}

float PivotArm::GetAngle()
{
	return EncoderC.Get();
}

void PivotArm::Disable()
{
	ArmC.Set(0);
}

void PivotArm::PIDWrite(float output)
{

}



