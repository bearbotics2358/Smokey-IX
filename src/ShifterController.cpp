#include <ShifterController.h>
#include "LiveWindow/LiveWindow.h"


ShifterController::ShifterController(uint32_t leftMotor, uint32_t rightMotor,
									 DoubleSolenoid &shifter,
									 uint32_t encPort1, uint32_t encPort2):
		_leftMotor(leftMotor),
		_rightMotor(rightMotor),
		_solenoid(shifter),
		_encoder(encPort1, encPort2),
		_speed(0)
{

}

ShifterController::~ShifterController()
{
}

void ShifterController::SetEncoderInverted(bool inverted)
{
	_encoder.SetReverseDirection(inverted);
}

void ShifterController::SetDriveInverted(bool inverted)
{
}

void ShifterController::Set(float speed, uint8_t syncGroup)
{
	_rightMotor.Set(speed);
	_leftMotor.Set(speed);
	_speed = speed;
}

void ShifterController::ShiftToggle()
{
	if(_solenoid.Get() == DoubleSolenoid::kForward) {
		_solenoid.Set(DoubleSolenoid::kReverse);
	} else {
		_solenoid.Set(DoubleSolenoid::kForward);
	}
}

void ShifterController::Shift(int shift)
{
	if(shift == 0) {
		_solenoid.Set(DoubleSolenoid::kReverse);
	} else {
		_solenoid.Set(DoubleSolenoid::kForward);
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
	return _speed;
}

void ShifterController::Disable()
{
	// See Talon.Disable()
	_rightMotor.Set(0);
	_leftMotor.Set(0);
}

float ShifterController::GetDistance()
{
	return _encoder.GetDistance();
}

void ShifterController::ResetEncoder()
{
	_encoder.Reset();
}

void ShifterController::PIDWrite(float output)
{
	Set(output);
}
