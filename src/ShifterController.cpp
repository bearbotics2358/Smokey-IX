#include <ShifterController.h>
#include "LiveWindow/LiveWindow.h"

ShifterController::ShifterController(uint32_t leftMotor, uint32_t rightMotor,
									 DoubleSolenoid &shifter,
									 uint32_t encPort1, uint32_t encPort2):
		_leftMotor(leftMotor),
		_rightMotor(rightMotor),
		_solenoid(shifter),
		_encoder(encPort1, encPort2),
		_pid(0.0, 0.0, 0.0, &_encoder, this)
{
	_encoder.SetPIDSourceType(PIDSourceType::kRate);
	_pid.SetOutputRange(-1.0, 1.0);
}

void ShifterController::Enable()
{
	_enabled = true;
	// EnablePIDControl();
}

void ShifterController::Disable()
{
	_enabled = false;
	DisablePIDControl();

	// See Talon.Disable()
	_rightMotor.Set(0);
	_leftMotor.Set(0);
}

void ShifterController::EnablePIDControl()
{
	// Make sure the PID values are present on the dashboard
	SmartDashboard::PutNumber("ShifterController P",
			SmartDashboard::GetNumber("ShifterController P", 0.001));
	SmartDashboard::PutNumber("ShifterController I",
			SmartDashboard::GetNumber("ShifterController I", 0.0));
	SmartDashboard::PutNumber("ShifterController D",
			SmartDashboard::GetNumber("ShifterController D", 0.0));
	_pid.Enable();
}

void ShifterController::DisablePIDControl()
{
	_pid.Disable();
}

bool ShifterController::IsPIDControlEnabled()
{
	return _pid.IsEnabled();
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
	if (!_enabled)
	{
		return;
	}

	if (IsPIDControlEnabled())
	{
		_pid.SetPID(
				SmartDashboard::GetNumber("ShifterController P", 0.0),
				SmartDashboard::GetNumber("ShifterController I", 0.0),
				SmartDashboard::GetNumber("ShifterController D", 0.0));
		_pid.SetSetpoint(speed * kRateSetpointScaleFactor);
	}
	else
	{
		_rightMotor.Set(speed);
		_leftMotor.Set(speed);
		_speed = speed;
	}
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

float ShifterController::GetDistance()
{
	return _encoder.GetRaw();
}

float ShifterController::GetRate()
{
	return _encoder.GetRate();
}

void ShifterController::ResetEncoder()
{
	_encoder.Reset();
}

void ShifterController::PIDWrite(float output)
{
	if (!_enabled)
	{
		return;
	}

	_rightMotor.Set(output);
	_leftMotor.Set(output);
	_speed = output;
}
