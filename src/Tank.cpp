#include "Tank.h"
#include <iostream>
#include <math.h>

const std::string Tank::CONTROL_TYPE_KEY = "Drive Control Scheme";

const int         Tank::CONTROL_TYPE_TANK_TWO_JOYSTICKS            = 0;
const std::string Tank::CONTROL_TYPE_TANK_TWO_JOYSTICKS_KEY        = "Tank Drive (two joysticks)";
const int         Tank::CONTROL_TYPE_TANK_GAMEPAD                  = 1;
const std::string Tank::CONTROL_TYPE_TANK_GAMEPAD_KEY              = "Tank Drive (two gamepad sticks)";
const int         Tank::CONTROL_TYPE_ARCADE_ONE_JOYSTICK           = 2;
const std::string Tank::CONTROL_TYPE_ARCADE_ONE_JOYSTICK_KEY       = "Arcade Drive (one joystick)";
const int         Tank::CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK      = 3;
const std::string Tank::CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK_KEY  = "Arcade Drive (one gamepad stick)";
const int         Tank::CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS     = 4;
const std::string Tank::CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS_KEY = "Arcade Drive (two gamepad sticks)";
const int		 Tank::CONTROL_TYPE_TWIST = 5;
const std::string Tank::CONTROL_TYPE_TWIST_KEY = "Come on now, let's do the twist";
const int		 Tank::CONTROL_TYPE_STICK_TWIST = 6;
const std::string Tank::CONTROL_TYPE_STICK_TWIST_KEY = "aLEXIS PlZ";

const std::string Tank::ARCADE_TUNING_PARAM_A_KEY     = "Arcade Drive Tuning Parameter (A)";
const double      Tank::ARCADE_TUNING_PARAM_A_DEFAULT = 0.0;
const std::string Tank::ARCADE_TUNING_PARAM_B_KEY = "Arcade Drive Tuning Parameter (B)";
const double      Tank::ARCADE_TUNING_PARAM_B_DEFAULT = 1.0;

bool isTwisting = false;
float setAngle;

Tank::Tank(ShifterController &Left, ShifterController &Right):
		a_LeftSide(Left),
		a_RightSide(Right)
{
}

Tank::~Tank()
{
}

void Tank::Init()
{
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_TANK_TWO_JOYSTICKS_KEY,
									(void *)&CONTROL_TYPE_TANK_TWO_JOYSTICKS);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_TANK_GAMEPAD_KEY,
									(void *)&CONTROL_TYPE_TANK_GAMEPAD);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_ARCADE_ONE_JOYSTICK_KEY,
									(void *)&CONTROL_TYPE_ARCADE_ONE_JOYSTICK);
	a_ControlTypeChooser.AddDefault(CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK_KEY,
									(void *)&CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS_KEY,
									(void *)&CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_TWIST_KEY,
									(void *)&CONTROL_TYPE_TWIST);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_STICK_TWIST_KEY,
									(void *)&CONTROL_TYPE_STICK_TWIST);
	SmartDashboard::PutData(CONTROL_TYPE_KEY, &a_ControlTypeChooser);

	SmartDashboard::PutNumber(ARCADE_TUNING_PARAM_A_KEY, ARCADE_TUNING_PARAM_A_DEFAULT);
	SmartDashboard::PutNumber(ARCADE_TUNING_PARAM_B_KEY, ARCADE_TUNING_PARAM_B_DEFAULT);
}

void Tank::Enable()
{
	a_LeftSide.Enable();
	a_RightSide.Enable();
}

void Tank::Disable()
{
	a_LeftSide.Disable();
	a_RightSide.Disable();
}

double EtherL(double fwd, double rcw, double a, double b) {
	return fwd + b*rcw*(1-fwd);
}

double EtherR(double fwd, double rcw, double a, double b) {
	return fwd - b*rcw + fwd*rcw*(b-a-1);
}

void EtherArcade(double fwd, double rcw, double a, double b, double &out_left, double &out_right) {
	// +---+---+ //
	// | 2 | 1 | //
	// +---+---+ //
	// | 3 | 4 | //
	// +---+---+ //
	if (fwd >= 0) {
		if (rcw >= 0) { // Q1
			out_left = EtherL(fwd,rcw,a,b);
			out_right = EtherR(fwd,rcw,a,b);
		} else { // Q2
			out_left = EtherR(fwd,-rcw,a,b);
			out_right = EtherL(fwd,-rcw,a,b);
		}
	} else {
		if (rcw>=0) { // Q4
			out_left = -EtherR(-fwd,rcw,a,b);
			out_right = -EtherL(-fwd,rcw,a,b);
		} else { // Q3
			out_left = -EtherL(-fwd,-rcw,a,b);
			out_right = -EtherR(-fwd,-rcw,a,b);
		}
	}
}

void Tank::Update(Joystick &stick, Joystick &stick2, float gyroValue)
{

	if(stick.GetRawButton(11))
	{
		a_LeftSide.ShiftHigh();
	}
	else if(stick.GetRawButton(12))
	{
		a_LeftSide.ShiftLow();
	}

	// SendableChooser *controlTypeChooser =
	// 		(SendableChooser *)SmartDashboard::GetData(CONTROL_TYPE_KEY);
	int *controlType;
	int controlTypeTwist = CONTROL_TYPE_TWIST;

	if(!isTwisting) {
		controlType = (int *)a_ControlTypeChooser.GetSelected();
	} else {
		controlType = &controlTypeTwist;
	}
	// printf("Control Type : %d\n", *controlType);

	if (controlType == NULL)
	{
		std::cout << "error reading control type" << std::endl;
		return;
	}

	double XInput1 = stick.GetX();
	double XInput2 = stick2.GetRawAxis(0);
	double YInput1 = stick.GetY();
	double YInput2 = stick2.GetRawAxis(1);

	// at this time this only applies to the different drive schemes that we actually use

	if(fabs(XInput1) < kJoystickDeadzone) {
		XInput1 = 0;
	} else if(XInput1 == 1.0 || XInput1 == -1.0) {
		// leave it alone- drive it like ya stole it
	} else { // remove deadzone, push it to the higher end
		if(XInput1 < 0) {
			XInput1 += kJoystickDeadzone;
		} else {
			XInput1 -= kJoystickDeadzone;
		}
	}

	if(fabs(XInput2) < kJoystickDeadzone) {
		XInput2 = 0;
	} else if (XInput2 == 1.0 || XInput2 == -1.0){
		// see above
	} else {
		if(XInput2 < 0) {
			XInput2 += kJoystickDeadzone;
		} else {
			XInput2 -= kJoystickDeadzone;
		}
	}

	if(fabs(YInput1) < kJoystickDeadzone) {
		YInput1= 0;
	} else if (YInput1 == 1.0 || YInput1 == -1.0){
		// see above
	} else {
		if(YInput1 < 0) {
			YInput1 += kJoystickDeadzone;
		} else {
			YInput1 -= kJoystickDeadzone;
		}
	}

	if(fabs(YInput2) < kJoystickDeadzone) {
		YInput2 = 0;
	} else if (YInput2 == 1.0 || YInput2 == -1.0){
		// see above
	} else {
		if(YInput2 < 0) {
			YInput2 += kJoystickDeadzone;
		} else {
			YInput2 -= kJoystickDeadzone;
		}
	}

	if(stick2.GetRawAxis(3) > 0.5) {
		XInput1 *= 0.8;
		XInput2 *= 0.8;
		YInput1 *= 0.8;
		YInput2 *= 0.8;
	}

	// Tuning parameters for the arcade drive algorithm
	double a = SmartDashboard::GetNumber(ARCADE_TUNING_PARAM_A_KEY, ARCADE_TUNING_PARAM_A_DEFAULT);
	double b = SmartDashboard::GetNumber(ARCADE_TUNING_PARAM_B_KEY, ARCADE_TUNING_PARAM_B_DEFAULT);

	// Outputs to the left/right motor groups
	double left = 0.0;
	double right = 0.0;

	int diff = fabs(setAngle - gyroValue);

	switch (*controlType)
	{
	case CONTROL_TYPE_TANK_TWO_JOYSTICKS:
		left = YInput2;
		right = -1.0 * YInput1;
		break;
	case CONTROL_TYPE_TANK_GAMEPAD:
		left = stick2.GetRawAxis(1);
		right = -1.0 * stick2.GetRawAxis(4);
		break;
	case CONTROL_TYPE_ARCADE_ONE_JOYSTICK:
		EtherArcade(YInput1, XInput1 * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	case CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK:
		EtherArcade(pow(YInput2, 3), pow(XInput2, 3) * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	case CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS:
		EtherArcade(stick2.GetRawAxis(1), stick2.GetRawAxis(4) * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	case CONTROL_TYPE_TWIST:
		if(gyroValue < setAngle - 3) {
			if(diff > 10) {
				left = -0.3;
				right = -0.3;
			} else {
				left = -0.2;
				right = -0.2;
			}
		} else if(gyroValue > setAngle + 3) {
			if(diff > 10) {
				left = 0.3;
				right = 0.3;
			} else {
				left = 0.2;
				right = 0.2;
			}
		} else {
			left = 0;
			right = 0;
			isTwisting = false;
		}
		break;
	case CONTROL_TYPE_STICK_TWIST:
		float power = stick2.GetY();
		float left = pow(stick2.GetRawAxis(3), 3);
		float right = pow(stick2.GetRawAxis(2), 3);
		left = power - left + right;
		right = -1.0 * (power - right + left);
		break;
	}

	SmartDashboard::PutNumber("Left Encoder Distance", a_LeftSide.GetDistance());
	SmartDashboard::PutNumber("Right Encoder Distance", a_RightSide.GetDistance());

	SmartDashboard::PutNumber("Left Encoder Rate", a_LeftSide.GetRate());
	SmartDashboard::PutNumber("Right Encoder Rate", a_RightSide.GetRate());

	SmartDashboard::PutNumber("Left Tank Input", left);
	SmartDashboard::PutNumber("Right Tank Input", right);

	a_LeftSide.Set(left);
	a_RightSide.Set(right);
}

void Tank::AutonUpdate(double left, double right)
{
	double leftDistance = fabs(a_LeftSide.GetDistance());
	double rightDistance = fabs(a_RightSide.GetDistance());
	SmartDashboard::PutNumber("left auto", leftDistance);
	SmartDashboard::PutNumber("right auto", rightDistance);
	a_LeftSide.Set(left);
    a_RightSide.Set(1.09 * right);
}

void Tank::AutonUpdateDriveStraightTest(double left, double right, Joystick &stick)
{
	double leftDistance = fabs(a_LeftSide.GetDistance());
	double rightDistance = fabs(a_RightSide.GetDistance());
	SmartDashboard::PutNumber("left auto", leftDistance);
	SmartDashboard::PutNumber("right auto", rightDistance);
	// difference in inches:
	/*
	double inchDifference = (leftDistance - rightDistance);
	if(fabs(inchDifference) < 0.10) {
		a_LeftSide.Set(left);
    a_RightSide.Set(right);
	} else if(inchDifference < 0) {
		// turn right
		a_LeftSide.Set(1.10 * left);
    a_RightSide.Set(0.90 * right);
	} else {
		//turn left
		a_LeftSide.Set(0.90 * left);
    a_RightSide.Set(1.10 * right);
	}
	*/
	if(stick.GetRawButton(9)) {
		a_LeftSide.Set(0.740 * left);
		a_RightSide.Set(1.35 * right);
		printf("driving to the left\n");
	} else if(stick.GetRawButton(10)) {
		a_LeftSide.Set(1.35 * left);
		a_RightSide.Set(0.740 * right);
		printf("driving to the right\n");
	} else {
		a_LeftSide.Set(left);
		a_RightSide.Set(right);
		printf("driving straight\n");
	}
}

void Tank::AutonUpdateDriveStraight(double left, double right)
{
	double leftDistance = fabs(a_LeftSide.GetDistance());
	double rightDistance = fabs(a_RightSide.GetDistance());
	SmartDashboard::PutNumber("left auto", leftDistance);
	SmartDashboard::PutNumber("right auto", rightDistance);
	// difference in inches:
	double inchDifference = (leftDistance - rightDistance);
	if(fabs(inchDifference) < 0.10) {
		a_LeftSide.Set(left);
		a_RightSide.Set(right);
	} else if(inchDifference < 0) {
		// turn right
		a_LeftSide.Set(1.5 * left);
		a_RightSide.Set(0.667 * right);
	} else {
		//turn left
		a_LeftSide.Set(0.667 * left);
		a_RightSide.Set(1.5 * right);
	}
}

void Tank::SimpleUpdate(Joystick &stick, Joystick &stick2)
{
	a_LeftSide.Set(stick2.GetY());
	a_RightSide.Set( -1.0 * stick.GetY());
}

void Tank::SetTwistingMode()
{
	isTwisting = 1;
}

void Tank::DisableTwist()
{
	isTwisting = 0;
}

void Tank::SetTwistingAngle(float angle)
{
	setAngle = angle;
}

void Tank::SetTwistingRelAngle(float gyroAngle, float angle)
{
	setAngle = gyroAngle + angle;
}

float Tank::GetDistance()
{
	return ( fabs( a_LeftSide.GetDistance() ) + fabs( a_RightSide.GetDistance() ) ) / 2.0 ;
}

void Tank::ResetEncoders()
{
	a_LeftSide.ResetEncoder();
	a_RightSide.ResetEncoder();
}

bool Tank::IsTwisting()
{
	return isTwisting;
}
