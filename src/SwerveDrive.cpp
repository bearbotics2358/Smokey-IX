/*
 * SwerveDrive.cpp
 *
 *  Created on: Mar 22, 2016
 *      Author: hstechclub
 */

#include "SwerveDrive.h"

const std::string SwerveDrive::CONTROL_TYPE_KEY = "Drive Scheme";

const int         SwerveDrive::CONTROL_TYPE_SWERVE = 0;
const std::string SwerveDrive::CONTROL_TYPE_SWERVE_KEY = "Swerve Drive";
const int         SwerveDrive::CONTROL_TYPE_CRAB   = 1;
const std::string SwerveDrive::CONTROL_TYPE_CRAB_KEY   = "Crab Drive";

SwerveDrive::SwerveDrive(SwerveModule &FR, SwerveModule &FL, SwerveModule &BL, SwerveModule &BR, float Length, float Width)
: a_FrontRight(FR),
  a_FrontLeft(FL),
  a_BackLeft(BL),
  a_BackRight(BR)
{
	a_RobotLength = Length;
	a_RobotWidth = Width;
	a_ChassisRadius = sqrt(pow(a_RobotLength, 2) + pow(a_RobotWidth,2));
}

void SwerveDrive::Init()
{
	InitSendableChooser(); // please, please, PLEASE call this first- otherwise our drive mode might not initialize properly
}

void SwerveDrive::InitSendableChooser()
{
	a_ControlTypeChooser.AddDefault(CONTROL_TYPE_SWERVE_KEY, (void *)&CONTROL_TYPE_SWERVE);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_CRAB_KEY, (void *)&CONTROL_TYPE_CRAB);

	SmartDashboard::PutData(CONTROL_TYPE_KEY, &a_ControlTypeChooser);
}

void SwerveDrive::Update(Joystick &stick, Joystick &stick2, float gyroValue)
{
	float zInput = stick.GetZ(); // Rotation Clockwise
	float xInput = stick.GetX(); // Strafe
	float yInput = stick.GetY(); // Forward

	int *controlType;

	controlType = (int *)a_ControlTypeChooser.GetSelected();

	if (controlType == NULL)
	{
		std::cout << "error reading control type" << std::endl;
		return;
	}

	float A = xInput - zInput * (a_RobotLength / a_ChassisRadius);
	float B = xInput + zInput * (a_RobotLength / a_ChassisRadius);
	float C = yInput - zInput * (a_RobotWidth / a_ChassisRadius);
	float D = yInput + zInput * (a_RobotWidth / a_ChassisRadius);

	float max = -9 * pow(10,4);

	double frSpeed = 0.0;
	double frAngle = 0.0;

	double flSpeed = 0.0;
	double flAngle = 0.0;

	double blSpeed = 0.0;
	double blAngle = 0.0;

	double brSpeed = 0.0;
	double brAngle = 0.0;

	switch (*controlType)
	{
	case CONTROL_TYPE_SWERVE: // Thank you, Ether, for agreeing with the math I did
		frSpeed = sqrt(pow(B,2) + pow(C,2));
		flSpeed = sqrt(pow(B,2) + pow(D,2));
		blSpeed = sqrt(pow(A,2) + pow(D,2));
		brSpeed = sqrt(pow(A,2) + pow(C,2));

		max = frSpeed;
		if(flSpeed > max) {
			max = flSpeed;
		}
		if(blSpeed > max) {
			max = blSpeed;
		}
		if(brSpeed > max) {
			max = brSpeed;
		}
		if(max > 1) { // This is done so that if a speed greater than 1 is calculated, all are reduced proportionally
			frSpeed /= max;
			flSpeed /= max;
			blSpeed /= max;
			brSpeed /= max;
		}

		frAngle = (atan2(C,B) * 180.0 / M_PI) + 180.0; // The + 180.0 is there because atan2() returns -180 to 180, so this makes it 0 to 360
		flAngle = (atan2(D,B) * 180.0 / M_PI) + 180.0;
		blAngle = (atan2(D,A) * 180.0 / M_PI) + 180.0;
		brAngle = (atan2(C,A) * 180.0 / M_PI) + 180.0;
		break;
	case CONTROL_TYPE_CRAB:
		float setAngle = atan2(yInput,xInput); // find the angle the stick is pointed to, use that for all wheels
		frAngle = setAngle;
		flAngle = setAngle;
		blAngle = setAngle;
		brAngle = setAngle;

		float setSpeed = sqrt(pow(xInput,2) + pow(yInput,2)); // find the r of the joystick vector, if you think about it in polar coordinates
		frSpeed = setSpeed;
		flSpeed = setSpeed;
		blSpeed = setSpeed;
		brSpeed = setSpeed;
		break;
	}

	a_FrontRight.Set(frAngle, frSpeed);
	a_FrontLeft.Set(flAngle, flSpeed);
	a_BackLeft.Set(blAngle, blSpeed);
	a_BackRight.Set(brAngle, brSpeed);
}
