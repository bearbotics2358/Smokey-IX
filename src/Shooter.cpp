/*
 * Shooter.cpp
 *
 *  Created on: Feb 4, 2016
 *      Author: hstechclub
 */

#include "Shooter.h"
#include <math.h>

bool cockIt = false;
bool fireIt = false;
int cockValue = 800;
int armedTolerance = 100; // ROUGHLY ten degrees

Shooter::Shooter(int TalonPort, int AbsPort)
:   ShooterC(TalonPort),
	EncoderC(AbsPort)
{

}

Shooter::~Shooter()
{

}

void Shooter::Update()
{
	if(cockIt) {
		if( fabs(EncoderC.GetValue() - cockValue) > armedTolerance) {
			ShooterC.Set(0.5);
		} else {
			cockIt = false;
			ShooterC.Set(0);
		}
	} else if(fireIt) {
		ShooterC.Set(0.5);
		fireIt = false;
		cockIt = true;
	} else {
		ShooterC.Set(0);
	}
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
}

void Shooter::Set(int value)
{
	ShooterC.Set(value);
}

void Shooter::Disable()
{
	ShooterC.Set(0);
}


