/*
 * PivotArm.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */
#include "PivotArm.h"

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

void PivotArm::SetToAngle(float value2, float angle, uint8_t syncGroup)
{

}

float PivotArm::GetAngle()
{
	return EncoderC.GetValue();
}

void PivotArm::Disable()
{
	ArmC.Set(0);
}

void PivotArm::PIDWrite(float output)
{

}



