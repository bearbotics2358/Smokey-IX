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
	/*
	ArmC.SetForwardSoftLimit(upperLimit);
	ArmC.SetForwardSoftLimit(lowerLimit);
	*/
}

void PivotArm::Init()
{
	ArmC.SetModeSelect(CanTalonSRX::kMode_PositionCloseLoop); // Change control mode of talon, default is PercentVbus
	ArmC.SetFeedbackDeviceSelect(CANTalon::AnalogEncoder); // Set the feedback device that is hooked up to the talon
	ArmC.SetPgain(0, 0.1); // 1st is slotIdx, I think it means profile #, second is gain
}

void PivotArm::Set(float value, uint8_t syncGroup)
{
	ArmC.Set(value);
}

void PivotArm::Update(Joystick &stick, int port1, int port2, float value)
{
	if(stick.GetRawButton(port1)) {
		ArmC.Set(value);
	} else if(stick.GetRawButton(port2)) {
		ArmC.Set(-1.0 * value);
	} else {
		ArmC.Set(0);
	}

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



