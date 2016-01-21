#include <ShifterController.h>
#include "LiveWindow/LiveWindow.h"


ShifterController::ShifterController(CanTalonSRX &Left, CanTalonSRX &Right, DoubleSolenoid &Shift)
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

void ShifterController::ShiftToggle()
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

void ShifterController::Shift(int shift)
{
	if(shift == 0)
	{
		Sol.Set(DoubleSolenoid::kReverse);
	}
	else
	{
		Sol.Set(DoubleSolenoid::kForward);
	}
}

void ShifterController::ShiftLow()
{
	Shift(0);
}

void ShifterController::ShiftHigh()
{
	Shift(1);
}

float ShifterController::Get()
{
	return m_speed;
}

void ShifterController::Disable()
{
	// See Talon.Disable()
	RightC.Set(0);
	LeftC.Set(0);
}

void ShifterController::SetInverted(bool isInverted)
{

}

bool ShifterController::GetInverted()
{
	return false;
}

void ShifterController::PIDWrite(float output)
{
	Set(output);
}
