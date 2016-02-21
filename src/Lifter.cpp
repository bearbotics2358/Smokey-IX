/*
 * Lifter.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: hstechclub
 */
#include <WPILib.h>
#include "Lifter.h"

double CONVERSION_FACTOR = 3.5 / 1000; // inches per encoder counts
double UPPER_TARGET = 64.5;
double LOWER_TARGET = 46;
bool movingUpTower = false; // Tape moving up the tower
bool liftBot = false; // Actually lifting the robot

Lifter::Lifter(int TalonPort, int EncoderAPort, int EncoderBPort, int SwitchPort)
:	LifterC(TalonPort),
	EncoderC(EncoderAPort, EncoderBPort),
	a_LifterSwitch(SwitchPort)
{

}

Lifter::~Lifter()
{

}

void Lifter::Update(uint8_t syncGroup)
{
	if(!a_LifterSwitch.Get())
	{
		EncoderC.Reset();
	}
	double tapeLength = EncoderC.GetRaw() * CONVERSION_FACTOR;
	if(movingUpTower && (tapeLength <= UPPER_TARGET) && !liftBot) {
		LifterC.Set(0.5);
	} else if(tapeLength >= UPPER_TARGET) {
		liftBot = true;
		movingUpTower = false;
	}
	if(liftBot) {
		if(tapeLength > LOWER_TARGET) {
			LifterC.Set(5 / 12);
		} else {
			// need to switch solenoid here
			liftBot = false;
		}
	} else {
		LifterC.Set(0);
	}

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

