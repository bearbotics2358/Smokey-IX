#include <Prefs.h>
#include <Tank.h>
#include <math.h>
#include <Joystick.h>


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
	a_LeftSide.Set(-1.0 * stick.GetY());
	a_RightSide.Set(stick.GetY());
	a_LeftSide.Set(1.0 * stick.GetX());
	a_RightSide.Set(1.0 * stick.GetX());

	a_LeftSide.Shift((int) stick.GetY() > 0);
}
