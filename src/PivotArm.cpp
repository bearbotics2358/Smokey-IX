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
	ArmC.SetControlMode(CANTalon::kPosition); // Change control mode of talon, default is PercentVbus
	ArmC.SetFeedbackDevice(CANTalon::AnalogEncoder); // Set the feedback device that is hooked up to the talon
	ArmC.Set(258);
	ArmC.SetP(10);
}

void PivotArm::Set(float value, uint8_t syncGroup)
{
	ArmC.SetSetpoint(value);
}

void PivotArm::Update(Joystick &stick, int port1, int port2, float value)
{
	if(stick.GetRawButton(port1)) {
		ArmC.SetSetpoint(258);
	} else if(stick.GetRawButton(port2)) {
		ArmC.SetSetpoint(530);
	}

}

float PivotArm::GetAngle()
{
	return ArmC.GetPosition();
}

void PivotArm::Disable()
{
	ArmC.Set(0);
}

void PivotArm::PIDWrite(float output)
{

}



