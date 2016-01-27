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

void EtherArcade(double fwd, double rcw, double &out_left, double &out_right) {
	const double a = 0.0;
	const double b = 1.0;

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

	if(stick.GetRawButton(2))
	{
		a_LeftSide.ShiftHigh();
	}
	else if(stick.GetRawButton(3))
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

	a_LeftSide.Set(stick2.GetY());
	a_RightSide.Set(-1.0 * stick.GetY());

}

