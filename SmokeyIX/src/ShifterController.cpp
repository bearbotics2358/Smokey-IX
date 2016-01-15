#include <ShifterController.h>
#include "LiveWindow/LiveWindow.h"


ShifterController::ShifterController(Victor &Left, Victor &Right, DoubleSolenoid &Shift)
: LeftC(Left),
  RightC(Right),
  Sol(Shift),

  m_speed(0)
{

}

ShifterController::~ShifterController()
{
}

void ShifterController::Set(float speed, uint8_t syncGroup)
{
	RightC.Set(speed);
	LeftC.Set(speed);
	m_speed = speed;
}

void ShifterController::Shift()
{
	if(Sol.Get() == DoubleSolenoid::kForward)
	{
		Sol.Set(DoubleSolenoid::kReverse);

	}
	else
	{
		Sol.Set(DoubleSolenoid::kForward);

	}
}

float ShifterController::Get()
{
	return m_speed;
}

void ShifterController::Disable()
{
	// See Talon.Disable()
	RightC.SetRaw(0);
	LeftC.SetRaw(0);
}

void ShifterController::PIDWrite(float output)
{
	Set(output);
}
