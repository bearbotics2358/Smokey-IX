/*
 * SwerveModule.h
 *
 *  Created on: Mar 21, 2016
 *      Author: hstechclub
 */

#pragma once

#include <WPILib.h>
#include <SpeedController.h>

class SwerveModule
{
public:
	SwerveModule(uint32_t turnMotorPort, uint32_t driveMotorPort);
	virtual ~SwerveModule() = default;

	void Set(float angle, float speed);

	float GetAngle();
	float GetSpeed();
	float GetDistance();

	void InvertQuad();
	void InvertAnalog();

	void InvertDriveMotor();

	void SetPID(float turnP, float turnI, float turnD, float speedP, float speedI, float speedD);

private:
	CANTalon a_TurnMotor;
	CANTalon a_DriveMotor;
};

/* SRC_SWERVEMODULE_H_ */
