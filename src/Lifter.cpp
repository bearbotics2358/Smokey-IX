/*
 * Lifter.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */
#include <WPILib.h>
#include "Lifter.h"

Lifter::Lifter(int TalonPort, int EncoderAPort, int EncoderBPort)
:	LifterC(TalonPort),
	EncoderC(EncoderAPort, EncoderBPort)
{

}

Lifter::~Lifter()
{
}

void Lifter::Update(float value, uint8_t syncGroup)
{
	LifterC.Set(value);
}

bool Lifter::GetLength()
{
	return EncoderC.GetDistance();
}

void Lifter::Disable()
{
	LifterC.Set(0);
}

void PIDWrite(float output)
{

}

