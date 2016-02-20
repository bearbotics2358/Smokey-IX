#pragma once

#include "SpeedController.h"
#include "PIDOutput.h"
#include "PWM.h"
#include "CanTalonSRX.h"       // This file isn't indexed for some reason
#include <WPILib.h>

class ShifterController
{
public:
	ShifterController(uint32_t LeftMotor, uint32_t RightMotor, DoubleSolenoid &Shift, int LeftPort, int RightPort);
	~ShifterController();
	void Set(float value, uint8_t syncGroup = 0);
	void ShiftToggle();
	void Shift(int state);
	void ShiftLow();
	void ShiftHigh();
	float Get();
	void Disable();
	void SetInverted(bool isInverted);
	bool GetInverted();
	float GetDistance();
	void ResetEncoder();

	void PIDWrite(float output);

private:
	CanTalonSRX LeftC;
	CanTalonSRX RightC;
	DoubleSolenoid &Sol;

	Encoder EncoderC;

	float m_speed;
};
