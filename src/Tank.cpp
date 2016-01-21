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

void Tank::Update(Joystick &stick, Joystick &stick2) {
		a_LeftSide.Set(-1.0 * stick.GetY());
		a_RightSide.Set(stick.GetY());
		a_LeftSide.Set(1.0 * stick.GetX());
		a_RightSide.Set(1.0 * stick.GetX());



		a_LeftSide.Shift((int) stick.GetY() > 0);
}

