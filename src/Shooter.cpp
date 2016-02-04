/*
 * Shooter.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: hstechclub
 */

#include "Shooter.h"

bool cockIt = false;
bool fireIt = false;
int cockValue = 0;

Shooter::Shooter(int TalonPort, int AbsPort)
:
	ShooterC(TalonPort),
	EncoderC(AbsPort)
{

}

Shooter::~Shooter()
{

}

void Shooter::Update()
{
	if(cockIt)
	{
		if(EncoderC.Get() < cockValue)
		{
			ShooterC.Set(0.5);
		}
		else
		{
			cockIt = false;
		}
	}
	else if(fireIt)
	{
		ShooterC.Set(.5);
		fireIt = false;
	}
}

float Shooter::GetPosition()
{
	return EncoderC.Get();
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
}

void Shooter::Set(int value)
{
	ShooterC.Set(value);
}

void Shooter::Disable()
{
	ShooterC.Set(0);
}


