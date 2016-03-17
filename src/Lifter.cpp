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

Lifter::Lifter(int TalonPort, int EncoderAPort, int EncoderBPort, int SwitchPort, int Solenoid1, int Solenoid2)
:	LifterC(TalonPort),
	EncoderC(EncoderAPort, EncoderBPort),
	a_LifterSwitch(SwitchPort),
	a_PleaseDoNotBreakTheBot(Solenoid1, Solenoid2)
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

void Lifter::Set(float value)
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

void Lifter::Shift(int shift)
{
	if(shift == 0) {
		a_PleaseDoNotBreakTheBot.Set(DoubleSolenoid::kReverse);
	} else {
		a_PleaseDoNotBreakTheBot.Set(DoubleSolenoid::kForward);
	}
}

void Lifter::ShiftOpen()
{
	Shift(0);
}

void Lifter::ShiftClosed()
{
	Shift(1);
}
