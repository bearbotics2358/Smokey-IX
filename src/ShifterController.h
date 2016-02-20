#pragma once

#include "SpeedController.h"
#include <WPILib.h>

class ShifterController: public PIDOutput
{
public:
	ShifterController(uint32_t leftMotor, uint32_t rightMotor, DoubleSolenoid &shifter,
					  uint32_t encPort1, uint32_t encPort2);

	void SetEncoderInverted(bool inverted);
	void SetDriveInverted(bool inverted);

	void Set(float value, uint8_t syncGroup = 0);
	float Get();

	void Disable();

	void ShiftToggle();
	void Shift(int state);
	void ShiftLow();
	void ShiftHigh();

	float GetDistance();
	void ResetEncoder();

	void PIDWrite(float output);

private:
	CanTalonSRX _leftMotor;
	CanTalonSRX _rightMotor;
	DoubleSolenoid &_solenoid;

	Encoder _encoder;

	float _speed;
};
