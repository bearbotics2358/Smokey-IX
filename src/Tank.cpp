#include <Prefs.h>
#include <Tank.h>
#include <math.h>
#include <Joystick.h>
#include <iostream>


Tank::Tank(ShifterController &Left, ShifterController &Right)
:a_LeftSide(Left),
 a_RightSide(Right)
{

}

Tank::~Tank()
{
}

void Tank::Update(Joystick &stick, Joystick &stick2) {

	if(stick.GetRawButton(2))
	{
		a_LeftSide.ShiftHigh();
	}
	else if(stick.GetRawButton(3))
	{
		a_LeftSide.ShiftLow();
	}

	if(stick.GetY() > 0) // Upper 2
	{
		if(stick.GetX() > 0) // Quadrant 1
		{
			a_LeftSide.Set(stick.GetY());
			a_RightSide.Set(stick.GetY() - stick.GetX());
		}
		else if(stick.GetX() < 0) // Quadrant 2
		{
			a_LeftSide.Set(stick.GetY() + stick.GetX());
			a_RightSide.Set(stick.GetY());
		}
	}
	else if(stick.GetY() < 0) // Lower 2
	{
		if(stick.GetX() > 0) // Quadrant 4
		{
			a_LeftSide.Set(stick.GetY());
			a_RightSide.Set(stick.GetY() + stick.GetX());
		}
		else if(stick.GetX() < 0) // Quadrant 3
		{
			a_LeftSide.Set(stick.GetY() - stick.GetX());
			a_RightSide.Set(stick.GetY());
		}
	}

	double pow = stick2.GetRawAxis(1);
	if(pow > 0)
	{
		a_LeftSide.Set(pow - stick2.GetRawAxis(2));
		a_RightSide.Set(pow - stick2.GetRawAxis(3));
	}
	else
	{
		a_LeftSide.Set(pow + stick2.GetRawAxis(2));
		a_RightSide.Set(pow + stick2.GetRawAxis(3));
	}

}

