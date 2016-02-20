#pragma once

#include "SpeedController.h"
#include <WPILib.h>

class ShifterController: public PIDOutput
{
public:
	ShifterController(uint32_t leftMotor, uint32_t rightMotor, DoubleSolenoid &shifter,
					  uint32_t encPort1, uint32_t encPort2);
	virtual ~ShifterController() = default;

	void Enable();
	void Disable();

	void EnablePIDControl();
	void DisablePIDControl();
	bool IsPIDControlEnabled();

	void SetEncoderInverted(bool inverted);
	void SetDriveInverted(bool inverted);

	void Set(float value, uint8_t syncGroup = 0);
	float Get();

	void ShiftToggle();
	void Shift(int state);
	void ShiftLow();
	void ShiftHigh();

	float GetDistance();
	float GetRate();
	void ResetEncoder();

	void PIDWrite(float output);

private:
	const float kRateSetpointScaleFactor = 800.0;

	CanTalonSRX _leftMotor;
	CanTalonSRX _rightMotor;
	DoubleSolenoid &_solenoid;

	Encoder _encoder;

	PIDController _pid;

	bool _enabled = false;
	float _speed = 0.0;
};
