#include <Prefs.h>
#include <Tank.h>
#include <math.h>
#include <Joystick.h>
#include <iostream>

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

const std::string Tank::ARCADE_TUNING_PARAM_A_KEY     = "Arcade Drive Tuning Parameter (A)";
const double      Tank::ARCADE_TUNING_PARAM_A_DEFAULT = 0.0;
const std::string Tank::ARCADE_TUNING_PARAM_B_KEY = "Arcade Drive Tuning Parameter (B)";
const double      Tank::ARCADE_TUNING_PARAM_B_DEFAULT = 1.0;

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
	a_ControlTypeChooser.AddDefault(CONTROL_TYPE_TANK_TWO_JOYSTICKS_KEY,
			               (void *)&CONTROL_TYPE_TANK_TWO_JOYSTICKS);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_TANK_GAMEPAD_KEY,
			              (void *)&CONTROL_TYPE_TANK_GAMEPAD);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_ARCADE_ONE_JOYSTICK_KEY,
			              (void *)&CONTROL_TYPE_ARCADE_ONE_JOYSTICK);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK_KEY,
			              (void *)&CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK);
	a_ControlTypeChooser.AddObject(CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS_KEY,
			              (void *)&CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS);
	SmartDashboard::PutData(CONTROL_TYPE_KEY, &a_ControlTypeChooser);

	SmartDashboard::PutNumber(ARCADE_TUNING_PARAM_A_KEY, ARCADE_TUNING_PARAM_A_DEFAULT);
	SmartDashboard::PutNumber(ARCADE_TUNING_PARAM_B_KEY, ARCADE_TUNING_PARAM_B_DEFAULT);
}

/*
function L(fwd,rcw,a,b: float): float;
begin
L := fwd + b*rcw*(1-fwd);
end;

function R(fwd,rcw,a,b: float): float;
begin
R := fwd - b*rcw + fwd*rcw*(b-a-1);
end;

procedure LR(fwd,rcw,a,b: float; var left,right: float);
begin
if fwd>=0 then
 begin
 if rcw>=0 then
  begin {Q1}
  left := L(fwd,rcw,a,b);
  right := R(fwd,rcw,a,b);
  end
 else
  begin {Q2}
  left := R(fwd,-rcw,a,b);
  right := L(fwd,-rcw,a,b);
  end
 end
else
 begin
 if rcw>=0 then
  begin {Q4}
  left := -R(-fwd,rcw,a,b);
  right := -L(-fwd,rcw,a,b);
  end
 else
  begin {Q3}
  left := -L(-fwd,-rcw,a,b);
  right := -R(-fwd,-rcw,a,b);
  end
 end;
end;
 */
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

void Tank::Update(Joystick &stick, Joystick &stick2) {

	if(stick.GetRawButton(11))
	{
		a_LeftSide.ShiftHigh();
	}
	else if(stick.GetRawButton(12))
	{
		a_LeftSide.ShiftLow();
	}

	/*
	if(stick.GetY() > 0) // Upper 2
	{
		if(stick.GetX() > 0) // Quadrant 1
		{
			a_LeftSide.Set(stick.GetY());
			a_RightSide.Set(-1.0 * (stick.GetY() - stick.GetX()));
		}
		else if(stick.GetX() < 0) // Quadrant 2
		{
			a_LeftSide.Set(stick.GetY() + stick.GetX());
			a_RightSide.Set(-1.0 * stick.GetY());
		}
	}
	else if(stick.GetY() < 0) // Lower 2
	{
		if(stick.GetX() > 0) // Quadrant 4
		{
			a_LeftSide.Set(stick.GetY());
			a_RightSide.Set(-1.0 * (stick.GetY() + stick.GetX()));
		}
		else if(stick.GetX() < 0) // Quadrant 3
		{
			a_LeftSide.Set(stick.GetY() - stick.GetX());
			a_RightSide.Set(-1.0 * stick.GetY());
		}
	}



	double pow = stick2.GetRawAxis(1);
	if(pow > 0)
	{
		a_LeftSide.Set(pow - stick2.GetRawAxis(2));
		a_RightSide.Set(-1.0 * (pow - stick2.GetRawAxis(3)));
	}
	else
	{
		a_LeftSide.Set(pow + stick2.GetRawAxis(2));
		a_RightSide.Set(-1.0 * (pow + stick2.GetRawAxis(3)));
	}
<<<<<<< HEAD

	*/

	// SendableChooser *controlTypeChooser =
	// 		(SendableChooser *)SmartDashboard::GetData(CONTROL_TYPE_KEY);
	int *controlType = (int *)a_ControlTypeChooser.GetSelected();

	if (controlType == NULL)
	{
		std::cout << "error reading control type" << std::endl;
		return;
	}

	// Tuning parameters for the arcade drive algorithm
	double a = SmartDashboard::GetNumber(ARCADE_TUNING_PARAM_A_KEY, ARCADE_TUNING_PARAM_A_DEFAULT);
	double b = SmartDashboard::GetNumber(ARCADE_TUNING_PARAM_B_KEY, ARCADE_TUNING_PARAM_B_DEFAULT);

	// Outputs to the left/right motor groups
	double left = 0.0;
	double right = 0.0;

	switch (*controlType)
	{
	case CONTROL_TYPE_TANK_TWO_JOYSTICKS:
		left = stick2.GetY();
		right = -1.0 * stick.GetY();
		break;
	case CONTROL_TYPE_TANK_GAMEPAD:
		left = stick2.GetRawAxis(1);
		right = stick2.GetRawAxis(5);
		break;
	case CONTROL_TYPE_ARCADE_ONE_JOYSTICK:
		EtherArcade(stick.GetY(), stick.GetX() * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	case CONTROL_TYPE_ARCADE_ONE_GAMEPAD_STICK:
		EtherArcade(stick2.GetRawAxis(1), stick2.GetRawAxis(0) * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	case CONTROL_TYPE_ARCADE_TWO_GAMEPAD_STICKS:
		EtherArcade(stick2.GetRawAxis(1), stick2.GetRawAxis(4) * -1.0, a, b, left, right);
		right *= -1.0;
		break;
	}

	a_LeftSide.Set(left);
	a_RightSide.Set(right);
}
