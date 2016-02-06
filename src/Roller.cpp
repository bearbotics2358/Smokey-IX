/*
 * Roller.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */
#include <WPILib.h>
#include "Roller.h"

Roller::Roller(int TalonPort, int SwitchPort)
:	ArmC(TalonPort),
	SwitchC(SwitchPort)
{

}

Roller::~Roller()
{
}

void Roller::Update(float value, uint8_t syncGroup)
{
	ArmC.Set(value);
}

bool Roller::GetSwitch()
{
	return SwitchC.Get();
}

void Roller::Disable()
{
	ArmC.Set(0);
}


