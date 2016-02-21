/*
 * Shooter.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: hstechclub
 */

#include "Shooter.h"
#include <math.h>
#include "SmokeyIX.h"

bool cockIt = true;
bool fireIt = false;
int cockValue = 2600;
int oldValue = 0;

Shooter::Shooter(int TalonPort, int AbsPort)
:   ShooterC(TalonPort),
	EncoderC(AbsPort)
{
	oldValue = EncoderC.GetValue();
}

Shooter::~Shooter()
{

}

void Shooter::Update(Joystick &a_Stick)
{
	int currentValue = EncoderC.GetValue();
	if(cockIt) {
		if(currentValue <= cockValue) {
			ShooterC.Set(0.5);
		} else {
			cockIt = false;
			ShooterC.Set(0);
		}
	} else if(fireIt) {
		ShooterC.Set(1);
		if(currentValue < oldValue) // evaluates to true if we pass the zero position
		{
			fireIt = false;
			cockIt = true;
		}
	} else {
		ShooterC.Set(0);
	}
	oldValue = currentValue;
}

float Shooter::GetPosition()
{
	return EncoderC.GetValue();
}

void Shooter::SetPosition(float SetPoint)
{

}

void Shooter::Cock()
{
	cockIt = true;
}

void Shooter::Fire()
{
	fireIt = true;
	cockIt = false;
}

void Shooter::Set(float value)
{
	ShooterC.Set(value);
}

void Shooter::Disable()
{
	ShooterC.Set(0);
}


