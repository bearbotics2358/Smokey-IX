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
	SetAngle(0);
	ArmC.SetP(10);
	ArmC.SetI(0.0001);
}

void PivotArm::Set(float value, uint8_t syncGroup)
{
	ArmC.SetSetpoint(value);
}

void PivotArm::SetAngle(float angleToSet)
{
	ArmC.SetSetpoint( ( angleToSet * (1024.0 / 360.0) ) + 258.0 );
}

void PivotArm::Update(Joystick &stick, int port1, int port2, int port3, int port4, int port5)
{
	if(stick.GetRawButton(port1)) {
		SetAngle(26);
	} else if(stick.GetRawButton(port2)) {
		SetAngle(90);
	} else if(stick.GetRawButton(port3)) {
		SetAngle(45);
	} else if(stick.GetRawButton(port4)) {
		SetAngle(115);
	} else if(stick.GetRawButton(port5)) {
		SetAngle(0);
	}
}

float PivotArm::GetAngle()
{
	return (ArmC.GetPosition() * 360 / 1024) - 90;
}

void PivotArm::Disable()
{
	ArmC.Set(0);
}

void PivotArm::PIDWrite(float output)
{

}



