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
	EncoderC(EncoderAPort, EncoderBPort),
	pulseA(EncoderAPort),
	pulseB(EncoderBPort)
{

}

Lifter::~Lifter()
{

}

void Lifter::Update(float value, uint8_t syncGroup)
{
	LifterC.Set(value);
}

float Lifter::GetLength()
{
	return EncoderC.GetRaw();
}

void Lifter::Disable()
{
	LifterC.Set(0);
}

void PIDWrite(float output)
{

}

