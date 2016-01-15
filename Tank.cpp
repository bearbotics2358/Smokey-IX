#include <Prefs.h>
#include <Tank.h>
#include <math.h>




Tank::Tank(ShifterController &Left, ShifterController &Right)
:a_LeftSide(Left),
 a_RightSide(Right)
{

}

Tank::~Tank()
{
}

void Tank::Update(Joystick &stick, Joystick &stick2) {
	if(abs(stick.GetZ()) <= 0.125){
		a_LeftSide.Set(stick.GetY());
		a_RightSide.Set(stick.GetY());
	}
	else{
		a_LeftSide.Set(stick.GetZ());
		a_RightSide.Set(-1.0 * stick.GetZ());
	}
}

