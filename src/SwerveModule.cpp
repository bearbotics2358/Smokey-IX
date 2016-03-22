/*
 * SwerveModule.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: hstechclub
 */

#include "SwerveModule.h"

const double ANALOG_CONV_FACTOR = 1024 / 360;
const double QUAD_SPEED_CONV_FACTOR = 0; // need to talk with someone in order to figure this one out

SwerveModule::SwerveModule(uint32_t turnMotorPort, uint32_t driveMotorPort)
: a_TurnMotor(turnMotorPort),
  a_DriveMotor(driveMotorPort)
{
	a_TurnMotor.SetControlMode(CANTalon::kPosition);
	a_TurnMotor.SetFeedbackDevice(CANTalon::AnalogEncoder);
	a_TurnMotor.SetSensorDirection(false);
	a_TurnMotor.SetPID(1,0,0);
	a_TurnMotor.Set(0);

	a_DriveMotor.SetControlMode(CANTalon::kSpeed);
	a_DriveMotor.SetFeedbackDevice(CANTalon::QuadEncoder);
	a_DriveMotor.SetSensorDirection(false);
	a_DriveMotor.SetPID(1,0,0);
	a_DriveMotor.Set(0);

}

void SwerveModule::Set(float angle, float speed)
{
	if(fabs(angle - GetAngle()) > 180 && angle > 180) {
		angle -= 180;
		speed *= -1.0;
	} else if(fabs(angle - GetAngle()) > 180 && angle < 180) {
		angle += 180;
		speed *= -1.0;
	}

	a_TurnMotor.Set(angle * ANALOG_CONV_FACTOR);
	a_DriveMotor.Set(speed * QUAD_SPEED_CONV_FACTOR);
}

float SwerveModule::GetAngle()
{
	return a_TurnMotor.GetPosition();
}

float SwerveModule::GetSpeed()
{
	return a_DriveMotor.GetSpeed();
}

float SwerveModule::GetDistance()
{
	return a_DriveMotor.GetEncPosition();
}

void SwerveModule::InvertQuad()
{
	a_DriveMotor.SetSensorDirection(true);
}

void SwerveModule::InvertAnalog()
{
	a_TurnMotor.SetSensorDirection(true);
}

void SwerveModule::InvertDriveMotor()
{
	a_DriveMotor.SetInverted(true);
}

void SwerveModule::SetPID(float turnP, float turnI, float turnD, float speedP, float speedI, float speedD)
{
	a_TurnMotor.SetPID(turnP,turnI,turnD);
	a_DriveMotor.SetPID(speedP,speedI,speedD);
}
